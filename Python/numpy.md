
# numpy



## 创建数组
```py
import numpy as np
#shape代表形状，比如我这里创建的就是5行三列的2维数组
data = np.ones(shape=(5,3))

data = np.arange(10,16,2) # 10-16的数据，步长为2

data = np.linspace(1,10,20) # 开始端1，结束端10，且分割成20个数据，生成线段

data = np.random.rand(3,4)

data = np.random.randint(2,5,size=(4,5));

data = np.array([1,2,3])

print(data.ndim)维度
```

## 计算数组的标准差
numpy.std(arr, axis=None, dtype=None, out=None): 计算数组的标准差。参数axis、dtype和out的含义与numpy.mean()相同。

## 计算数组的方差
numpy.var(arr, axis=None, dtype=None, out=None): 计算数组的方差。参数axis、dtype和out的含义与numpy.mean()相同。

## 计算数组的最小值 
numpy.min(arr, axis=None, out=None): 计算数组的最小值。参数axis和out的含义与numpy.mean()相同。

## 计算数组的最大值
numpy.max(arr, axis=None, out=None): 计算数组的最大值。参数axis和out的含义与numpy.mean()相同

## 计算数组的元素之和
numpy.sum(arr, axis=None, dtype=None, out=None): 计算数组的元素之和。参数axis、dtype和out的含义与numpy.mean()相同。

## 计算数组的元素乘积
numpy.prod(arr, axis=None, dtype=None, out=None): 计算数组的元素乘积。参数axis、dtype和out的含义与numpy.mean()相同

## 计算数组的累积和
numpy.cumsum(arr, axis=None, dtype=None, out=None): 计算数组的累积和。参数axis、dtype 和out的含义与numpy.mean()相同。

