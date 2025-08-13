#include <iostream>
#include <vector>
#include <string>
#include <stack>
using namespace std;

// 实现插入排序算法
void InsertSort(vector<int> &nums)
{

    for (int i = 1; i < nums.size(); i++)
    {
        int key = nums[i];
        int j = i - 1;
        while (j >= 0 && nums[j] > key)
        {
            swap(nums[j], nums[j + 1]);
            j--;
        }
    }
}

int main()
{
    vector<int> nums = {1, 2, 3, 56, 5, 6, 7, 8, 99};
    InsertSort(nums);
    for (int i = 0; i < nums.size(); i++)

        return 0;
}