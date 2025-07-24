# C++ `std::optional` 全面指南：从基础到高级应用

## 一、`std::optional` 概述

`std::optional` 是 C++17 引入的模板类，用于表示一个可能包含值（类型为 `T`）或"无值"的状态。它提供了一种类型安全且显式的方式处理可能缺失的值，避免了使用特殊标记值（如 `nullptr`、`-1` 等）或额外的 `bool` 变量来标识有效性。

### 核心优势
- **类型安全**：明确表达值的"存在"或"不存在"，避免歧义
- **零成本抽象**：直接存储值（不依赖动态内存分配），性能高效
- **代码清晰**：替代传统方案（如指针、`std::pair<T, bool>` 或特殊值），提升可读性
- **标准库支持**：与其他C++标准库组件良好集成

## 二、基本用法详解

### 1. 创建与初始化

#### 1.1 直接初始化
```cpp
#include <optional>
#include <string>
#include <vector>

// 默认构造（无值）
std::optional<int> opt1;                   

// 值初始化（隐式转换）
std::optional<std::string> opt2 = "Hello"; 

// 显式无值
std::optional<double> opt3 = std::nullopt; 

// 使用 std::in_place 构造（避免临时对象）
std::optional<std::vector<int>> opt4(std::in_place, {1, 2, 3});

// 使用工厂函数构造
auto opt5 = std::make_optional(3.14);
```

#### 1.2 使用 `std::nullopt`
```cpp
inline constexpr nullopt_t nullopt{/*unspecified*/};
```
`std::nullopt` 是 `std::nullopt_t` 类型的常量，用于指示 `std::optional` 不包含值。

### 2. 赋值与重置

```cpp
std::optional<int> opt;

// 赋值
opt = 42;       

// 重置为无值（两种等效方式）
opt = std::nullopt; 
opt.reset();    
```

### 3. 检查值是否存在

```cpp
std::optional<std::string> name = get_name();

// 方法1：has_value()
if (name.has_value()) {
    std::cout << "Name: " << *name << std::endl;
}

// 方法2：bool转换
if (name) {
    std::cout << "Name length: " << name->length() << std::endl;
}

// 方法3：value_or()
std::cout << name.value_or("(unknown)") << std::endl;
```

### 4. 访问值

```cpp
std::optional<int> opt = 42;

// 安全访问（若不存在则抛出 std::bad_optional_access）
int value1 = opt.value();

// 提供默认值
int value2 = opt.value_or(0);

// 直接访问（需确保值存在）
int value3 = *opt;
auto size = opt->some_method(); // 假设T有some_method()
```

### 5. 原位构造与交换

#### 5.1 `emplace()` - 原位构造
```cpp
template< class... Args >
T& emplace( Args&&... args );
```
示例：
```cpp
std::optional<std::vector<int>> opt;
opt.emplace({1, 2, 3});  // 直接构造vector，无需临时对象
```

#### 5.2 `swap()` - 交换内容
```cpp
void swap( optional& other ) noexcept(...);
```
示例：
```cpp
std::optional<int> a = 1;
std::optional<int> b = 2;
a.swap(b);  // 现在 a=2, b=1
```

## 三、典型应用场景

### 1. 函数返回可能无效的结果

```cpp
// 查找元素，可能不存在
std::optional<int> find_item(const std::vector<int>& vec, int target) {
    auto it = std::find(vec.begin(), vec.end(), target);
    if (it != vec.end()) return *it;
    return std::nullopt;
}

// 使用示例
auto numbers = {1, 2, 3, 4, 5};
if (auto result = find_item(numbers, 3)) {
    std::cout << "Found: " << *result << std::endl;
} else {
    std::cout << "Not found" << std::endl;
}
```

### 2. 延迟初始化或可选成员变量

```cpp
class DatabaseConnection {
public:
    void connect() {
        if (!connection_) {
            connection_ = establish_connection();
        }
    }
    
    bool is_connected() const {
        return connection_.has_value();
    }
    
    // 获取连接（必须确保已连接）
    Connection& get_connection() {
        return connection_.value();
    }

private:
    std::optional<Connection> connection_;
};
```

### 3. 解析可能失败的数据

```cpp
// 传统方式：使用特殊值表示失败
int parse_int(const std::string& s) {
    try { return std::stoi(s); }
    catch (...) { return -1; } // 歧义：-1 是错误还是有效值？
}

// 使用 std::optional
std::optional<int> safe_parse(const std::string& s) {
    try { return std::stoi(s); }
    catch (...) { return std::nullopt; }
}

// 使用示例
auto num = safe_parse("123");
if (num) {
    std::cout << "Parsed: " << *num << std::endl;
} else {
    std::cout << "Invalid number" << std::endl;
}
```

