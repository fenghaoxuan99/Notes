# C++ string 类全面指南

## 1. string 基本概念与构造

string 是 C++ 标准库提供的字符串类，它封装了字符数组并提供了丰富的操作方法，解决了 C 风格字符串(字符指针)的诸多不便。

**主要特点：**
- 自动管理内存
- 提供丰富的成员函数
- 支持运算符重载
- 可动态扩展

### 1.1 构造函数

| 构造函数原型 | 功能 | 示例 |
|-------------|------|------|
| `string()` | 创建空字符串 | `string s1;` |
| `string(const char* str)` | 用 C 风格字符串初始化 | `string s2("hello");` |
| `string(const string& str)` | 拷贝构造函数 | `string s3(s2);` |
| `string(int n, char c)` | 用 n 个字符 c 初始化 | `string s4(5, 'a'); // "aaaaa"` |
| `string(InputIt first, InputIt last)` | 用迭代器范围初始化 | `string s5(s2.begin(), s2.end());` |

**示例代码：**
```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s1;                  // 空字符串
    string s2("Hello World");   // 用C字符串初始化
    string s3(s2);              // 拷贝构造
    string s4(5, 'A');          // "AAAAA"
    string s5(s2.begin(), s2.begin()+5); // "Hello"
    
    cout << "s1: " << s1 << endl;
    cout << "s2: " << s2 << endl;
    cout << "s3: " << s3 << endl;
    cout << "s4: " << s4 << endl;
    cout << "s5: " << s5 << endl;
    
    return 0;
}
```

## 2. string 赋值操作

string 提供了多种赋值方式，包括运算符重载和成员函数。

### 2.1 赋值方法

| 方法原型 | 功能 | 示例 |
|---------|------|------|
| `string& operator=(const char* str)` | 用C字符串赋值 | `s = "hello";` |
| `string& operator=(const string& str)` | 用string对象赋值 | `s = other_str;` |
| `string& operator=(char c)` | 用单个字符赋值 | `s = 'a';` |
| `string& assign(const char *str)` | 用C字符串赋值 | `s.assign("hello");` |
| `string& assign(const char *s, int n)` | 用C字符串前n个字符赋值 | `s.assign("hello", 3); // "hel"` |
| `string& assign(const string &s)` | 用string对象赋值 | `s.assign(other_str);` |
| `string& assign(int n, char c)` | 用n个字符c赋值 | `s.assign(5, 'x'); // "xxxxx"` |

**示例代码：**
```cpp
void assignmentDemo() {
    string s1, s2, s3, s4;
    
    // 使用运算符赋值
    s1 = "Hello";
    s2 = s1;
    s3 = 'A';
    
    // 使用assign方法
    s4.assign("World");
    string s5;
    s5.assign("Hello World", 5);  // "Hello"
    
    string s6;
    s6.assign(s1);  // "Hello"
    
    string s7;
    s7.assign(3, 'Z');  // "ZZZ"
    
    cout << "s1: " << s1 << endl;
    cout << "s2: " << s2 << endl;
    cout << "s3: " << s3 << endl;
    cout << "s4: " << s4 << endl;
    cout << "s5: " << s5 << endl;
    cout << "s6: " << s6 << endl;
    cout << "s7: " << s7 << endl;
}
```

## 3. string 拼接操作

string 提供了多种字符串拼接方式，可以方便地将内容添加到现有字符串末尾。

### 3.1 拼接方法

| 方法原型 | 功能 | 示例 |
|---------|------|------|
| `string& operator+=(const char* str)` | 追加C字符串 | `s += " world";` |
| `string& operator+=(const char c)` | 追加单个字符 | `s += '!';` |
| `string& operator+=(const string& str)` | 追加string对象 | `s += other_str;` |
| `string& append(const char *str)` | 追加C字符串 | `s.append(" world");` |
| `string& append(const char *str, int n)` | 追加C字符串前n个字符 | `s.append("world", 3); // "wor"` |
| `string& append(const string &str)` | 追加string对象 | `s.append(other_str);` |
| `string& append(const string &s, int pos, int n)` | 追加string对象从pos开始的n个字符 | `s.append(other_str, 1, 3);` |

**示例代码：**
```cpp
void concatenationDemo() {
    string s1 = "Hello";
    
    // 使用+=运算符
    s1 += " World";
    s1 += '!';
    
    string s2 = " C++";
    s1 += s2;
    cout << "After += operations: " << s1 << endl; // "Hello World! C++"
    
    // 使用append方法
    string s3 = "Programming";
    s3.append(" is fun");
    cout << "After append: " << s3 << endl; // "Programming is fun"
    
    s3.append(" language", 4); // " lang"
    cout << "After append 4 chars: " << s3 << endl; // "Programming is fun lang"
    
    string s4 = " with C++";
    s3.append(s4);
    cout << "After append string: " << s3 << endl; // "Programming is fun lang with C++"
    
    s3.append(s4, 5, 3); // " C++"中从第5个字符开始取3个
    cout << "After append substring: " << s3 << endl; // "Programming is fun lang with C++ C++"
}
```

