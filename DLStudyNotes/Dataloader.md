

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [DataLoader](#dataloader)
  - [drop_last参数](#drop_last参数)
    - [使用场景：](#使用场景)
  - [sampler参数](#sampler参数)
  - [SubsetRandomSampler采样器](#subsetrandomsampler采样器)
    - [应用场景](#应用场景)
  - [batch_sampler参数](#batch_sampler参数)
    - [特点](#特点)
    - [注意事项](#注意事项)
    - [应用场景](#应用场景-1)
  - [num_workers参数](#num_workers参数)
  - [collate_fn参数](#collate_fn参数)

<!-- /code_chunk_output -->



# DataLoader
- torch.utils.data.Dataset是代表这一数据的抽象类（也就是基类）。我们可以通过继承和重写这个抽象类实现自己的数据类，只需要定义__len__和__getitem__这个两个函数。

- DataLoader是Pytorch中用来处理模型输入数据的一个工具类。组合了数据集（dataset） + 采样器(sampler)，并在数据集上提供单线程或多线程(num_workers )的可迭代对象。在DataLoader中有多个参数，这些参数中重要的几个参数的含义说明如下：  
    1. dataset (必需): 用于加载数据的数据集，通常是torch.utils.data.Dataset的子类实例。
    2. batch_size (可选): 每个批次的数据样本数。默认值为1。
    3. shuffle (可选): 是否在每个周期开始时打乱数据。默认为False。
    4. sampler (可选): 定义从数据集中抽取样本的策略。如果指定，则忽略shuffle参数。
    5. batch_sampler (可选): 与sampler类似，但一次返回一个批次的索引。不能与batch_size、shuffle和sampler同时使用。
    6. num_workers (可选): 用于数据加载的子进程数量。默认为0，意味着数据将在主进程中加载。
    7. collate_fn (可选): 如何将多个数据样本整合成一个批次。通常不需要指定。
    8. drop_last (可选): 如果数据集大小不能被批次大小整除，是否丢弃最后一个不完整的批次。默认为False。

```py
from torch.utils.data import Dataset, DataLoader

class MyDataset(Dataset):
    def __init__(self, data):
        self.data = data
        
    def __len__(self):
        return len(self.data)
    
    def __getitem__(self, idx):
        return self.data[idx]

# 创建自定义数据集实例
my_data = [1, 2, 3, 4, 5, 6, 7]
my_dataset = MyDataset(my_data)

# 使用DataLoader加载自定义数据集my_dataset
dataloader = DataLoader(dataset=my_dataset)
```
## drop_last参数
drop_last参数决定了在数据批次划分时是否丢弃最后一个不完整的批次。当数据集的大小不能被批次大小整除时，最后一个批次的大小可能会小于指定的批次大小。drop_last参数用于控制是否保留这个不完整的批次。

### 使用场景：
当数据集大小不能被批次大小整除时，如果最后一个批次的大小较小，可能会导致模型训练时的不稳定。通过将drop_last设置为True，可以确保每个批次的大小都相同，从而避免这种情况。
在某些情况下，丢弃最后一个批次可能不会对整体训练效果产生太大影响，但可以减少计算资源的浪费。例如，当数据集非常大时，最后一个不完整的批次可能只包含很少的数据样本，对于整体训练过程的贡献较小。


## sampler参数
sampler参数定义从数据集中抽取样本的策略。如果指定了sampler，则忽略shuffle参数。它可以是任何实现了__iter__()方法的对象，通常会使用torch.utils.data.Sampler的子类。

```py
from torch.utils.data import SubsetRandomSampler

# 创建一个随机抽样器，只选择索引为偶数的样本 【索引从0开始~】
sampler    = SubsetRandomSampler(indices=[i for i in range(0, len(my_dataset), 2)])
dataloader = DataLoader(dataset=my_dataset, sampler=sampler)

for data in dataloader:
    print(data)
```

## SubsetRandomSampler采样器

SubsetRandomSampler 是 PyTorch 中的一个采样器，它允许你从一个数据集中随机选择一个子集，并在每个 epoch 中随机地从这个子集中抽取样本。这在数据集很大或者你只想在训练过程中使用数据集的一部分时非常有用。
### 应用场景
- 数据集很大：当你的数据集非常大，而你只想在训练过程中使用其中的一部分时。
- 数据不平衡：在某些情况下，你可能希望在训练过程中多次看到某些类别的样本，而较少地看到其他类别的样本。
- 内存限制：当你的硬件资源有限，无法一次性加载整个数据集时。
```py
sampler = SubsetRandomSampler(indices)

from torch.utils.data import SubsetRandomSampler, DataLoader

# 假设 my_dataset 是你的数据集
indices = [i for i in range(0, len(my_dataset), 2)]  # 获取偶数索引

# 创建 SubsetRandomSampler 实例
sampler = SubsetRandomSampler(indices)

# 使用 sampler 创建 DataLoader
dataloader = DataLoader(dataset=my_dataset, sampler=sampler)

# 迭代 DataLoader
for data in dataloader:
    # 在这里，data 将只包含偶数索引的样本
    print(data)
```

## batch_sampler参数
batch_sampler参数与sampler类似，但它返回的是一批次的索引，而不是单个样本的索引。不能与batch_size、shuffle和sampler同时使用。
BatchSampler 是 PyTorch 中的一个采样器，它允许你自定义每个批次中抽取样本的方式。这在进行深度学习训练时非常有用，尤其是当你需要从数据集中创建自定义批次时。

### 特点
- 自定义批次：你可以通过 BatchSampler 定义每个批次的样本数和行为。
- 灵活的采样器：BatchSampler 可以与任何采样器（如 SequentialSampler、RandomSampler、SubsetRandomSampler 等）结合使用。
### 注意事项
- BatchSampler 只影响样本的批次划分，不会影响样本的选择或顺序。
- 如果 drop_last=True，则在最后一个批次中的样本数可能小于 batch_size，这在某些情况下可能不适用。
### 应用场景
- 自定义批次大小：当你需要每批次有特定数量的样本时。
- 不同的采样策略：你可以结合不同的采样器来实现复杂的采样策略，比如先随机采样再分批。

```py
from torch.utils.data import BatchSampler
from torch.utils.data import SubsetRandomSampler

# 创建一个随机抽样器，只选择索引为偶数的样本 【索引从0开始~】
sampler = SubsetRandomSampler(indices=[i for i in range(0, len(my_dataset), 2)])

# 创建一个批量抽样器，每个批次包含2个样本
batch_sampler = BatchSampler(sampler, batch_size=2, drop_last=True)
dataloader    = DataLoader(dataset=my_dataset, batch_sampler=batch_sampler)

for data in dataloader:
    print(data)
```


## num_workers参数
num_workers参数指定用于数据加载的子进程数量。默认为0，表示数据将在主进程中加载。增加num_workers的值可以加快数据的加载速度，但也会增加内存消耗。
```py
dataloader = DataLoader(dataset=my_dataset, num_workers=4)
```
代码解释: 在这个示例中，子进程数量设置为4，这意味着将使用4个子进程并行加载数据，以加快数据加载速度。


## collate_fn参数 
collate_fn参数指定如何将多个数据样本整合成一个批次，通常不需要指定。如果需要自定义批次数据的整合方式，可以提供一个可调用的函数。该函数接受一个样本【列表】作为输入，返回一个批次的数据。
在 PyTorch 的 DataLoader 中，collate_fn 是一个可选参数，它定义了如何将一个批次（batch）中的多个数据样本（通常是一个列表）合并成一个批次的数据。默认情况下，DataLoader 会简单地将数据样本列表堆叠（stack）成一个批次的张量（tensor），但有时你可能需要更复杂的合并逻辑，这时就可以通过自定义 collate_fn 函数来实现。
```py
import torch
class MyDataset(Dataset):
    def __init__(self, data):
        self.data = data

    def __len__(self):
        return len(self.data)

    def __getitem__(self, idx):
        return self.data[idx]


# 创建自定义数据集实例
my_data = [1, 2, 3, 4, 5, 6, 7]
my_dataset = MyDataset(my_data)

def my_collate_fn(batch):
    print(type(batch))
    # 将batch中的每个样本转换为pytorch的tensor并都加上10
    return [torch.tensor(data) + 10 for data in batch]

dataloader = DataLoader(dataset=my_dataset, batch_size=2, collate_fn=my_collate_fn)

for data in dataloader:
    print(data)
```


