# reduce 函数

reduce 函数位于 functools 模块中，它接收一个函数（这个函数必须接受两个参数）和一个可迭代对象。它会连续地将函数应用于序列的元素，从而将序列缩减为单个值。具体来说，reduce 会首先将序列的前两个元素传递给函数，并运行它们；然后，它会将该函数的返回值与下一个元素一起再次传递给函数，如此持续，直到序列中没有更多元素为止。

```py
from functools import reduce

# 定义相加的函数
def add(x, y):
    return x + y

# 数字列表
numbers = [1, 2, 3, 4, 5]

# 使用reduce进行累加
result = reduce(add, numbers)

#result的值将会是15，因为reduce累加了列表中的所有数字（1+2+3+4+5）。
```
