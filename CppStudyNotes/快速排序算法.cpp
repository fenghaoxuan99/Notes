#include <iostream>
#include <vector>
#include <string>
#include <stack>
using namespace std;

int middle(int a, int b, int c)
{
    if ((a > b) ^ (a > c))
    {
        return a;
    }
    else if ((b < a) ^ (b < c))
    {
        return b;
    }
    else
    {
        return c;
    }
}
// 实现快速排序算法
void QuickSort(vector<int> &nums, int left, int right)
{
    if (left >= right)
    {
        return;
    }
    int i = left;
    int j = right;
    int key1 = nums[left];
    int key2 = nums[right];
    int key3 = nums[left + (right - left) / 2];
    int key = middle(key1, key2, key3);
    if (key == key2)
    {
        swap(nums[left], nums[right]);
    }
    else if (key == key3)
    {
        swap(nums[left + (right - left) / 2], nums[left]);
    }

    while (i < j)
    {
        while (i < j && nums[j] >= key)
        {
            j--;
        }
        if (i < j)
        {
            nums[i] = nums[j];
            i++;
        }
        while (i < j && nums[i] <= key)
        {
            i++;
        }
        if (i < j)
        {
            nums[j] = nums[i];
            j--;
        }
        if (i == j)
        {
            nums[i] = key;
        }
    }
    QuickSort(nums, left, i - 1);
    QuickSort(nums, i + 1, right);
}

int main()
{
    vector<int> nums = {1, 2, 3, 56, 5, 6, 7, 8, 99, 45, 213, 2, 1, 67};
    QuickSort(nums, 0, nums.size() - 1);
    for (int i = 0; i < nums.size(); i++)
    {
        cout << nums[i] << " ";
    }

    return 0;
}