#include <iostream>
#include <vector>
#include <string>
#include <stack>
using namespace std;

int BinarySearch(vector<int> &nums, int target)
{
    int left  = 0;
    int right = nums.size();
    int mid   = 0;
    while (left < right)
    {
        mid = left + (right - left) / 2;
        if (nums[mid] == target)
        {
            return mid;
        }
        else if (nums[mid] > target)
        {
            right = mid;
        }
        else
        {
            left = mid + 1;
        }
    }
    return -1;
}

int main()
{
    vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int target = 9;
    cout << BinarySearch(nums, target) << endl;

    return 0;
}