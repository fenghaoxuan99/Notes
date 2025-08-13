#include <iostream>
#include <vector>
#include <string>
#include <stack>
using namespace std;

// 实现希尔排序算法
void ShellSort(vector<int> &nums)
{
    int n = nums.size();
    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < n; i++)
        {
            int key = nums[i];
            int j = i - gap; // 获得前一个元素的索引
            while (j >= 0 && nums[j] > key)
            {
                swap(nums[j], nums[j + gap]);
                j -= gap; // 向前移动gap个位置
            }
        }
    }
}

int main()
{
    vector<int> nums = {1, 2, 3, 56, 5, 6, 7, 8, 99};
    ShellSort(nums);
    for (int i = 0; i < nums.size(); i++)
    {
        cout << nums[i] << " ";
    }

    return 0;
}