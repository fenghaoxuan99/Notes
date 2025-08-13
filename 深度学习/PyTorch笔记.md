随机初始化矩阵 我们可以通过torch.rand()的方法，构造一个随机初始化的矩阵：
```py
import torch

x = torch.rand(4, 3) 


x = torch.zeros(4, 3, dtype=torch.long)
我们还可以通过torch.zero_()和torch.zeros_like()将现有矩阵转换为全0矩阵.
torch.zeros_like(x, dtype=torch.float)
x.new_ones:返回一个与size大小相同的用1填充的张量
x = torch.rand_like(x,dtype=torch.float) 返回与输入相同大小的张量，该张量由区间[0,1)上均匀的随机数填充。


print(x.size())
print(x.shape)
torch.Size其实是一个tuple，⽀持所有tuple的操作。我们可以使用索引操作取得张量的长、宽等数据维度。
```


arange(s,e,step)    从s到e，步长为step
linspace(s,e,steps) 从s到e，均匀分成step份
rand/randn(sizes)   rand是[0,1)均匀分布；randn是服从N(0，1)的正态分布
normal(mean,std)    正态分布(均值为mean，标准差是std)
randperm(m)         随机排列


## 加法操作

```py
import torch
# 方式1
y = torch.rand(4, 3) 
print(x + y)

# 方式2
print(torch.add(x, y))

# 方式3 in-place，原值修改
y.add_(x) 
print(y)
```

## Tensor的索引操作：

索引出来的结果与原数据共享内存，也即修改一个，另一个也会跟着修改。
最简单的是直接传入标量，分别取对应维度的对应数据即可。需要主要的是:表示取该维度全数据，x:y表示取该维度上[x,y)之间的数据。
```py
a = paddle.arange(10).reshape((2,5))
print(a)
# Tensor Output: 
# [[0, 1, 2, 3, 4],
#  [5, 6, 7, 8, 9]]

# 基本切片
b = a[0, 1:4]  # 选取第一行,第二个到第四个元素(不包括第四个)
print(b)
# Tensor Output: [1, 2, 3]

c = a[:, ::2]  # 选取所有行,步长为2
print(c)
# Tensor Output:
# [[0, 2, 4],
#  [5, 7, 9]] 

# 使用负索引
d = a[:, ::-1] # 反向选取每一行
print(d)
# Tensor Output:
# [[4, 3, 2, 1, 0], 
#  [9, 8, 7, 6, 5]]


# 选取全部元素
f = a[:, :]
print(f)
# Tensor Output:
# [[0, 1, 2, 3, 4],
#  [5, 6, 7, 8, 9]]

```

## 二维数组的花式索引
花式索引获得指定下标的值
（0,0）（2,2）（3,3）
arr2[(0,2,3),(0,2,3)]
x,y一一对应
也可以使用arr2[[0,2,3],[0,2,3]]效果是一样的 
```py
arr2 = np.array([[1,2,3,4,5],
                [4,5,6,7,8],
                [7,8,9,10,11],
                [10,11,12,13,14]])
# 得到的是(0,0),(2,2),(3,3)              
print(arr2[(0,2,3),(0,2,3)])
#索引第2，3行中的第0，2，3列（不连续的列使用元组或列表表示）
print(arr2[2:4,(0,2,3)])
```
## 花式索引
arr2[[[0],[2],[3]],(0,2,3)]        # [[0],[2],[3]]与（0,2,3）维度不同。每一个都会与（0,2,3）内所有元素相匹配一次
arr2[[[0],[2],[3]],[[0],[2],[3]]]  # 维度相同，对应匹配
arr2[[0,2,3],2]                    # [0,2,3]都会与2匹配一次
arr2[(0,2,3),[[0],[2],[3]]]
arr2[([0],[2],[3]),(0,2,3)]
arr2[(0,2,3),([0],[2],[3])]

arr3=arr2[:,[0,2,3]] # 先获得列
arr3[[0,2,3]] # 再从中选择行 （反过来也行



## torch.view() 
函数返回的新tensor与源tensor共享内存(同一个tensor)，更改其中的一个，另外一个也会跟着改变。
顾名思义，view()仅仅是改变了对这个张量的观察角度。  
## torch.reshape()
为为了使创建的张量和原始张量不共享内存，我们需要使用第二种方法torch.reshape()， 同样可以改变张量的形状，但是**此函数并不能保证返回的是其拷贝值**，所以官方不推荐使用。推荐的方法是我们先用 clone() 创造一个张量副本然后再使用 torch.view()进行函数维度变换 。

使用 clone() 还有一个好处是会被记录在计算图中，即梯度回传到副本时也会传到源Tensor 。 
取值操作如果我们有一个元素tensor我们可以使用.item()来获得这个 alue，而不获得其他性质：