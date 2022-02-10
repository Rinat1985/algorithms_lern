#include <iostream>
#include <unordered_map>

using namespace std;

int lengthOfLongestSubstring(string s) {
  int res = 0;
  unordered_map<char, int> mp;

  for (int i = 0, j = 0; j < s.length(); ++j) {
    if (mp[s[j]] > 0)
      i = max(mp[s[j]], i);

    res = max(res, j - i + 1);
    mp[s[j]] = j + 1;
  }

  return res;
}

int main() {
  string str("abcabcbb");
  cout << lengthOfLongestSubstring(str) << endl;
  return 0;
}
