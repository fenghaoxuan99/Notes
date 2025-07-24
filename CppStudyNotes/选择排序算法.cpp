#include <iostream>
#include <vector>
#include <string>
#include <stack>
using namespace std;

// 实现选择排序算法
void SelectSort(vector<int> &nums)
{
    for (int i = 0; i < nums.size() - 1; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < nums.size(); j++)
        {
            if (nums[j] < nums[minIndex])
            {
                minIndex = j;
            }
        }
        swap(nums[i], nums[minIndex]);
    }
}

int main()
{
    vector<int> nums = {1, 2, 3, 56, 5, 6, 7, 8, 99};
    SelectSort(nums);
    for (int i = 0; i < nums.size(); i++)
    {
        cout << nums[i] << " ";
    }

    return 0;
}