#include <iostream>
#include <vector>
#include <functional>
using namespace std;

// 实现基数排序
void radixSort(vector<int> &arr)
{
    int maxNum = *max_element(arr.begin(), arr.end());
    int minNum = *min_element(arr.begin(), arr.end()); // 计算最小值
    int absMaxNum = abs(maxNum);                       // 计算最大值的绝对值
    int absMinNum = abs(minNum);                       // 计算最小值的绝对值

    int exp = 1; // 当前位数的权重

    while (maxNum / exp > 0)
    {
        vector<int> output(arr.size()); // 输出数组
        vector<int> count(20, 0);       // 计数数组

        // 统计当前位数的出现次数
        for (int i = 0; i < arr.size(); i++)
        {
            count[((arr[i] / exp) % 10) + 10]++;
        }

        // 累加计数数组
        for (int i = 1; i < 20; i++)
        {
            count[i] += count[i - 1];
        }

        // 构建输出数组
        for (int i = arr.size() - 1; i >= 0; i--)
        {
            output[count[((arr[i] / exp) % 10) + 10] - 1] = arr[i];
            count[((arr[i] / exp) % 10) + 10]--;
        }

        // 将输出数组复制回原数组
        arr = output;
        exp *= 10; // 移动到下一个位数
    }
}

int main()
{
    vector<int> arr = {170, 45, 75, 90, 802, 24, 2, 66, -1, -5, -3, 0, 100, 200, -100, -2000};
    radixSort(arr);

    cout << "排序后的数组: ";
    for (int i = 0; i < arr.size(); i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}
