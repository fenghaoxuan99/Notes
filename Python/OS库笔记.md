<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [OS 库的使用](#os库的使用)
  - [os.walk 使用](#oswalk使用)
  - [os.access()使用](#osaccess使用)
    - [下面的值可以作为 access()的模式参数传递来测试以下内容:](#下面的值可以作为access的模式参数传递来测试以下内容)
    - [返回:](#返回)
  - [os.chflags()使用](#oschflags使用)
    - [参数：](#参数)
  - [os.chmod()](#oschmod)
    - [参数:](#参数-1)
    - [Mode:](#mode)
  - [os.chroot()](#oschroot)
  - [os.getlogin()使用](#osgetlogin使用)
  - [os.getenv()使用](#osgetenv使用)
  - [os.getpid()](#osgetpid)
  - [os.strerror()](#osstrerror)
  - [os.path.splitext()](#ospathsplitext)
  - [os.umask()](#osumask)
    - [查看当前的 umask 值：](#查看当前的umask值)
  - [os.linesep()](#oslinesep)
  - [os.pathsep()](#ospathsep)
  - [os.path.lexists()](#ospathlexists)
  - [os.cpu_count](#oscpu_count)
  - [os.system()](#ossystem)
    - [注意事项：](#注意事项)

<!-- /code_chunk_output -->

# OS 库的使用

1.  os.getcwd() 获取当前工作目录，即当前 python 脚本工作的目录路径
2.  os.chdir("dirname") 改变当前脚本工作目录；相当于 shell 下 cd
3.  os.curdir 返回当前目录: ('.')
4.  os.pardir 获取当前目录的父目录字符串名：('..')
5.  os.makedirs('dirname1/dirname2')可生成多层递归目录
6.  os.removedirs('dirname1') 若目录为空，则删除，并递归到上一级目录，如若也为空，则删除，依此类推
7.  os.mkdir('dirname') 生成单级目录；相当于 shell 中 mkdir dirname
8.  os.rmdir('dirname') 删除单级空目录，若目录不为空则无法删除，报错；相当于 shell 中 rmdir dirname
9.  os.listdir('dirname') 列出指定目录下的所有文件和子目录，包括隐藏文件，并以列表方式打印
10. os.remove() 删除一个文件
11. os.rename("oldname","newname") 重命名文件/目录
12. os.stat('path/filename') 获取文件/目录信息
13. os.sep 输出操作系统特定的路径分隔符，win 下为"\\",Linux 下为"/"
14. os.linesep 输出当前平台使用的行终止符，win 下为"\t\n",Linux 下为"\n"
15. os.pathsep 输出用于分割文件路径的字符串
16. os.name 输出字符串指示当前使用平台。win->'nt'; Linux->'posix'
17. os.system("bash command") 运行 shell 命令，直接显示
18. os.environ 获取系统环境变量
19. os.path.abspath(path) 返回 path 规范化的绝对路径
20. os.path.split(path) 将 path 分割成目录和文件名二元组返回
21. os.path.dirname(path) 返回 path 的目录。其实就是 os.path.split(path)的第一个元素
22. os.path.basename(path) 返回 path 最后的文件名。如何 path 以／或\结尾，那么就会返回空值。即 os.path.split(path)的第二个元素
23. os.path.exists(path) 如果 path 存在，返回 True；如果 path 不存在，返回 False
24. os.path.isabs(path) 如果 path 是绝对路径，返回 True
25. os.path.isfile(path) 如果 path 是一个存在的文件，返回 True。否则返回 False
26. os.path.isdir(path) 如果 path 是一个存在的目录，则返回 True。否则返回 False
27. os.path.join(path1[, path2[, ...]]) 将多个路径组合后返回，第一个绝对路径之前的参数将被忽略
28. os.path.getatime(path) 返回 path 所指向的文件或者目录的最后存取时间
29. os.path.getmtime(path) 返回 path 所指向的文件或者目录的最后修改时间

### os.walk 使用

OS.walk()通过自顶向下或自底向上遍历目录树生成文件名。对于根在目录 top 的树中的每个目录(包括 top 本身)，它会生成一个 3 元组(dirpath, dirnames, filename)。
os.walk 的返回值是一个生成器(generator),也就是说我们需要用循环不断的遍历它（不可以直接 print），来获得所有的内容。

**每次遍历的对象都是返回的是一个三元元组(root,dirs,files)**

root 所指的是当前正在遍历的这个文件夹的本身的地址
dirs 是一个 list ， 内容是该文件夹中所有的目录的名字(不包括子目录)
files 同样是 list , 内容是该文件夹中所有的文件(不包括子目录)

```python
# Driver function
import os
if __name__ == "__main__":
    for (root,dirs,files) in os.walk(r'C:\Users\hp\Desktop\apimemos', topdown=True):
        print (root)
        print (dirs)
        print (files)

C:\Users\hp\Desktop\apimemos
[]
['apidemos.com.txt', 'target.txt'] 
```

### os.access()使用

Python 中的 os.access()方法使用真实的 uid/gid 来**测试对 path 的访问权限**。大多数操作使用有效的 uid/gid，因此，可以在 suid/sgid 环境中使用此例程来测试调用用户是否具有指定的路径访问权。
使用格式：os.access(path, mode)

#### 下面的值可以作为 access()的模式参数传递来测试以下内容:

- os.F_OK:测试路径是否存在。
- os.R_OK:测试路径的可读性。
- os.W_OK:测试路径可写性。
- os.X_OK:检查路径是否可以执行。

#### 返回:

如果允许访问返回 True，否则返回 False。

```python
import os
import sys
 
# Checking access with os.F_OK
path1 = os.access("gfg.txt", os.F_OK)
print("Exists the path:", path1)
 
# Checking access with os.R_OK
path2 = os.access("gfg.txt", os.R_OK)
print("Access to read the file:", path2)
 
# Checking access with os.W_OK
path3 = os.access("gfg.txt", os.W_OK)
print("Access to write the file:", path3)
 
# Checking access with os.X_OK
path4 = os.access("gfg.txt", os.X_OK)
print("Check if path can be executed:", path4)
```

## os.chflags()使用

**格式：os.chflag(path, flags)**

#### 参数：

path:待修改目录的完整路径。
flag:采用下列标志值的组合(按位 OR)-
os.UF_NODUMP -不要转储文件。
os.UF_IMMUTABLE -文件不能被修改(只读)。
os.UF_APPEND -文件只能被追加到。
os.UF_OPAQUE -目录是不透明的，通过联合堆栈查看。
os.UF_NOUNLINK -文件不能被重命名或删除。
os.UF_COMPRESSED -文件被压缩保存
os.UF_HIDDEN -文件不应该显示在 GUI 中
os.SF_ARCHIVED -文件可能被存档。(可设置为超级用户)
os.SF_IMMUTABLE -文件不能被修改。(可设置为超级用户)
os.SF_APPEND -文件只能被追加到。(可设置为超级用户)
os.SF_NOUNLINK -文件不能重命名或删除。(可设置为超级用户)
os.SF_SNAPSHOT -文件是一个快照文件。(可设置为超级用户)
返回:不返回任何值

## os.chmod()

Python 中的 os.chmod()方法用于更改文件或目录的权限。  
**格式：os.chmod(path, mode)**

#### 参数:

Path -文件或目录路径的路径名称

#### Mode:

stat.S_ISUID:设置执行时的用户 ID
stat.S_ISGID:在执行时设置组 ID
stat.S_ENFMT:执行记录锁定
stat.S_ISVTX:执行后保存文本图像
stat.S_IREAD:由所有者阅读。
stat.S_IWRITE:由所有者写入。
stat.S_IEXEC:由所有者执行。
stat.S_IRWXU:由所有者进行读、写和执行
stat.S_IRUSR:由所有者阅读
stat.S_IWUSR:所有者写入。
stat.S_IXUSR:由所有者执行。
stat.S_IRWXG:按组读、写和执行
stat.S_IRGRP:按组读
stat.S_IWGRP:按组写
stat.S_IXGRP:按组执行
stat.S_IRWXO:由其他人读取、写入和执行。
stat.S_IROTH:其他人读
stat.S_IWOTH:别人写
stat.S_IXOTH:被其他人执行

## os.chroot()

Python 中的 os.chroot()方法用于将当前进程的根目录更改为 path。

```python
import os, sys
os.chroot("/Geeks/gfg")
print ("root path successfully changed.")


# Function to Change root directory of the process.
def change_root_directory(path):
    try:
        os.chdir(path)
        os.chroot(path)
    except Exception as exc:
        error = DaemonOSEnvironmentError("Unable to change root directory ({exc})".format(exc = exc))
        raise error
# main function
change_root_directory("/Geeks/gfg")

```

## os.getlogin()使用

Python 中的 os.getlogin()方法用于获取在进程的控制终端上登录的用户的名称。

语法:os.getlogin()

参数:不需要

返回类型:该方法返回一个字符串，表示在进程的控制终端上登录的用户的名称。

```python
import os
user = os.getlogin()
print(user) 
```

## os.getenv()使用

Python 中的 os.getenv()方法返回环境变量键的值(如果它存在，则返回默认值)。
**语法:os.getenv(key, default = None)**

参数:
key:表示环境变量名的字符串
Default(可选):字符串，表示 key 不存在时的默认值。如果省略，默认设置为“None”。
返回类型:该方法返回一个字符串，表示环境变量 key 的值。如果 key 不存在，则返回默认参数的值。

```python
import os
key = 'HOME'
value = os.getenv(key)
print("Value of 'HOME' environment variable :", value) 
key = 'JAVA_HOME'
value = os.getenv(key)
print("Value of 'JAVA_HOME' environment variable :", value) 
```

## os.getpid()

Python 中的 os.getpid()方法用于获取当前进程的进程 ID。
语法:os.getpid()
参数:不需要
返回类型:该方法返回一个整数值，表示当前进程的进程 ID。这个方法的返回类型是’ int ‘类。

```python
import os
pid = os.getpid()
print(pid) 
```

## os.strerror()

os 模块中的所有函数在文件名和路径无效或不可访问，或其他具有正确类型但操作系统不接受的参数时都会引发 OSError。
**Python 中的 os.strerror()方法用于获取与错误代码对应的错误消息。**
语法:os.strerror(code)
参数:
code:表示错误码的整数值
返回类型:该方法返回一个字符串，表示与指定错误码对应的错误消息。

```python
import os
code = 1
error = os.strerror(code)
print("Error message corresponding to error code % d:" % code, error)

code = 5
error = os.strerror(code)
print("Error message corresponding to error code % d:" % code, error)
```

## os.path.splitext()

Python 中的 os.path.splitext()方法用于分割路径，返回路径名和文件扩展名的元组。在这里，ext 代表扩展，并具有指定路径的扩展部分，而 root 是除 ext 部分以外的所有内容。
**如果指定的路径没有任何扩展名，则 Ext 为空。如果指定的路径有前导句号(‘ . ‘)，它将被忽略。**
**语法:os.path.splitext(path)**
参数:
path:表示文件系统路径的类路径对象。类路径对象是表示路径的 str 或 bytes 对象。
返回类型:该方法返回一个元组，表示指定路径名的根和 ext 部分。

```python
import os
path = '/home/User/Desktop/file.txt'
root_ext = os.path.splitext(path)

print("root part of '% s':" % path, root_ext[0])
print("ext part of '% s':"  % path, root_ext[1], "\n")
  
输出：
root part of '/home/User/Desktop/file.txt': /home/User/Desktop/file
ext part of  '/home/User/Desktop/file.txt': .txt
```

## os.umask()

Python 中的 os.umask()方法用于设置当前数值 umask 值并获取以前的 umask 值。

**Umask 代表用户文件创建模式掩码。用于确定新创建的文件或目录的文件权限。**

语法:os.umask(mask)
参数:
mask:整数，表示有效的 umask 值。
返回类型:这个方法设置当前的 umask 值，并返回一个代表前一个 umask 值的整数值。

默认情况下的 umask 值是 022，此时建立的文件默认权限是 644(6-0,6-2,6-2)，
建立的目录的默认 权限是 755(7-0,7-2,7-2)，它是为了控制默认权限，不要使默认的文件和目录具有全权而设的

**umask 　 024 则以后建立的文件和目录的默认权限就为 642,753 了**
Linux 中 umask 的工作方式与 chmod 命令类似，它也用于定义文件或目录的权限。它们之间的区别在于 chmod 用于改变已有文件或目录的权限，而**umask 用于定义新建文件或目录的默认权限**。

每次新建一个文件时，文件的默认权限是由 umask 的值决定的。当我们输入 umask 命令时，它会输出一个 4 为的八进制数值，如 0002。如果 umask 值的某位被设置，**在新建文件或目录时将禁用对应的权限。**
os.umask() 只影响当前进程及其子进程的文件和目录创建权限。
**改变 umask 会影响所有随后创建的文件和目录，因此在改变 umask 之前应该谨慎考虑。**
改变 umask 的值应该在程序的早期阶段完成，以确保所有文件和目录都使用新的掩码。

```py
import os
mask = 0o777
umask = os.umask(mask) 
print("Current umask:", mask)
print("Previous umask:", umask) 
```

### 查看当前的 umask 值：

```py
import os
current_umask = os.umask(0)
print("Current umask:", current_umask)
```

## os.linesep()

描述：当前平台用于分隔（或终止）行的字符串。它可以是单个字符，如 POSIX 上是 '\n'，也可以是多个字符，如 Windows 上是 '\r\n'。在写入以文本模式（默认模式）打开的文件时，请不要使用 os.linesep 作为行终止符，请在所有平台上都使用一个 '\n' 代替。

## os.pathsep()

描述：操作系统通常用于分隔搜索路径（如 PATH）中不同部分的字符，如 POSIX 上是 ':'，Windows 上是 ';'。在 os.path 中也可用。
语法：os.pathsep

在 Python 中，`os`模块提供了许多与操作系统交互的功能，其中包括路径分隔符和路径分隔符的设置。`os.sep`和`os.pathsep`是两个常用的属性，它们分别有不同的含义和应用场景：

1. **`os.sep`**：

   - 含义：`os.sep`代表文件系统中用于分隔目录的字符。在 Unix 系统（包括 Linux 和 macOS）中，`os.sep`是`'/'`；在 Windows 系统中，它是`'\\'`。
   - 应用：当你需要构建或解析文件路径时，可以使用`os.sep`来确保路径在不同操作系统中都是正确的。例如，你可以使用`os.sep`来连接目录和文件名，以避免硬编码斜杠或反斜杠。

2. **`os.pathsep`**：
   - 含义：`os.pathsep`是一个字符串，用于分隔文件路径列表。在所有系统中，`os.pathsep`都是`':'`，在 Windows 系统中，它是一个分号`';'`。
   - 应用：当你需要处理环境变量，如`PATH`，这些环境变量通常包含多个路径，并且这些路径由系统特定的分隔符分隔。使用`os.pathsep`可以确保你正确地分割或合并这些路径列表。

**区别**：

- `os.sep`用于路径的目录分隔，而`os.pathsep`用于路径列表的分隔。
- 在 Unix 系统中，两者都是`':'`，但在 Windows 系统中，`os.sep`是`'\\'`，而`os.pathsep`是`';'`。

**示例**：

```python
import os

# 使用os.sep来构建跨平台的路径
path = os.sep.join(['home', 'user', 'file.txt'])

# 使用os.pathsep来分割环境变量
path_list = os.environ['PATH'].split(os.pathsep)
```

在编写跨平台的 Python 代码时，使用`os.sep`和`os.pathsep`可以提高代码的可移植性和健壮性。

## os.path.lexists()

描述：路径存在则返回 True，路径损坏也返回 True， 不存在，返回 False。

## os.cpu_count()

描述：返回当前系统的 CPU 核心数

## os.system()

在一个子终端运行系统命令，而不能获取命令执行后的返回信息。
os.system(cmd)的返回值。如果执行成功，那么会返回 0，表示命令执行成功。否则，则是执行错误。

### 注意事项：

1. 安全性：由于 os.system() 会执行传入的任何命令，如果命令字符串包含用户输入，可能会受到注入攻击。
2. 环境依赖：执行的命令依赖于系统的 shell 环境，不同操作系统或不同用户配置可能有不同的行为。
3. 错误处理：返回的退出状态码可以用来判断命令是否成功执行，但通常需要额外的错误处理逻辑。
