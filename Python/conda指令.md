<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [conda 指令](#conda指令)
  - [创建虚拟环境](#创建虚拟环境)
  - [激活/使用/进入某个虚拟环境](#激活使用进入某个虚拟环境)
  - [退出当前环境](#退出当前环境)
  - [复制某个虚拟环境](#复制某个虚拟环境)
  - [删除某个环境](#删除某个环境)
  - [查看当前所有环境](#查看当前所有环境)
  - [查看当前虚拟环境下的所有安装包](#查看当前虚拟环境下的所有安装包)
  - [安装或卸载包(进入虚拟环境之后)](#安装或卸载包进入虚拟环境之后)
  - [分享虚拟环境](#分享虚拟环境)
  - [清理 anaconda 缓存](#清理anaconda缓存)
  - [源服务器管理](#源服务器管理)
    - [国内 pip 源](#国内pip源)
  - [升级](#升级)
  - [卸载](#卸载)
  - [批量导出虚拟环境中的所有组件](#批量导出虚拟环境中的所有组件)
  - [pip 批量导出环境中的所有组件](#pip批量导出环境中的所有组件)

<!-- /code_chunk_output -->

# conda 指令

## 创建虚拟环境

- conda create --name env_name
- conda create --name env_name python=3.6 # 创建指定 python 版本
- conda create --name env_name python=3.6 pandas numpy scipy # 创建指定 python 版本下包含某些包

## 激活/使用/进入某个虚拟环境

conda activate env_name

## 退出当前环境

conda deactivate env_name

## 复制某个虚拟环境

conda create --name new_env_name --clone old_env_name

## 删除某个环境

conda remove --name env_name --all

## 查看当前所有环境

conda info --envs 或者 conda env list

## 查看当前虚拟环境下的所有安装包

conda list 需进入该虚拟环境
conda list -n env_name

查看虚拟环境的包
conda list -n env_name

查看虚拟环境的包的详细信息
conda list -n env_name -v

## 安装或卸载包(进入虚拟环境之后)

conda install   requests
conda install   xx = 版本号 # 指定版本号
conda install   xxx -i 源名称或链接 # 指定下载源
conda uninstall xxx

## 分享虚拟环境

conda env export > environment.yml # 导出当前虚拟环境
conda env create -f environment.yml # 创建保存的虚拟环境

## 清理 anaconda 缓存

conda clean -p # 删除没有用的包 --packages
conda clean -t # 删除 tar 打包 --tarballs
conda clean -y -all # 删除所有的安装包及 cache(索引缓存、锁定文件、未使用过的包和 tar 包)

## 源服务器管理

conda 当前的源设置在$HOME/.condarc 中，可通过文本查看器查看或者使用命令>conda config --show-sources 查看。

conda config --show-sources 查看当前使用源
conda config --remove channels 源名称或链接 #删除指定源
conda config --add channels 源名称或链接 #添加指定源
例如：

conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/conda-forge/

### 国内 pip 源

|       名称       |                   地址                    |
| :--------------: | :---------------------------------------: |
|      阿里云      |  http://mirrors.aliyun.com/pypi/simple/   |
|   中国科技大学    | https://pypi.mirrors.ustc.edu.cn/simple/  |
|   豆瓣(douban)   |      http://pypi.douban.com/simple/       |
|     清华大学     | https://pypi.tuna.tsinghua.edu.cn/simple/ |
| 中国科学技术大学   |  http://pypi.mirrors.ustc.edu.cn/simple/  |

## 升级

升级 Anaconda 需先升级 conda

conda update conda
conda update anaconda

## 卸载

rm -rf anaconda

## 批量导出虚拟环境中的所有组件

conda list -e > requirements.txt # 导出
conda install --yes --file requirements.txt # 安装

## pip 批量导出环境中的所有组件

pip freeze > requirements.txt
pip install -r requirements.txt