### 4. 环境变量读取示例

```cpp
#include <cstdlib>
#include <iostream>
#include <optional>

std::optional<const char*> maybe_getenv(const char* name) {
    if (const char* value = std::getenv(name))
        return value;
    return std::nullopt;
}

int main() {
    // 输出SHELL环境变量，若无则输出(none)
    std::cout << maybe_getenv("SHELL").value_or("(none)") << '\n';
    
    // 检查特定环境变量是否存在
    if (auto path = maybe_getenv("PATH")) {
        std::cout << "PATH is set to: " << *path << std::endl;
    }
}
```

## 四、高级用法

### 1. Monadic 操作（C++23）

C++23 为 `std::optional` 添加了函数式编程风格的链式操作：

```cpp
// and_then: 如果optional有值，则应用函数并返回新的optional
std::optional<int> a = 42;
auto b = a.and_then([](int x) { 
    return x > 0 ? std::optional(x*2) : std::nullopt; 
});
// b = 84

// transform: 映射值到新类型
auto c = a.transform([](int x) { return std::to_string(x); });
// c = optional<string>{"42"}

// or_else: 如果无值，则调用提供的函数
auto d = std::optional<int>{}.or_else([] { 
    return std::optional<int>{0}; 
});
// d = 0
```

### 2. 与 `std::variant` 结合

当需要更丰富的错误信息时，可以结合 `std::variant`：

```cpp
enum class Error { InvalidInput, OutOfRange, NetworkError };

std::variant<std::optional<int>, Error> parse_input(const std::string& input) {
    if (input.empty()) return Error::InvalidInput;
    
    try {
        int value = std::stoi(input);
        if (value < 0 || value > 100) 
            return Error::OutOfRange;
        return value;
    } catch (...) {
        return Error::InvalidInput;
    }
}

// 使用示例
auto result = parse_input("42");
if (std::holds_alternative<std::optional<int>>(result)) {
    if (auto opt = std::get<std::optional<int>>(result)) {
        std::cout << "Success: " << *opt << std::endl;
    } else {
        std::cout << "Unexpected error" << std::endl;
    }
} else {
    auto err = std::get<Error>(result);
    std::cout << "Error: " << static_cast<int>(err) << std::endl;
}
```

### 3. 性能优化技巧

`std::optional` 通常会增加少量内存开销（通常为 `sizeof(T) + 1`，可能有对齐填充）。对于性能敏感场景：

```cpp
// 优化1：对小类型使用optional
std::optional<int> small_opt;  // 通常占用8字节（4+1+对齐）

// 优化2：对大类型考虑指针
std::optional<std::vector<int>> big_opt;  // 可能比裸指针占用更多空间

// 优化3：使用标记位替代optional
struct CompactOptional {
    int value;
    bool has_value;
};  // 通常与std::optional<int>大小相同，但更明确
```

## 五、注意事项与最佳实践

1. **访问前必须检查**：直接使用 `operator*` 或 `operator->` 前必须确保值存在，否则是未定义行为
   ```cpp
   std::optional<int> opt;
   // 危险！未定义行为
   // int x = *opt;  
   
   // 安全访问
   if (opt) { int x = *opt; }
   ```

2. **不要滥用**：
   - 仅当"无值"是正常逻辑的一部分时使用
   - 对于真正的错误情况（如文件不存在、网络断开），应考虑使用异常
   - 对于性能关键的小类型，考虑使用特殊标记值可能更高效

3. **与指针的区别**：
   - `optional` 直接存储值，而指针存储地址
   - `optional` 不涉及动态内存分配
   - `optional` 语义更明确，不需要考虑所有权问题

4. **移动语义**：
   ```cpp
   std::optional<std::string> opt1 = "hello";
   auto opt2 = std::move(opt1);  // opt1现在为空
   ```

5. **比较操作**：
   ```cpp
   std::optional<int> a = 1, b = 2, c;
   a < b;   // true
   a == 1;  // true
   c == std::nullopt;  // true
   ```

## 六、总结

`std::optional` 是C++17中一个极其实用的工具，它通过类型安全的方式明确处理"有/无值"的场景，显著提升了代码的：

- **可读性**：明确表达意图，避免魔术数字或特殊标记
- **安全性**：强制处理缺失值的情况，减少运行时错误
- **性能**：通常比动态分配更高效，适合值语义

在实际开发中，合理使用 `std::optional` 可以：
1. 替代可能为null的指针
2. 表示可能失败的函数结果
3. 实现延迟初始化
4. 处理用户输入或外部数据解析