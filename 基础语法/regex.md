
std::regex 是 C++11 标准库中引入的正则表达式库，位于头文件 <regex> 中。它提供了一系列类和函数来执行正则表达式的匹配、搜索、替换等操作。下面我将详细介绍 std::regex 的用法，包括正则表达式语法、主要类、函数以及示例。

### 1. 正则表达式语法
std::regex 默认使用 ECMAScript 语法（类似于 JavaScript 中的正则表达式），但也支持其他语法（如 basic、extended、awk、grep、egrep）。可以通过标志位指定语法。

常用的标志位（在 std::regex_constants 命名空间中）：
- `syntax_option_type::icase`：忽略大小写
- `syntax_option_type::nosubs`：不保存子表达式
- `syntax_option_type::optimize`：优化匹配速度
- `syntax_option_type::collate`：使用区域敏感排序
- `syntax_option_type::ECMAScript`：使用 ECMAScript 语法（默认）
- `syntax_option_type::basic`：使用 POSIX basic 语法
- `syntax_option_type::extended`：使用 POSIX extended 语法
- `syntax_option_type::awk`：使用 awk 语法
- `syntax_option_type::grep`：使用 grep 语法
- `syntax_option_type::egrep`：使用 egrep 语法

### 2. 主要类
- `std::regex`：表示一个正则表达式对象。
- `std::regex_match`：尝试匹配整个字符串。
- `std::regex_search`：在字符串中搜索匹配的子串。
- `std::regex_replace`：替换匹配的子串。
- `std::smatch`：保存匹配结果的容器（针对 std::string），是 std::match_results 的特化版本。类似的还有 `std::cmatch`（针对 const char*）、`std::wsmatch`（针对 std::wstring）等。
- `std::sub_match`：表示子表达式匹配的结果，通常存储在 std::match_results 中。

### 3. 匹配结果：std::smatch 和 std::sub_match
当使用 `std::regex_match` 或 `std::regex_search` 时，匹配结果会存储在 `std::smatch`（或对应的类型）中。它是一个容器，包含多个 `std::sub_match` 对象。

- `smatch[0]`：整个匹配的字符串。
- `smatch[1]`：第一个子表达式（括号内）的匹配结果，依此类推。
- `smatch.size()`：匹配的子表达式数量（包括整个匹配）。
- `smatch.prefix()`：匹配前面的部分。
- `smatch.suffix()`：匹配后面的部分。

`sub_match` 对象可以转换为字符串，或者通过 `.str()` 方法获取字符串。

### 4. 函数详解

#### 4.1 std::regex_match
用于检查整个字符串是否与正则表达式匹配。

函数原型：
```cpp
bool regex_match(const std::string& s, std::smatch& m, const std::regex& e, std::regex_constants::match_flag_type flags = std::regex_constants::match_default);
bool regex_match(const std::string& s, const std::regex& e, std::regex_constants::match_flag_type flags = std::regex_constants::match_default);
```

示例：
```cpp
#include <iostream>
#include <regex>
#include <string>

int main() {
    std::string str = "hello123";
    std::regex pattern("hello\\d+"); // 匹配hello后跟一个或多个数字

    if (std::regex_match(str, pattern)) {
        std::cout << "Match!" << std::endl;
    } else {
        std::cout << "No match!" << std::endl;
    }

    // 使用smatch获取子表达式
    std::regex pattern2("(hello)(\\d+)");
    std::smatch matches;
    if (std::regex_match(str, matches, pattern2)) {
        std::cout << "Whole match: " << matches[0] << std::endl; // hello123
        std::cout << "Part1: " << matches[1] << std::endl; // hello
        std::cout << "Part2: " << matches[2] << std::endl; // 123
    }

    return 0;
}
```

#### 4.2 std::regex_search
在字符串中搜索匹配的子串（不需要整个字符串匹配）。

函数原型：
```cpp
bool regex_search(const std::string& s, std::smatch& m, const std::regex& e, std::regex_constants::match_flag_type flags = std::regex_constants::match_default);
bool regex_search(const std::string& s, const std::regex& e, std::regex_constants::match_flag_type flags = std::regex_constants::match_default);
```

