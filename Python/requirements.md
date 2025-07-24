# 生成 requirements.txt 文件

requirements.txt 是一个文本文件，列出了所有必须安装的库，以便于在新环境中复制相同的设置。它通常包括库的名称和版本号，格式如下：

## 方法一：使用 pip freeze

最简单的生成方法是使用 pip freeze 命令。这个命令会列出当前环境中所有已安装的包及其版本号。

在命令行中运行： pip freeze > requirements.txt
这将会创建一个包含所有依赖项的 requirements.txt 文件。

## 方法二：使用 pipreqs

pipreqs 是一个更智能的工具，它根据项目文件中的实际导入来生成依赖列表。

首先安装 pipreqs：

pip install pipreqs

然后在项目根目录下运行：

pipreqs . --encoding=utf8

或者
pipreqs ./ --encoding utf-8

pipreqs 会在项目文件夹中创建一个 requirements.txt 文件，只包含项目实际使用的依赖。

## pip freeze 特点

pip freeze 生成的 requirements.txt 包含了当前环境中所有已安装的库及其版本号。这种方法的主要特点是：

简单直接：只需一个命令，即可列出所有安装的包。
全面性：生成的文件包括所有包，无论它们是否直接用于项目。
适用场景：最适合小型项目或在虚拟环境中工作时使用。

### 优点：

快速且易于操作。
确保了环境一致性，因为它列出了所有包。

### 缺点：

可能包含不必要的依赖，导致项目膨胀。
如果不是在虚拟环境中运行，可能会包含全局安装的包，与项目无关。

## pipreqs 特点

与 pip freeze 不同，pipreqs 会分析项目文件，仅列出项目实际使用的依赖。它的特点包括：

智能分析：基于项目代码分析依赖项。
轻量结果：仅包含项目需要的依赖，避免不必要的包。
适用场景：适合较大的项目或需要精确依赖管理的情况。

### 优点

生成的 requirements.txt 更准确，不包含冗余依赖。
减少了项目大小，提高了安装速度。

### 缺点

分析过程可能需要更多时间。
某些情况下可能无法检测到间接依赖。

## 使用 requirements.txt？

在新环境部署项目时，您可以使用 requirements.txt 文件轻松安装所有依赖项。只需运行：

pip install -r requirements.txt
