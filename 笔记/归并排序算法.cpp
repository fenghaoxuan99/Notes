#include <vector>
#include <algorithm>
using namespace std;

void mergesort(vector<int> &nums, int left, int right)
{
    if (left >= right)
        return;
    int mid = left + (right - left) / 2;
    mergesort(nums, left, mid);
    mergesort(nums, mid + 1, right);

    vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right)
    {
        if (nums[i] <= nums[j])
            temp[k++] = nums[i++];
        else
            temp[k++] = nums[j++];
    }
    while (i <= mid)
    {
        temp[k++] = nums[i++];
    }
    while (j <= right)
    {
        temp[k++] = nums[j++];
    }
    for (int i = 0; i < temp.size(); ++i)
    {
        nums[left + i] = temp[i];
    }
}

void mergeSortOpt(vector<int> &nums)
{
    vector<int> temp(nums.size()); // 预分配
    mergeSortHelper(nums, 0, nums.size() - 1, temp);
}

void mergeSortHelper(vector<int> &nums, int left, int right, vector<int> &temp)
{
    if (left >= right)
        return;
    int mid = left + (right - left) / 2;
    mergeSortHelper(nums, left, mid, temp);
    mergeSortHelper(nums, mid + 1, right, temp);
    // 复用temp的对应区间空间
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right)
    {
        temp[k++] = nums[i] < nums[j] ? nums[i++] : nums[j++];
    }
    while (i <= mid)
        temp[k++] = nums[i++];
    while (j <= right)
        temp[k++] = nums[j++];
    for (int p = 0; p < k; ++p)
    { // 仅拷贝实际使用的部分
        nums[left + p] = temp[p];
    }
}

int main()
{
    return 0;
}