示例：
```cpp
#include <iostream>
#include <regex>
#include <string>

int main() {
    std::string str = "some numbers: 123 and 456";
    std::regex pattern("\\d+"); // 匹配一个或多个数字

    std::smatch match;
    if (std::regex_search(str, match, pattern)) {
        std::cout << "Found number: " << match[0] << std::endl; // 123（只找到第一个）
    }

    // 若要找到所有匹配，可以使用迭代器
    auto words_begin = std::sregex_iterator(str.begin(), str.end(), pattern);
    auto words_end = std::sregex_iterator();
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::cout << match.str() << std::endl; // 123 然后 456
    }

    return 0;
}
```

#### 4.3 std::regex_replace
替换匹配的部分。

函数原型：
```cpp
std::string regex_replace(const std::string& s, const std::regex& e, const std::string& fmt, std::regex_constants::match_flag_type flags = std::regex_constants::match_default);
```

替换格式字符串 `fmt` 中可以使用以下转义：
- `$&`：表示整个匹配
- `$n`：表示第n个子表达式（n为数字）
- `$``：匹配前缀
- `$'`：匹配后缀
- `$$`：表示$字符本身

示例：
```cpp
#include <iostream>
#include <regex>
#include <string>

int main() {
    std::string str = "hello world, hello regex";
    std::regex pattern("hello");

    // 将所有"hello"替换为"hi"
    std::string result = std::regex_replace(str, pattern, "hi");
    std::cout << result << std::endl; // "hi world, hi regex"

    // 使用子表达式替换
    std::string str2 = "123-456";
    std::regex pattern2("(\\d+)-(\\d+)");
    // 将两个数字交换位置
    std::string result2 = std::regex_replace(str2, pattern2, "$2-$1");
    std::cout << result2 << std::endl; // "456-123"

    return 0;
}
```

### 5. 迭代器
- `std::sregex_iterator`：用于遍历字符串中所有匹配的子串。
- `std::sregex_token_iterator`：用于遍历字符串中所有匹配的子表达式或指定部分。

示例（`sregex_token_iterator`）：
```cpp
#include <iostream>
#include <regex>
#include <string>

int main() {
    std::string str = "apple,banana,cherry";
    std::regex pattern(",");

    // 使用逗号分割字符串
    std::sregex_token_iterator it(str.begin(), str.end(), pattern, -1); // -1表示匹配之间的部分
    std::sregex_token_iterator end;
    while (it != end) {
        std::cout << *it++ << std::endl; // 输出 apple, banana, cherry
    }

    // 也可以获取所有子表达式匹配的部分
    std::string str2 = "date: 2023-01-01";
    std::regex pattern2("(\\d{4})-(\\d{2})-(\\d{2})");
    std::sregex_token_iterator it2(str2.begin(), str2.end(), pattern2, {1,2,3}); // 获取三个子表达式
    while (it2 != end) {
        std::cout << *it2++ << std::endl; // 2023, 01, 01
    }

    return 0;
}
```

### 6. 错误处理
正则表达式在构造时可能会抛出 `std::regex_error` 异常，错误类型在 `std::regex_constants::error_type` 中定义。

示例：
```cpp
#include <iostream>
#include <regex>
#include <string>

int main() {
    try {
        std::regex pattern("(\\d+"); // 缺少右括号
    } catch (const std::regex_error& e) {
        std::cout << "Regex error: " << e.what() << std::endl;
        std::cout << "Error code: " << e.code() << std::endl; // 错误码
    }
    return 0;
}
```

### 7. 标志位
在匹配和替换时，可以使用标志位控制行为，例如：
- `match_not_bol`：不将序列开始视为行首
- `match_not_eol`：不将序列结束视为行尾
- `match_not_bow`：不将序列开始视为词首
- `match_not_eow`：不将序列结束视为词尾
- `match_any`：如果有多个匹配，任意一个都可以
- `match_not_null`：不匹配空序列
- `match_continuous`：整个序列必须连续匹配
- `match_prev_avail`：在第一个匹配前还有内容（设置后，^ 会匹配行首）
- `format_default`：默认格式替换（ECMAScript规则）
- `format_sed`：使用sed规则
- `format_no_copy`：不复制未匹配部分
- `format_first_only`：只替换第一个匹配