## 4. string 查找与替换

string 提供了多种查找和替换子串的方法。

### 4.1 查找方法

| 方法原型 | 功能 | 返回值 |
|---------|------|--------|
| `size_type find(const string& str, size_type pos = 0)` | 查找子串第一次出现位置 | 位置或string::npos |
| `size_type find(const char* str, size_type pos = 0)` | 同上 | 同上 |
| `size_type find(const char* str, size_type pos, size_type n)` | 查找前n个字符 | 同上 |
| `size_type rfind(const string& str, size_type pos = npos)` | 查找子串最后一次出现位置 | 同上 |
| `size_type rfind(const char* s, size_type pos, size_type n)` | 查找前n个字符最后一次出现 | 同上 |
| `size_type find_first_of(const string& str, size_type pos = 0)` | 查找str中任意字符第一次出现 | 同上 |
| `size_type find_first_not_of(const string& str, size_type pos = 0)` | 查找不在str中的字符第一次出现 | 同上 |

### 4.2 替换方法

| 方法原型 | 功能 | 示例 |
|---------|------|------|
| `string& replace(size_type pos, size_type n, const string& str)` | 替换从pos开始的n个字符为str | `s.replace(1, 3, "abc")` |
| `string& replace(size_type pos, size_type n, const char* s)` | 同上 | 同上 |
| `string& replace(size_type pos, size_type n, size_type n1, char c)` | 替换为n1个字符c | `s.replace(1, 3, 2, 'x')` |
| `string& replace(iterator i1, iterator i2, const char* s)` | 替换迭代器范围 | `s.replace(s.begin()+1, s.begin()+4, "abc")` |

**示例代码：**
```cpp
void searchReplaceDemo() {
    string s = "Hello World, welcome to C++ programming!";
    
    // 查找示例
    size_t found = s.find("World");
    if (found != string::npos) {
        cout << "'World' found at position: " << found << endl;
    }
    
    found = s.find("C++", 0, 2); // 查找"C+"
    if (found != string::npos) {
        cout << "'C+' found at position: " << found << endl;
    }
    
    found = s.rfind("o"); // 查找最后一个'o'
    if (found != string::npos) {
        cout << "Last 'o' found at position: " << found << endl;
    }
    
    // 替换示例
    s.replace(6, 5, "Universe"); // "World" -> "Universe"
    cout << "After replace: " << s << endl;
    
    s.replace(s.begin()+6, s.begin()+14, "Earth"); // "Universe" -> "Earth"
    cout << "After iterator replace: " << s << endl;
    
    // 使用find_first_of查找第一个元音字母
    string vowels = "aeiouAEIOU";
    found = s.find_first_of(vowels);
    while (found != string::npos) {
        cout << "Vowel found at: " << found << " (" << s[found] << ")" << endl;
        found = s.find_first_of(vowels, found+1);
    }
}
```

## 5. string 字符访问与子串操作

### 5.1 字符访问方法

| 方法 | 功能 | 示例 |
|-----|------|------|
| `char& operator[](size_type pos)` | 访问指定位置字符 | `char c = s[0];` |
| `char& at(size_type pos)` | 访问指定位置字符(带边界检查) | `char c = s.at(0);` |
| `const char* c_str()` | 获取C风格字符串 | `printf("%s", s.c_str());` |
| `const char* data()` | 获取字符数组 | `memcpy(buf, s.data(), s.size());` |

### 5.2 子串操作

`substr`方法用于获取子串：
```cpp
basic_string substr(size_type pos = 0, size_type count = npos) const;
```

**示例代码：**
```cpp
void accessSubstrDemo() {
    string s = "Hello World";
    
    // 字符访问
    cout << "First character: " << s[0] << endl;
    cout << "Second character: " << s.at(1) << endl;
    
    // 修改字符
    s[0] = 'h';
    s.at(6) = 'w';
    cout << "After modification: " << s << endl;
    
    // 获取C风格字符串
    const char* cstr = s.c_str();
    cout << "C string: " << cstr << endl;
    
    // 子串操作
    string sub1 = s.substr(6);     // "world"
    string sub2 = s.substr(0, 5);  // "hello"
    string sub3 = s.substr(6, 5);  // "world"
    
    cout << "sub1: " << sub1 << endl;
    cout << "sub2: " << sub2 << endl;
    cout << "sub3: " << sub3 << endl;
    
    // 使用data()获取底层数组
    char buffer[20];
    size_t length = s.copy(buffer, s.size());
    buffer[length] = '\0';
    cout << "Copied buffer: " << buffer << endl;
}
```

## 6. string 插入与删除

### 6.1 插入方法

