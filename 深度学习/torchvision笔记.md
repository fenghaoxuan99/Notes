# torchvision
**PyTorch框架中有一个非常重要且好用的包**：**torchvision**，该包主要由3个子包组成：
torchvision.datasets、torchvision.models、torchvision.transforms。  
torchvision.models这个包中包含alexnet、densenet、inception、resnet、squeezenet、vgg等常用的网络结构，并且提供了预训练模型，
可以通过简单调用来读取网络结构和预训练模型。


## torchvision.models实例
**预训练模型可以通过设置pretrained=True来构建：**
```python
import torchvision.models as models

resnet18   = models.resnet18(pretrained=True)
vgg16      = models.vgg16(pretrained=True)
alexnet    = models.alexnet(pretrained=True)
squeezenet = models.squeezenet1_0(pretrained=True)
```
预训练模型期望的输入是RGB图像的mini-batch：(batch_size, 3, H, W)，并且H和W不能低于224。图像的像素值必须在范围[0,1]间，并且用
均值mean=[0.485, 0.456, 0.406]和方差std=[0.229, 0.224, 0.225]进行归一化。
**如果只需要网络结构，不需要用与训练模型的参数来初始化，可以将pretrained = False**  
```python
model = torchvision.models.densenet169(pretrained=False)
# 等价于：
model = torchvision.models.densenet169()
```