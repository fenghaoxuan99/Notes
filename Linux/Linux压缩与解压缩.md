# Linux 压缩与解压缩全面指南

## 一、压缩工具概述

Linux系统支持多种压缩格式，每种格式都有其特点和适用场景：

| 压缩格式 | 常用工具 | 特点 | 适用场景 |
|----------|----------|------|----------|
| .tar.gz/.tgz | tar + gzip | 高压缩比，速度较快 | 通用压缩 |
| .tar.bz2 | tar + bzip2 | 更高压缩比，速度较慢 | 需要更高压缩率 |
| .zip | zip/unzip | 跨平台兼容 | Windows/Linux共享文件 |
| .rar | rar/unrar | 专有格式，需安装 | 处理Windows压缩包 |
| .tar.xz | tar + xz | 极高压缩比，速度慢 | 需要极致压缩率 |

## 二、详细使用说明

### 1. tar 命令（最常用）

#### 1.1 压缩操作
```bash
# gzip方式压缩（.tar.gz）
tar zcvf 压缩包名.tar.gz 文件/目录1 文件/目录2...

# bzip2方式压缩（.tar.bz2）
tar jcvf 压缩包名.tar.bz2 文件/目录1 文件/目录2...

# 示例：
tar zcvf backup.tar.gz /home /etc  # 压缩home和etc目录
tar jcvf docs.tar.bz2 *.txt docs/  # 压缩所有txt文件和docs目录
```

#### 1.2 解压操作
```bash
# 解压到当前目录
tar zxvf 压缩包名.tar.gz       # gzip格式
tar jxvf 压缩包名.tar.bz2      # bzip2格式

# 解压到指定目录
tar zxvf 压缩包名.tar.gz -C 目标目录
tar jxvf 压缩包名.tar.bz2 -C 目标目录

# 示例：
tar zxvf backup.tar.gz -C /tmp  # 解压到/tmp目录
```

### 2. zip/unzip 命令

#### 2.1 安装（如未预装）
```bash
# Ubuntu/Debian
sudo apt install zip unzip

# CentOS/RHEL
sudo yum install zip unzip
```

#### 2.2 压缩与解压
```bash
# 压缩文件/目录（-r递归目录）
zip -r 压缩包名.zip 文件/目录1 文件/目录2...

# 解压到当前目录
unzip 压缩包名.zip

# 解压到指定目录
unzip 压缩包名.zip -d 目标目录

# 示例：
zip -r project.zip src/ docs/ README.md
unzip project.zip -d /opt/
```

### 3. rar/unrar 命令

#### 3.1 安装
```bash
# Ubuntu/Debian
sudo apt install rar unrar

# CentOS/RHEL
wget https://www.rarlab.com/rar/rarlinux-x64-6.0.0.tar.gz
tar zxvf rarlinux-x64-6.0.0.tar.gz
cd rar/
sudo make install
```

#### 3.2 使用示例
```bash
# 压缩（a=压缩，-r递归）
rar a -r 压缩包名.rar 文件/目录1 文件/目录2...

# 解压到当前目录
unrar x 压缩包名.rar

# 解压到指定目录
unrar x 压缩包名.rar 目标目录/

# 示例：
rar a -r backup.rar /var/www/
unrar x backup.rar /tmp/restore/
```

### 4. xz 高压缩率工具

#### 4.1 压缩与解压
```bash
# 两步压缩法（先打包再压缩）
tar cvf 打包文件.tar 要压缩的文件/目录
xz -z 打包文件.tar   # 生成打包文件.tar.xz

# 两步解压法
xz -d 压缩包.tar.xz  # 解压为.tar文件
tar xvf 压缩包.tar   # 解包

# 示例：
tar cvf data.tar /bigdata/
xz -z data.tar
xz -d data.tar.xz
tar xvf data.tar
```

## 三、实用技巧与注意事项

1. **查看压缩包内容**：
   ```bash
   tar ztvf file.tar.gz      # 查看.gz包内容
   tar jtvf file.tar.bz2     # 查看.bz2包内容
   unzip -l file.zip         # 查看.zip包内容
   unrar l file.rar          # 查看.rar包内容
   ```

2. **排除特定文件**：
   ```bash
   tar zcvf backup.tar.gz --exclude='*.tmp' --exclude='cache/*' /path/
   zip -r backup.zip folder/ -x *.log temp/*
   ```

3. **分卷压缩**：
   ```bash
   zip -r -s 100m archive.zip large_folder/  # 100MB分卷
   rar a -v100m archive.rar large_folder/    # RAR分卷
   ```

4. **性能考虑**：
   - gzip：平衡压缩率和速度（默认推荐）
   - bzip2：更高压缩率，适合可以接受更慢速度的场景
   - xz：最高压缩率，适合需要极致压缩的场景

5. **安全性**：
   ```bash
   zip -e secure.zip sensitive_file  # 加密ZIP
   rar a -p secret.rar secret_file   # 加密RAR
   ```

## 四、总结对比表

| 特性 | tar.gz | tar.bz2 | zip | rar | tar.xz |
|------|--------|---------|-----|-----|--------|
| 压缩率 | 中 | 高 | 中 | 高 | 极高 |
| 速度 | 快 | 慢 | 中 | 中 | 最慢 |
| 跨平台 | 优 | 优 | 优 | 良 | 优 |
| 目录支持 | 是 | 是 | 需-r | 需-r | 是 |
| 预装情况 | 是 | 是 | 可能需安装 | 需安装 | 是 |

