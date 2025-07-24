

# Dataset类


```py
class Dataset(object):
    """An abstract class representing a Dataset.
    All other datasets should subclass it. All subclasses should override
    ``__len__``, that provides the size of the dataset, and ``__getitem__``,
    supporting integer indexing in range from 0 to len(self) exclusive.
    """
 
    def __getitem__(self, index):
        raise NotImplementedError
 
    def __len__(self):
        raise NotImplementedError
 
    def __add__(self, other):
        return ConcatDataset([self, other])
```
__getitem__就是获取样本对，模型直接通过这一函数获得一对样本对{x:y}。__len__是指数据集长度。


```py
import os
from PIL import Image
from torch.utils.data import Dataset
from torch.utils.data import DataLoader
import torchvision.transforms as transforms
 
 
class RMBDataset(Dataset):
    def __init__(self, data_dir, transform=None):
        """
        rmb面额分类任务的Dataset
        :param data_dir: str, 数据集所在路径
        :param transform: torch.transform，图像数据预处理
        """
        self.label_name = {"1": 0, "100": 1}
        self.data_info = self.get_img_info(data_dir)
        self.transform = transform
 
    def __getitem__(self, index):
        path_img, label = self.data_info[index]
        img = Image.open(path_img).convert('RGB')
 
        if self.transform is not None:
            img = self.transform(img)
 
        return img, label
 
    def __len__(self):
        return len(self.data_info)
 
    def get_img_info(self, data_dir):
        data_info = list()
        for root, dirs, _ in os.walk(data_dir):
            # 遍历类别
            for sub_dir in dirs:
                img_names = os.listdir(os.path.join(root, sub_dir))
                img_names = list(filter(lambda x: x.endswith('.jpg'), img_names))
 
                # 遍历图片
                for i in range(len(img_names)):
                    img_name = img_names[i]
                    path_img = os.path.join(root, sub_dir, img_name)
                    label = self.label_name[sub_dir]
                    data_info.append((path_img, int(label)))
 
        return data_info
 
 
if __name__ == '__main__':
    norm_mean = [0.485, 0.456, 0.406]
    norm_std  = [0.229, 0.224, 0.225]
    train_transform = transforms.Compose([
        transforms.Resize((32, 32)),
        transforms.RandomCrop(32, padding=4),
        transforms.ToTensor(),
        transforms.Normalize(norm_mean, norm_std),
    ])
    train_data   = RMBDataset("data/RMB_data/", train_transform)
    train_loader = DataLoader(dataset=train_data, batch_size=4, shuffle=True)
    for i, data in enumerate(train_loader):
        inputs, labels = data
```
