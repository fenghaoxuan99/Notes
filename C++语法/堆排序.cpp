#include <iostream>
#include <vector>
#include <functional>
using namespace std;

// 实现堆排序
void HeapSort(vector<int> &arr)
{
    // 构建大顶堆
    for (int i = arr.size() / 2 - 1; i >= 0; i--)
    {
        int j = i;
        while (j * 2 + 1 < arr.size())
        {
            int k = j * 2 + 1; // 左子节点
            if (k + 1 < arr.size() && arr[k] < arr[k + 1])
            {
                k++; // 右子节点更大
            }
            if (arr[j] < arr[k])
            {
                swap(arr[j], arr[k]);
                j = k; // 下沉
            }
            else
            {
                break;
            }
        }
    }
    // 排序
    for (int i = arr.size() - 1; i > 0; i--)
    {
        swap(arr[0], arr[i]); // 交换堆顶和最后一个元素
        int j = 0;
        while (j * 2 + 1 < i)
        {
            int k = j * 2 + 1; // 左子节点
            if (k + 1 < i && arr[k] < arr[k + 1])
            {
                k++; // 右子节点更大
            }
            if (arr[j] < arr[k])
            {
                swap(arr[j], arr[k]);
                j = k; // 下沉
            }
            else
            {
                break;
            }
        }
    }
}
int main()
{
    vector<int> arr = {3, 5, 1, 10, 2, 7};
    HeapSort(arr);
    for (int i = 0; i < arr.size(); i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}
