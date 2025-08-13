# 张量Tensor笔记


## 创建张量
```python
import torch

# 从数据创建张量
x = torch.tensor([1, 2, 3])
print(x)  # tensor([1, 2, 3])

# 从NumPy数组创建张量
import numpy as np
y = np.array([[1, 2], [3, 4]])
z = torch.from_numpy(y)
print(z)  # tensor([[1, 2],
          #        [3, 4]])

# 使用特定数据类型创建张量
a = torch.tensor([1.0, 2.0, 3.0], dtype=torch.float64)
print(a)  # tensor([1., 2., 3.], dtype=torch.float64)

# 使用形状和数据类型创建全零张量
b = torch.zeros((2, 3), dtype=torch.int64)
print(b)  # tensor([[0, 0, 0],
          #        [0, 0, 0]])

# 使用形状和数据类型创建全1张量
c = torch.ones((2, 3), dtype=torch.float32)
print(c)  # tensor([[1., 1., 1.],
          #        [1., 1., 1.]])

# 使用形状和数据类型创建随机初始化张量
d = torch.randn((2, 3))
print(d)  # 输出将是一个形状为(2, 3)的随机浮点数张量
```

## 张量的操作   
  PyTorch提供了丰富的张量操作函数，包括数学运算、索引、切片、重塑等：
```python
# 张量加法
e = x + z  # 假设x是一个一维张量，z是一个二维张量，这里会发生广播（broadcasting）
print(e)

# 张量乘法（逐元素相乘）
f = x * z[:, 0]  # 假设我们想要将x与z的第一行逐元素相乘
print(f)

# 索引和切片
g = z[0, :]  # 获取z的第一行
print(g)

# 重塑张量
h = g.reshape((1, 2))  # 将g重塑为一个形状为(1, 2)的二维张量
print(h)


```

## 使用GPU加速张量运算
 如果您的计算机上安装了NVIDIA GPU并安装了CUDA支持的PyTorch版本，则可以使用GPU来加速张量运算。这可以通过将张量移动到GPU上来实现。
```python
# 检查是否有可用的GPU
if torch.cuda.is_available():
    device = torch.device('cuda')
    # 将张量移动到GPU
    k = x.to(device)
    print(k)  # 输出将显示张量位于哪个GPU上
    
    # 在GPU上进行运算
    l = k * 2
    print(l)  # 结果也在GPU上
else:
    print('No GPU available.')
```
## 张量自动微分
 在PyTorch中，torch.tensor 支持自动微分，这对于深度学习模型的训练至关重要。
通过调用 .requires_grad_() 方法，您可以告诉PyTorch跟踪对张量的所有操作，以便稍后可以自动计算梯度。

```python
# 创建一个需要梯度的张量
m = torch.tensor([1.0, 2.0, 3.0], requires_grad=True)

# 对张量进行运算
n = m * m

# 计算梯度（需要先调用backward方法）
n.backward()
# 输出梯度
print(m.grad)  # tensor([2., 4., 6.])
```