#include <fstream>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

#pragma pack(1)
struct BITMAPFILEHEADER {
  int16_t bfType;
  int32_t bfSize;
  int16_t bfReserved1;
  int16_t bfReserved2;
  int32_t bfOffBits;
};

struct BITMAPINFOHEADER {
  int32_t biSize;
  int32_t biWidth;
  int32_t biHeight;
  int16_t biPlanes;
  int16_t biBitCount;
  int32_t biCompression;
  int32_t biSizeImage;
  int32_t biXPelsPerMeter;
  int32_t biYPelsPerMeter;
  int32_t biClrUsed;
  int32_t biClrImportant;
};
#pragma pack()

void turnRight(unsigned char *inputBuffer, unsigned char *outputBuffer,
               int width, int height, int sizeOfPixel) {

  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      const int newIndex = ((width - i - 1) * height + j) * sizeOfPixel;
      const int oldIndex = (j * width + i) * sizeOfPixel;

      for (int k = 0; k < sizeOfPixel; ++k) {
        outputBuffer[newIndex + k] = inputBuffer[oldIndex + k];
      }
    }
  }
}

void turnLeft(unsigned char *inputBuffer, unsigned char *outputBuffer,
              int width, int height, int sizeOfPixel) {
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      const int newIndex = ((height - i - 1) + j * height) * sizeOfPixel;
      const int oldIndex = (i * width + j) * sizeOfPixel;

      for (int k = 0; k < sizeOfPixel; ++k) {
        outputBuffer[newIndex + k] = inputBuffer[oldIndex + k];
      }
    }
  }
}

void createKernelGauss(double kernel[5][5]) {
  // double kernel[5][5];
  //  initialising standard deviation to 1.0
  double sigma = 1.0;
  double r;
  double s = 2.0 * sigma * sigma;

  // sum is for normalization
  double sum = 0.0;

  // generating 5x5 kernel
  for (int x = -2; x <= 2; x++) {
    for (int y = -2; y <= 2; y++) {
      r = sqrt(x * x + y * y);
      kernel[x + 2][y + 2] = (exp(-(r * r) / s)) / (M_PI * s);
      sum += kernel[x + 2][y + 2];
    }
  }

  // normalising the Kernel
  for (int i = 0; i < 5; ++i)
    for (int j = 0; j < 5; ++j)
      kernel[i][j] /= sum;
}

void applyGaussianFilter(unsigned char *inputBuffer,
                         unsigned char *outputBuffer, int width, int height,
                         int sizeOfPixel) {
  // Gaussian kernel for a 5x5 filter
  double kernel[5][5];
  createKernelGauss(kernel);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      double totalR = 0, totalG = 0, totalB = 0;
      double divisor = 0;

      for (int ky = -2; ky <= 2; ++ky) {
        for (int kx = -2; kx <= 2; ++kx) {
          int posX = x + kx;
          int posY = y + ky;

          if (posX >= 0 && posX < width && posY >= 0 && posY < height) {
            int index = (posY * width + posX) * sizeOfPixel;
            const double kernelParam = kernel[ky + 2][kx + 2];
            totalR += inputBuffer[index] * kernelParam;
            totalG += inputBuffer[index + 1] * kernelParam;
            totalB += inputBuffer[index + 2] * kernelParam;
            divisor += kernelParam;
          }
        }
      }

      int index = (y * width + x) * sizeOfPixel;
      outputBuffer[index] = totalR / divisor;
      outputBuffer[index + 1] = totalG / divisor;
      outputBuffer[index + 2] = totalB / divisor;

      if (sizeOfPixel * 8 == 32) {
        outputBuffer[index + 3] = inputBuffer[index + 3]; // Alpha channel
      }
    }
  }
}

void writeBuffer(const char *file, ifstream &is, BITMAPINFOHEADER bmpInfoHeader,
                 unsigned char *buffer, int bufferSize, int sizeOfPixel,
                 int shiftToData, bool isHeadChanged = true) {
  ofstream os(file, ofstream::binary);
  if (!os.is_open()) {
    cerr << "Output file is unvalid" << endl;
    return;
  }

  is.seekg(0, is.beg);
  os << is.rdbuf();

  if (isHeadChanged) {
    os.seekp(sizeof(BITMAPFILEHEADER));
    os.write(reinterpret_cast<char *>(&bmpInfoHeader),
             sizeof(BITMAPINFOHEADER));
  }

  os.seekp(shiftToData); // Перемещаем указатель к началу пиксельных данных
  os.write((char *)buffer,
           bufferSize *
               sizeOfPixel); // Записывает данные размером newBufferSize
  // байт из буфера newBuffer в поток os
  os.close();

  delete[] buffer;
}

int main() {
  ifstream is("Mandrill.bmp", ifstream::binary);

  if (!is.is_open()) {
    cerr << "File can't be opened!"
         << endl; // Выводит сообщение об ошибке в стандартный поток ошибок
    return 1;
  }

  BITMAPFILEHEADER bmpHeader;
  is.read(reinterpret_cast<char *>(&bmpHeader), sizeof(BITMAPFILEHEADER));
  const int length = bmpHeader.bfSize;
  cout << "Count of memory for image: " << length << "B" << endl;

  is.seekg(sizeof(BITMAPFILEHEADER));
  BITMAPINFOHEADER bmpInfoHeader;
  is.read(reinterpret_cast<char *>(&bmpInfoHeader), sizeof(BITMAPINFOHEADER));
  int width = bmpInfoHeader.biWidth;
  int height = bmpInfoHeader.biHeight;

  // Читаем пиксели из исходного файла и поворачиваем изображение
  int bufferSize = width * height;
  int sizeOfPixel = bmpInfoHeader.biBitCount / 8; // Длина пикселя в байтах
  unsigned char *buffer = new unsigned char[sizeOfPixel * bufferSize];
  const int shiftToData = bmpHeader.bfOffBits;

  is.seekg(
      shiftToData); // Перемещаем указатель к началу пиксельных данных (байт 54)
  is.read((char *)buffer, bufferSize * sizeOfPixel);

  // Создаем новый буфер для повернутого изображения
  bmpInfoHeader.biWidth = height;
  bmpInfoHeader.biHeight = width;

  // write to gauss file -----------------------------
  unsigned char *bufferGauss = new unsigned char[sizeOfPixel * bufferSize];
  applyGaussianFilter(buffer, bufferGauss, width, height, sizeOfPixel);
  writeBuffer("Gauss.bmp", is, bmpInfoHeader, bufferGauss, bufferSize,
              sizeOfPixel, shiftToData, false);

  // write to right file -----------------------------
  unsigned char *rightTransformBuffer =
      new unsigned char[sizeOfPixel * bufferSize];
  turnRight(buffer, rightTransformBuffer, width, height, sizeOfPixel);
  writeBuffer("Rotated1.bmp", is, bmpInfoHeader, rightTransformBuffer,
              bufferSize, sizeOfPixel, shiftToData);

  // write to left file -----------------------------
  unsigned char *leftTransformBuffer =
      new unsigned char[sizeOfPixel * bufferSize];
  turnLeft(buffer, leftTransformBuffer, width, height, sizeOfPixel);
  writeBuffer("Rotated2.bmp", is, bmpInfoHeader, leftTransformBuffer,
              bufferSize, sizeOfPixel, shiftToData);

  is.close();
  delete[] buffer;

  cout << "Success!!!" << endl;
  return 0;
}
