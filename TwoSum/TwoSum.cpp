#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

vector<int> twoSum(vector<int> &nums, int target) {
  vector<int> results;
  unordered_map<int, int> myHash;
  for (int i = 0; i < nums.size(); ++i) {
    int value = target - nums[i];
    auto it = myHash.find(value);
    if (it != myHash.end())
      return {i, it->second};

    myHash.insert({nums[i], i});
  }

  return results;
}

int main() 
{
  vector<int> nums = {2, 7, 11, 15};
  const int target = 9;
  twoSum(nums, target);
  return 0;
}
