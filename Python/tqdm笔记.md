
# tqdm使用笔记

## tqdm
Tqdm 是 Python 进度条库，可以在 Python 长循环中添加一个进度提示信息。用户只需要封装任意的迭代器，是一个快速、扩展性强的进度条工具库。
### 使用实例1：
```py
import time
from tqdm import *
for i in tqdm(range(1000)):
    time.sleep(.01)   #进度条每0.01s前进一次，总时间为1000*0.01=10s 
# 运行结果如下
100%|██████████| 1000/1000 [00:10<00:00, 93.21it/s]  
```

### 使用实例2：
```python
from tqdm import tqdm

dataset_num = 100000
batch_size = 8
epoch = 1
d_loss = 1.2345
g_loss = 1.2345

pbar = tqdm(range(int(dataset_num / batch_size)))

for i in pbar:
    epoch += 1
    d_loss += 1.2345
    g_loss += 1.2345
    pbar.set_description('Epoch: %i' % epoch)
    pbar.set_postfix(d_loss=format(d_loss,'.3f'), g_loss=format(g_loss,'.3f'))
    pbar.set_postfix({'loss': '{0:1.5f}'.format(i/100), "x": '{0:1.5f}'.format(i)})  # 输入一个字典，显示实验指标
```

### 手动控制更新：
```py
import time
from tqdm import tqdm

with tqdm(total=200) as pbar:
    for i in range(20):
        pbar.update(10)
        time.sleep(.1)
# 结果如下，一共更新了20次
  0%|          | 0/200 [00:00<?, ?it/s]
 10%|█         | 20/200 [00:00<00:00, 199.48it/s]
 15%|█▌        | 30/200 [00:00<00:01, 150.95it/s]

```
### tqdm的write方法
```py
bar = trange(10)
for i in bar:
    time.sleep(0.1)
    if not (i % 3):
        tqdm.write("Done task %i" % i)
# 结果如下：
Done task 0
  0%|                                           | 0/10 [00:10<?, ?it/s]
  0%|                                           | 0/10 [00:00<?, ?it/s]
 10%|████████▎                                 | 1/10 [00:00<00:01,  8.77it/s]
 20%|████████████████▌                                                                                    
```

### 通过set_description和set_postfix方法设置进度条显示信息：
```py
from tqdm import trange
from random import random,randint
import time
 
with trange(10) as t:
  for i in t:
    #设置进度条左边显示的信息
    t.set_description("GEN %i"%i)
    #设置进度条右边显示的信息
    t.set_postfix(loss=random(),gen=randint(1,999),str="h",lst=[1,2])
    time.sleep(0.1)
```