示例（format_no_copy）：
```cpp
#include <iostream>
#include <regex>
#include <string>

int main() {
    std::string str = "hello123 world456";
    std::regex pattern("\\d+");
    // 只复制匹配的部分（即数字），不复制未匹配的
    std::string result = std::regex_replace(str, pattern, "number", std::regex_constants::format_no_copy);
    std::cout << result << std::endl; // "number number"

    return 0;
}
```

### 8. 性能与注意事项
- 正则表达式的编译（即构造std::regex对象）可能比较耗时，建议重用正则表达式对象。
- 匹配结果（smatch）只在正则表达式对象存在且未被修改时有效。
- 正则表达式语法复杂，注意转义字符（在C++字符串中，反斜杠需要两个，如 `\\d` 表示 \d）。

### 9. 总结
std::regex 提供了强大的正则表达式功能，包括匹配、搜索、替换等操作。通过结合使用 regex、smatch、regex_match、regex_search、regex_replace 以及迭代器，可以完成复杂的文本处理任务。使用时需要注意正则表达式的语法和转义，以及异常处理。

以上内容覆盖了 std::regex 的主要用法，希望对你有所帮助。


### `std::regex` 全面指南

`std::regex` 是 C++11 引入的正则表达式库（头文件 `<regex>`），提供强大的文本模式匹配、搜索和替换功能。以下是详细用法：

---

#### **一、核心类**
| **类名**               | **作用**                                                                 |
|------------------------|--------------------------------------------------------------------------|
| `std::regex`           | 存储正则表达式模式（编译后的正则）                                       |
| `std::smatch`          | 存储匹配结果（用于 `std::string`，包含子匹配信息）                       |
| `std::cmatch`          | 存储匹配结果（用于 C 风格字符串 `const char*`）                          |
| `std::sub_match`       | 表示子表达式匹配结果（如 `smatch[i]`）                                   |
| `std::regex_iterator`  | 迭代器，遍历所有匹配                                                     |
| `std::regex_token_iterator` | 迭代器，遍历子匹配或未匹配部分                                     |

---

#### **二、正则表达式语法（默认 ECMAScript）**
```cpp
// 常用元字符
.   任意字符（除换行）      \d  数字          \s  空白字符
\w  字母/数字/下划线       \D  非数字        \S  非空白字符
^   行首                  $   行尾          |   或
[]  字符集合              [^] 否定字符集     ()  捕获分组
{}  重复次数              *   0次或多次      +   1次或多次
?   0次或1次              \   转义字符

// 示例
std::regex re("\\d{3}-\\d{2}-\\d{4}");  // 匹配SSN：123-45-6789
```

---

#### **三、核心操作函数**
##### 1. **`std::regex_match`（完全匹配）**
```cpp
std::string s = "2023-08-25";
std::regex date_re(R"((\d{4})-(\d{2})-(\d{2}))");  // 分组捕获年月日

std::smatch matches;
if (std::regex_match(s, matches, date_re)) {
    std::cout << "Full match: " << matches[0] << "\n";  // 2023-08-25
    std::cout << "Year: " << matches[1] << "\n";        // 2023
    std::cout << "Month: " << matches[2] << "\n";       // 08
}
```

##### 2. **`std::regex_search`（搜索子串）**
```cpp
std::string log = "Error: 404; Time: 2023-08-25";
std::regex err_re(R"(Error: (\d+))");

std::smatch err_match;
if (std::regex_search(log, err_match, err_re)) {
    std::cout << "Error code: " << err_match[1] << "\n";  // 404
}
```

##### 3. **`std::regex_replace`（替换）**
```cpp
std::string text = "Name: John, Age: 30";
std::regex name_re(R"(Name: (\w+))");

// 替换为哈希值s
std::string result = std::regex_replace(
    text, 
    name_re, 
    "Name: ***$1***"  // $1 引用第一个子匹配
);
// 结果: "Name: ***John***, Age: 30"
```

---

