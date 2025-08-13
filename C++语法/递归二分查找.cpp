#include <iostream>
#include <vector>
#include <string>
#include <stack>
using namespace std;

int BinarySearch(vector<int> &nums, int i, int j, int target)
{

    if (i > j)
    {
        return -1;
    }
    int mid = (i + j) / 2;
    if (nums[mid] == target)
    {
        return mid;
    }
    else if (nums[mid] > target)
    {
        return BinarySearch(nums, i, mid - 1, target);
    }
    else
    {
        return BinarySearch(nums, mid + 1, j, target);
    }
}

int main()
{
    vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int target = 5;
    cout << BinarySearch(nums, 0, 8, target) << endl;

    return 0;
}