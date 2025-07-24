#include <iostream>
#include <vector>

// 合并两个有序子数组
void merge(std::vector<int> &arr, int left, int mid, int right)
{
    int n1 = mid - left + 1; // 左子数组的大小
    int n2 = right - mid;    // 右子数组的大小

    // 创建临时数组
    std::vector<int> L(n1), R(n2);

    // 拷贝数据到临时数组
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // 合并临时数组回原数组
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // 拷贝剩余元素
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// 归并排序主函数
void mergeSort(std::vector<int> &arr, int left, int right)
{
    if (left >= right)
        return;

    if (left < right)
    {
        // 找到中间点
        int mid = left + (right - left) / 2;

        // 递归排序左半部分和右半部分
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // 合并已排序的两部分
        merge(arr, left, mid, right);
    }
}

// 打印数组
void printArray(const std::vector<int> &arr)
{
    for (int num : arr)
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

int main()
{
    std::vector<int> arr = {12, 11, 13, 5, 6, 7};

    std::cout << "原始数组: ";
    printArray(arr);

    mergeSort(arr, 0, arr.size() - 1);

    std::cout << "排序后数组: ";
    printArray(arr);

    return 0;
}
