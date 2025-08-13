# C++文件操作完全指南：从基础到高级应用

## 1. C语言文件操作基础

### 1.1 文件指针与fopen函数

在C语言中，文件操作通过`FILE`结构体指针实现。`fopen()`函数用于打开文件并返回一个`FILE`指针：

```c
FILE *fopen(const char *filename, const char *mode);
```

**常用打开模式**：
- "r"：只读模式
- "w"：写入模式（会截断文件）
- "a"：追加模式
- "r+"：读写模式（文件必须存在）
- "w+"：读写模式（创建或截断文件）
- "a+"：读写追加模式

**示例：检查文件可读写性**
```c
#include <stdio.h>
#include <stdbool.h>

_Bool isReadWriteable(const char *filename) {
    FILE *fp = fopen(filename, "r+");
    if (fp != NULL) {
        fclose(fp);
        return true;
    }
    return false;
}
```

### 1.2 文件关闭与错误处理

始终使用`fclose()`关闭文件，并检查操作是否成功：

```c
if (fclose(fp) != 0) {
    perror("关闭文件失败");
    // 错误处理
}
```

## 2. C++文件流操作

### 2.1 文件流类体系

C++提供了三个主要的文件流类：
- `ifstream`：输入文件流（只读）
- `ofstream`：输出文件流（只写）
- `fstream`：输入输出文件流（读写）

```cpp
#include <fstream>
using namespace std;
```

### 2.2 文件打开模式

| 模式标志       | 描述                                                                 |
|----------------|----------------------------------------------------------------------|
| ios::in        | 以读取方式打开文件                                                   |
| ios::out       | 以写入方式打开文件（默认会截断文件）                                 |
| ios::app       | 追加模式，所有写入都追加到文件末尾                                   |
| ios::ate       | 打开文件后立即定位到文件末尾                                         |
| ios::trunc     | 如果文件已存在则截断文件                                             |
| ios::binary    | 以二进制模式打开文件                                                 |

**组合使用示例**：
```cpp
// 以读写方式打开，不截断文件
fstream file("data.txt", ios::in | ios::out | ios::app);
```

### 2.3 基本读写操作

**写入文件示例**：
```cpp
void writeToFile(const string& filename, const string& content) {
    ofstream outfile(filename, ios::out);
    if (!outfile) {
        cerr << "无法打开文件: " << filename << endl;
        return;
    }
    outfile << content;
    outfile.close();
}
```

**读取文件示例**：
```cpp
string readFromFile(const string& filename) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "无法打开文件: " << filename << endl;
        return "";
    }
    
    string content((istreambuf_iterator<char>(infile)), 
                   istreambuf_iterator<char>());
    infile.close();
    return content;
}
```

## 3. 高级文件操作技术

### 3.1 流状态管理

C++文件流提供了状态检查方法：

| 方法      | 描述                          |
|-----------|-------------------------------|
| good()    | 流状态正常                    |
| bad()     | 发生严重错误                  |
| fail()    | 操作失败（非严重错误）        |
| eof()     | 到达文件末尾                  |
| clear()   | 重置流状态                    |

**示例：安全读取文件**
```cpp
ifstream infile("data.txt");
string line;

while (getline(infile, line)) {
    if (infile.fail() && !infile.eof()) {
        cerr << "读取文件时发生错误" << endl;
        break;
    }
    cout << line << endl;
}

if (infile.bad()) {
    cerr << "发生严重错误" << endl;
}
infile.close();
```

### 3.2 文件定位操作

**随机访问方法**：
- `seekg()`：**设置**输入位置（用于读取）
- `seekp()`：**设置**输出位置（用于写入）
- `tellg()`：***获取***当前读取位置
- `tellp()`：***获取***当前写入位置

**位置基准**：
- `ios::beg`：文件开头
- `ios::cur`：当前位置
- `ios::end`：文件末尾

**示例：修改文件中间内容**
```cpp
fstream file("data.txt", ios::in | ios::out);
if (!file) {
    cerr << "无法打开文件" << endl;
    return;
}

// 定位到第10个字节处
file.seekp(10, ios::beg);
file << "MODIFIED";  // 写入新内容

// 获取当前位置
streampos pos = file.tellp();
cout << "当前位置: " << pos << endl;

file.close();
```

### 3.3 二进制文件操作

**写入二进制数据**：
```cpp
struct Person {
    char name[50];
    int age;
    double height;
};

void writeBinaryFile() {
    ofstream outfile("people.dat", ios::binary);
    Person p = {"张三", 30, 175.5};
    outfile.write(reinterpret_cast<char*>(&p), sizeof(Person));
    outfile.close();
}
```

**读取二进制数据**：
```cpp
void readBinaryFile() {
    ifstream infile("people.dat", ios::binary);
    Person p;
    infile.read(reinterpret_cast<char*>(&p), sizeof(Person));
    cout << "姓名: " << p.name 
         << ", 年龄: " << p.age 
         << ", 身高: " << p.height << endl;
    infile.close();
}
```

## 4. 实际应用示例

### 4.1 日志系统实现

```cpp
class Logger {
private:
    ofstream logFile;
    mutex mtx;  // 用于线程安全
    
public:
    Logger(const string& filename) {
        logFile.open(filename, ios::out | ios::app);
        if (!logFile) {
            throw runtime_error("无法打开日志文件");
        }
    }
    
    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }
    
    void log(const string& message) {
        lock_guard<mutex> lock(mtx);
        time_t now = time(nullptr);
        logFile << put_time(localtime(&now), "%Y-%m-%d %H:%M:%S") 
                << " - " << message << endl;
    }
};
```

### 4.2 CSV文件解析器

```cpp
vector<vector<string>> parseCSV(const string& filename) {
    ifstream infile(filename);
    if (!infile) {
        throw runtime_error("无法打开CSV文件");
    }
    
    vector<vector<string>> data;
    string line;
    
    while (getline(infile, line)) {
        vector<string> row;
        stringstream ss(line);
        string cell;
        
        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        
        data.push_back(row);
    }
    
    infile.close();
    return data;
}
```

## 5. 最佳实践与注意事项

1. **资源管理**：
   - 始终检查文件是否成功打开
   - 使用RAII技术管理文件资源
   - 考虑使用`unique_ptr`自定义删除器管理文件指针

2. **性能优化**：
   - 对于大文件操作，考虑使用内存映射文件
   - 减少频繁的打开/关闭操作
   - 使用缓冲区提高IO效率

3. **跨平台问题**：
   - 注意路径分隔符差异（Windows用`\`，Unix用`/`）
   - 处理文本文件的换行符差异
   - 考虑文件编码问题（UTF-8、GBK等）

4. **错误处理**：
   - 检查所有IO操作是否成功
   - 提供有意义的错误信息
   - 考虑异常安全设计