# C++ I/O流基础：std::ios_base详解

## 1. std::ios_base概述

std::ios_base是所有I/O流类的基类，它提供了流操作的核心功能和控制机制。这个类维护着以下几类重要数据：

1. **状态信息**：流的状态标志（如goodbit, badbit等）
2. **控制信息**：格式化标志和区域设置
3. **私有存储**：可扩展的数据结构，用于存储long和void*类型的数据
4. **回调函数**：在特定事件发生时调用的用户定义函数

## 2. 格式化控制

### 2.1 格式化标志管理

#### flags()函数
```cpp
fmtflags flags() const;            // 获取当前格式标志
fmtflags flags(fmtflags flags);    // 设置格式标志并返回之前的值
```

#### setf()和unsetf()函数
```cpp
fmtflags setf(fmtflags flags);                     // 设置指定标志
fmtflags setf(fmtflags flags, fmtflags mask);      // 使用掩码设置标志
void unsetf(fmtflags flags);                       // 清除指定标志
```

### 2.2 格式化标志类型(fmtflags)

std::ios_base定义了多种格式化标志，可分为以下几类：

#### 数值基数控制
```cpp
static constexpr fmtflags dec = /*...*/;      // 十进制
static constexpr fmtflags oct = /*...*/;      // 八进制 
static constexpr fmtflags hex = /*...*/;      // 十六进制
static constexpr fmtflags basefield = dec | oct | hex;
```

#### 对齐方式控制
```cpp
static constexpr fmtflags left = /*...*/;     // 左对齐
static constexpr fmtflags right = /*...*/;    // 右对齐
static constexpr fmtflags internal = /*...*/; // 内部对齐
static constexpr fmtflags adjustfield = left | right | internal;
```

#### 浮点数格式控制
```cpp
static constexpr fmtflags scientific = /*...*/;  // 科学计数法
static constexpr fmtflags fixed = /*...*/;       // 定点表示法
static constexpr fmtflags floatfield = scientific | fixed;
```

#### 其他格式控制
```cpp
static constexpr fmtflags boolalpha = /*...*/;  // 布尔值字母表示
static constexpr fmtflags showbase = /*...*/;   // 显示数值基数前缀
static constexpr fmtflags showpoint = /*...*/;  // 总是显示小数点
static constexpr fmtflags showpos = /*...*/;    // 显示正数的+号
static constexpr fmtflags skipws = /*...*/;     // 跳过前导空白
static constexpr fmtflags unitbuf = /*...*/;    // 每次操作后刷新缓冲区
static constexpr fmtflags uppercase = /*...*/;  // 使用大写字母
```

### 2.3 使用示例

```cpp
#include <iostream>
#include <iomanip>
#include <numbers>

int main() {
    const double PI = std::numbers::pi;
    const int WIDTH = 15;

    // 设置右对齐
    std::cout.setf(std::ios::right);
    std::cout << std::setw(WIDTH/2) << "radius"
              << std::setw(WIDTH) << "circumference" << '\n';

    // 设置定点表示法
    std::cout.setf(std::ios::fixed);
    for(double radius = 1; radius <= 6; radius += 0.5) {
        std::cout << std::setprecision(1) << std::setw(WIDTH/2) << radius
                  << std::setprecision(2) << std::setw(WIDTH) 
                  << (2 * PI * radius) << '\n';
    }
}
```

## 3. 文件操作控制

### 3.1 文件打开模式(openmode)

```cpp
typedef /*实现定义*/ openmode;
static constexpr openmode app = /*...*/;     // 追加模式
static constexpr openmode binary = /*...*/;  // 二进制模式
static constexpr openmode in = /*...*/;      // 读模式
static constexpr openmode out = /*...*/;     // 写模式
static constexpr openmode trunc = /*...*/;   // 截断模式
static constexpr openmode ate = /*...*/;     // 打开后定位到末尾
```

### 3.2 使用示例

```cpp
#include <fstream>
#include <iostream>
#include <string>

int main() {
    const char* fname = "test.txt";

    // 写入文件(截断模式)
    std::fstream(fname, std::ios::out | std::ios::trunc) << "Hello World";

    // 读取文件
    std::string s;
    std::fstream(fname, std::ios::in) >> s;
    std::cout << s << '\n';
}
```

## 4. 流状态控制

### 4.1 流状态标志(iostate)

```cpp
typedef /*实现定义*/ iostate;
static constexpr iostate goodbit = 0;    // 无错误
static constexpr iostate badbit = /*...*/;  // 不可恢复错误
static constexpr iostate failbit = /*...*/; // 操作失败
static constexpr iostate eofbit = /*...*/;  // 到达文件末尾
```

### 4.2 使用示例

```cpp
#include <iostream>
#include <fstream>

int main() {
    std::ifstream file("nonexistent.txt");
    
    if(file.fail()) {
        std::cerr << "Failed to open file\n";
        if(file.bad()) {
            std::cerr << "Critical error occurred\n";
        }
    }
}
```

## 5. 文件定位控制

### 5.1 定位方向(seekdir)

```cpp
static constexpr seekdir beg = /*...*/;  // 流的开始
static constexpr seekdir end = /*...*/;  // 流的末尾
static constexpr seekdir cur = /*...*/;  // 当前位置
```

### 5.2 使用示例

```cpp
#include <iostream>
#include <sstream>
#include <string>

int main() {
    std::istringstream in("Hello, World!");
    std::string word1, word2, word3;
    
    in >> word1;  // 读取第一个单词
    in.seekg(0, std::ios_base::beg);  // 回到开头
    in >> word2;  // 再次读取第一个单词
    
    in.seekg(1, std::ios_base::cur);  // 从当前位置向后移动1个字符
    in >> word3;  // 读取剩余部分
    
    std::cout << "word1: " << word1 << "\n"
              << "word2: " << word2 << "\n"
              << "word3: " << word3 << "\n";
}
```

## 6. 精度控制

### 6.1 precision()函数

```cpp
streamsize precision() const;            // 获取当前精度
streamsize precision(streamsize prec);   // 设置新精度并返回旧值
```

### 6.2 使用示例

```cpp
#include <iostream>

int main() {
    const double d = 12.345678901234;
    std::cout << "Default precision: " << std::cout.precision() << "\n";
    std::cout << "Default output: " << d << "\n";
    
    std::cout.precision(10);
    std::cout << "High precision: " << d << "\n";
    
    std::cout.precision(3);
    std::cout << "Low precision: " << d << "\n";
}
```

## 7. 最佳实践

1. **使用操纵符替代直接设置标志**：大多数情况下，使用`std::hex`等操纵符比直接调用`setf()`更清晰。
2. **检查流状态**：在进行I/O操作后，总是检查流状态以确保操作成功。
3. **合理设置精度**：根据实际需求设置浮点数精度，避免不必要的高精度输出。
4. **组合使用标志**：使用位或操作(`|`)组合多个标志，如`std::ios::in | std::ios::binary`。