#### **四、迭代器遍历匹配**
##### 1. **`regex_iterator`（遍历所有匹配）**
```cpp
std::string data = "10.20.30.40";
std::regex ip_re(R"((\d{1,3})\.(\d{1,3}))");

auto begin = std::sregex_iterator(data.begin(), data.end(), ip_re);
auto end = std::sregex_iterator();

for (auto it = begin; it != end; ++it) {
    std::smatch match = *it;
    std::cout << "Part: " << match[0] << "\n";  // 10.20, 30.40
    std::cout << "Group1: " << match[1] << "\n"; // 10, 30
}
```

##### 2. **`regex_token_iterator`（按子匹配/分隔符遍历）**
```cpp
std::string csv = "apple,orange,banana";
std::regex comma_re(",");

// 分割字符串（-1 表示非匹配部分）
auto token_it = std::sregex_token_iterator(
    csv.begin(), csv.end(), comma_re, -1
);
while (token_it != std::sregex_token_iterator()) {
    std::cout << *token_it++ << "\n";  // apple, orange, banana
}
```

---

#### **五、标志位（控制行为）**
```cpp
// 语法选项（构造 regex 时使用）
std::regex_constants::icase         // 忽略大小写
std::regex_constants::nosubs        // 不保存子匹配
std::regex_constants::optimize      // 优化匹配速度

// 匹配选项（函数参数）
std::regex_constants::match_not_bol // ^ 不匹配行首
std::regex_constants::format_no_copy // 仅复制匹配部分（替换时）

// 示例：忽略大小写
std::regex word_re("hello", std::regex_constants::icase);
```

---

#### **六、错误处理**
```cpp
try {
    std::regex invalid_re("(unclosed|group");  // 缺少 ')'
} catch (const std::regex_error& e) {
    std::cout << "Error: " << e.what() << "\n";
    // 常见错误码: error_paren, error_backref, error_badrepeat
}
```

---

#### **七、高级技巧**
1. **惰性匹配**  
   `.*?` 或 `.+?` 最小化匹配（默认贪婪匹配）：
   ```cpp
   std::regex lazy_re(R"(<div>(.*?)</div>)");  // 匹配最短内容
   ```

2. **非捕获分组**  
   `(?:...)` 不保存子匹配：
   ```cpp
   std::regex noncap_re("(?:http|ftp)://([^/]+)");  // 不捕获协议
   ```

3. **断言（Lookaround）**  
   `(?=...)` 正向肯定断言，`(?<=...)` 正向否定断言：
   ```cpp
   std::regex lookahead_re("\\d+(?=px)");  // 匹配 "100px" 中的 "100"
   ```

---

#### **八、性能优化**
1. **重用 `regex` 对象**：避免重复编译正则表达式。
2. **预编译正则**：在循环外部定义 `std::regex`。
3. **使用 `regex_constants::optimize`**：
   ```cpp
   std::regex fast_re("pattern", std::regex::optimize);
   ```

---

#### **九、完整示例：解析日志**
```cpp
#include <iostream>
#include <regex>
#include <string>

int main() {
    std::string log = 
        "[ERROR] 2023-08-25 14:30:22: File not found\n"
        "[INFO] 2023-08-25 14:31:05: User logged in";

    std::regex log_re(R"(\[(\w+)\] (\d{4}-\d{2}-\d{2}) (\d{2}:\d{2}:\d{2}): (.+))");

    auto begin = std::sregex_iterator(log.begin(), log.end(), log_re);
    for (auto it = begin; it != std::sregex_iterator(); ++it) {
        std::smatch match = *it;
        std::cout << "Level: " << match[1] << "\n"
                  << "Date: " << match[2] << "\n"
                  << "Time: " << match[3] << "\n"
                  << "Message: " << match[4] << "\n\n";
    }
}
```

**输出**：
```
Level: ERROR
Date: 2023-08-25
Time: 14:30:22
Message: File not found

Level: INFO
Date: 2023-08-25
Time: 14:31:05
Message: User logged in
```

---

#### **十、注意事项**
1. **转义规则**：C++ 字符串需双重转义（如 `\\d` 表示 `\d`），或使用原始字符串 `R"(...)"`。
2. **Unicode 支持**：需结合 `std::wregex` 和宽字符（`std::wstring`）。
3. **匹配结果有效性**：`smatch` 仅在原字符串存在时有效（不复制数据）。

通过灵活组合这些功能，可高效处理文本解析、数据验证、日志分析等复杂任务。