| 方法原型 | 功能 | 示例 |
|---------|------|------|
| `string& insert(size_type pos, const char* str)` | 在pos插入C字符串 | `s.insert(5, " dear");` |
| `string& insert(size_type pos, const string& str)` | 在pos插入string对象 | `s.insert(5, other_str);` |
| `string& insert(size_type pos, size_type n, char c)` | 在pos插入n个字符c | `s.insert(5, 3, '!');` |
| `iterator insert(iterator p, char c)` | 在迭代器位置插入字符 | `s.insert(s.begin()+5, '!');` |

### 6.2 删除方法

| 方法原型 | 功能 | 示例 |
|---------|------|------|
| `string& erase(size_type pos = 0, size_type n = npos)` | 删除从pos开始的n个字符 | `s.erase(5, 6);` |
| `iterator erase(iterator p)` | 删除迭代器位置的字符 | `s.erase(s.begin()+5);` |
| `iterator erase(iterator first, iterator last)` | 删除迭代器范围的字符 | `s.erase(s.begin()+5, s.begin()+10);` |
| `void clear()` | 清空字符串 | `s.clear();` |

**示例代码：**
```cpp
void insertEraseDemo() {
    string s = "Hello World";
    
    // 插入操作
    s.insert(5, " dear"); // "Hello dear World"
    cout << "After insert: " << s << endl;
    
    s.insert(0, 3, '!'); // "!!!Hello dear World"
    cout << "After insert chars: " << s << endl;
    
    // 使用迭代器插入
    s.insert(s.begin()+5, '*');
    cout << "After iterator insert: " << s << endl;
    
    // 删除操作
    s.erase(0, 3); // 删除前3个'!'
    cout << "After erase: " << s << endl;
    
    s.erase(s.begin()+5); // 删除'*'
    cout << "After iterator erase: " << s << endl;
    
    s.erase(5, 5); // 删除" dear"
    cout << "After erase substring: " << s << endl;
    
    s.clear();
    cout << "After clear: " << s << endl;
}
```

## 7. string 大小与容量

### 7.1 大小相关方法

| 方法 | 功能 | 示例 |
|-----|------|------|
| `size_type size() const` | 返回字符数 | `int len = s.size();` |
| `size_type length() const` | 同上 | `int len = s.length();` |
| `bool empty() const` | 判断是否为空 | `if (s.empty()) {...}` |
| `size_type max_size() const` | 返回最大可能字符数 | `cout << s.max_size();` |
| `size_type capacity() const` | 返回当前分配的存储容量 | `cout << s.capacity();` |
| `void resize(size_type n)` | 调整字符串大小 | `s.resize(10);` |
| `void reserve(size_type n)` | 预留存储空间 | `s.reserve(100);` |
| `void shrink_to_fit()` | 减少容量以适应大小 | `s.shrink_to_fit();` |

**示例代码：**
```cpp
void sizeCapacityDemo() {
    string s = "Hello";
    
    cout << "Size: " << s.size() << endl;
    cout << "Length: " << s.length() << endl;
    cout << "Empty: " << (s.empty() ? "true" : "false") << endl;
    cout << "Max size: " << s.max_size() << endl;
    cout << "Capacity: " << s.capacity() << endl;
    
    s.reserve(100);
    cout << "After reserve(100), capacity: " << s.capacity() << endl;
    
    s.resize(3);
    cout << "After resize(3): " << s << endl;
    cout << "Size: " << s.size() << endl;
    cout << "Capacity: " << s.capacity() << endl;
    
    s.shrink_to_fit();
    cout << "After shrink_to_fit(), capacity: " << s.capacity() << endl;
    
    s.clear();
    cout << "After clear, empty: " << (s.empty() ? "true" : "false") << endl;
}
```

## 8. string 类型转换

### 8.1 其他类型转为string

1. 使用`to_string()`函数:
```cpp
int i = 42;
double d = 3.14;
string s1 = to_string(i); // "42"
string s2 = to_string(d); // "3.140000"
```

2. 使用字符串流:
```cpp
#include <sstream>
int num = 123;
ostringstream oss;
oss << num;
string s = oss.str(); // "123"
```

### 8.2 string转为其他类型

1. 使用`stoi`, `stol`, `stoll`等函数:
```cpp
string s1 = "42";
int i = stoi(s1);

string s2 = "3.14";
double d = stod(s2);
```

2. 使用字符串流:
```cpp
string s = "123";
istringstream iss(s);
int num;
iss >> num;
```

3. 使用C风格函数:
```cpp
string s = "456";
int num;
sscanf(s.c_str(), "%d", &num);
```

**示例代码：**
```cpp
void conversionDemo() {
    // 其他类型转string
    int i = 42;
    double d = 3.14159;
    string s1 = to_string(i);
    string s2 = to_string(d);
    cout << "s1: " << s1 << endl; // "42"
    cout << "s2: " << s2 << endl; // "3.141590"
    
```