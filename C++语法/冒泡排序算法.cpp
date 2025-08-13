#include <iostream>
#include <vector>
#include <string>
#include <stack>
using namespace std;

// 实现冒泡排序算法
void BubbleSort(vector<int> &nums)
{
    for (int i = 0; i < nums.size() - 1; i++)
    {
        for (int j = 0; j < nums.size() - i - 1; j++)
        {
            if (nums[j] > nums[j + 1])
            {
                swap(nums[j], nums[j + 1]);
            }
        }
    }
}

int main()
{
    vector<int> nums = {1, 2, 3, 56, 5, 6, 7, 8, 99};
    BubbleSort(nums);
    for (int i = 0; i < nums.size(); i++)
    {
        cout << nums[i] << " ";
    }
    cout << endl;
    return 0;
}