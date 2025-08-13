# Sequential基本使用方法


## 类结构
```python
class Sequential(Module):           # 继承Module
    def __init__(self, *args):       # 重写了构造函数
    def _get_item_by_idx(self, iterator, idx):
    def __getitem__(self, idx):
    def __setitem__(self, idx, module):
    def __delitem__(self, idx):
    def __len__(self):
    def __dir__(self):
    def forward(self, input):  # 重写关键方法forward
```

```python
第一种用法：  
import torch.nn as nn
model = nn.Sequential(
                  nn.Conv2d(1,20,5),
                  nn.ReLU(),
                  nn.Conv2d(20,64,5),
                  nn.ReLU()
                )
第二种用法：  
from collections import OrderedDict
model = nn.Sequential(OrderedDict([
                  ('conv1', nn.Conv2d(1,20,5)),
                  ('relu1', nn.ReLU()),
                  ('conv2', nn.Conv2d(20,64,5)),
                  ('relu2', nn.ReLU())
                ]))
第三种用法：
model = nn.Sequential()
model.add_module("conv1",nn.Conv2d(1,20,5))
model.add_module('relu1', nn.ReLU())
model.add_module('conv2', nn.Conv2d(20,64,5))
model.add_module('relu2', nn.ReLU())

例子1：  
class MyNet(nn.Module):
    def __init__(self):
        super(MyNet, self).__init__()
        self.conv_block = nn.Sequential(
            nn.Conv2d(3, 32, 3, 1, 1),
            nn.ReLU(),
            nn.MaxPool2d(2))
        self.dense_block = nn.Sequential(
            nn.Linear(32 * 3 * 3, 128),
            nn.ReLU(),
            nn.Linear(128, 10)
        )

        
例子2：      
from collections import OrderedDict
class MyNet(nn.Module):
    def __init__(self):
        super(MyNet, self).__init__()
        self.conv_block = nn.Sequential(
            OrderedDict(
                [
                    ("conv1", nn.Conv2d(3, 32, 3, 1, 1)),
                    ("relu1", nn.ReLU()),
                    ("pool", nn.MaxPool2d(2))
                ]
            ))
 
        self.dense_block = nn.Sequential(
            OrderedDict([
                ("dense1", nn.Linear(32 * 3 * 3, 128)),
                ("relu2", nn.ReLU()),
                ("dense2", nn.Linear(128, 10))
            ])
        )
 
    def forward(self, x):
        conv_out = self.conv_block(x)
        res = conv_out.view(conv_out.size(0), -1)
        out = self.dense_block(res)
        return out
    
例子3：  
class MyNet(nn.Module):
    def __init__(self):
        super(MyNet, self).__init__()
        self.conv_block=torch.nn.Sequential()
        self.conv_block.add_module("conv1",torch.nn.Conv2d(3, 32, 3, 1, 1))
        self.conv_block.add_module("relu1",torch.nn.ReLU())
        self.conv_block.add_module("pool1",torch.nn.MaxPool2d(2))
 
        self.dense_block = torch.nn.Sequential()
        self.dense_block.add_module("dense1",torch.nn.Linear(32 * 3 * 3, 128))
        self.dense_block.add_module("relu2",torch.nn.ReLU())
        self.dense_block.add_module("dense2",torch.nn.Linear(128, 10))
 
    def forward(self, x):
        conv_out = self.conv_block(x)
        res = conv_out.view(conv_out.size(0), -1)
        out = self.dense_block(res)
        return out
 
model = MyNet()

```
