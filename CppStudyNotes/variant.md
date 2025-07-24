# C++ `std::variant` 完全指南：从基础到高级应用

## 1. 概述与核心概念

`std::variant` 是 C++17 引入的一个模板类，用于表示类型安全的联合体。它解决了传统 C++ 联合体(`union`)的多项限制，同时提供了更安全、更灵活的多类型值存储方案。

### 1.1 与传统联合体的对比

| 特性                | `std::variant`              | 传统 `union`               |
|---------------------|-----------------------------|----------------------------|
| 支持类型            | 任何可复制/移动类型         | 仅平凡类型(POD)            |
| 类型安全            | 是                          | 否                         |
| 空状态              | 可通过`std::monostate`实现  | 无                         |
| 异常安全            | 是                          | 否                         |
| 生命周期管理        | 自动调用析构函数            | 需手动管理                 |

### 1.2 核心优势

1. **类型安全**：编译时类型检查，避免运行时类型错误
2. **支持复杂类型**：可存储`std::string`、`std::vector`等非平凡类型
3. **高效访问**：编译时确定类型布局，无运行时类型信息(RTTI)开销
4. **功能丰富**：提供多种访问方式和工具函数

## 2. 基础用法详解

### 2.1 基本定义与初始化

```cpp
#include <variant>
#include <string>
#include <vector>

// 基本定义
std::variant<int, double, std::string> var1;  // 默认初始化为第一个类型(int)的默认值(0)

// 直接初始化
std::variant<int, double> var2{3.14};         // 存储double
std::variant<std::string, int> var3{"text"};  // 存储string

// 使用std::in_place_index构造
std::variant<std::vector<int>, std::string> var4{
    std::in_place_index<0>, {1, 2, 3}  // 直接构造vector
};
```

### 2.2 值访问方法

#### 2.2.1 std::get

```cpp
auto v = std::variant<int, std::string>{"hello"};

// 按类型访问
try {
    std::cout << std::get<std::string>(v) << "\n";  // 输出"hello"
    std::cout << std::get<int>(v);                  // 抛出std::bad_variant_access
} catch (const std::bad_variant_access& e) {
    std::cerr << "访问错误: " << e.what() << "\n";
}

// 按索引访问
std::cout << std::get<1>(v) << "\n";  // 输出"hello"
```

#### 2.2.2 std::get_if

```cpp
auto v = std::variant<int, std::string>{42};

if (auto* p = std::get_if<int>(&v)) {
    std::cout << "整数值: " << *p << "\n";  // 会执行
} else if (auto* p = std::get_if<std::string>(&v)) {
    std::cout << "字符串值: " << *p << "\n";
}
```

#### 2.2.3 std::visit (推荐方式)

```cpp
// 定义访问器
struct Visitor {
    void operator()(int i) const {
        std::cout << "int: " << i << "\n";
    }
    void operator()(double d) const {
        std::cout << "double: " << d << "\n";
    }
    void operator()(const std::string& s) const {
        std::cout << "string: " << s << "\n";
    }
};

std::variant<int, double, std::string> v = 3.14;
std::visit(Visitor{}, v);  // 输出: double: 3.14

// 使用lambda的现代写法
std::visit([](auto&& arg) {
    using T = std::decay_t<decltype(arg)>;
    if constexpr (std::is_same_v<T, int>) {
        std::cout << "int: " << arg << "\n";
    } else if constexpr (std::is_same_v<T, double>) {
        std::cout << "double: " << arg << "\n";
    } else if constexpr (std::is_same_v<T, std::string>) {
        std::cout << "string: " << arg << "\n";
    }
}, v);
```

### 2.3 类型查询

```cpp
std::variant<int, double, std::string> v = "test";

// 方法1: index()
std::cout << "当前类型索引: " << v.index() << "\n";  // 输出2

// 方法2: holds_alternative
if (std::holds_alternative<std::string>(v)) {
    std::cout << "包含字符串\n";
}

// 方法3: 与std::get_if配合
if (std::get_if<std::string>(&v)) {
    std::cout << "包含字符串\n";
}
```

## 3. 高级特性与应用

### 3.1 就地构造(emplace)

```cpp
std::variant<std::vector<int>, std::string> v;

v.emplace<0>({1, 2, 3});  // 在索引0处构造vector
v.emplace<std::string>("hello");  // 替换为string

// 带参数的构造
v.emplace<0>(3, 42);  // vector包含3个42
```

### 3.2 使用std::monostate表示空状态

```cpp
#include <variant>

std::variant<std::monostate, int, std::string> v;

if (v.index() == 0) {
    std::cout << "variant为空\n";
}

v = 42;
if (!std::holds_alternative<std::monostate>(v)) {
    std::cout << "variant非空\n";
}
```

### 3.3 递归variant实现树形结构

```cpp
struct Node;

using Leaf = std::string;
using Branch = std::vector<Node>;
using TreeNode = std::variant<Leaf, Branch>;

struct Node {
    TreeNode data;
    std::string name;
};

// 使用
TreeNode tree = Branch{
    Node{Leaf{"leaf1"}, "node1"},
    Node{Branch{
        Node{Leaf{"leaf2"}, "node2"},
        Node{Leaf{"leaf3"}, "node3"}
    }, "node4"}
};

// 访问递归variant需要递归访问器
struct TreeVisitor {
    void operator()(const Leaf& leaf) {
        std::cout << "Leaf: " << leaf << "\n";
    }
    
    void operator()(const Branch& branch) {
        std::cout << "Branch with " << branch.size() << " children:\n";
        for (const auto& node : branch) {
            std::visit(*this, node.data);
        }
    }
};

std::visit(TreeVisitor{}, tree);
```

## 4. 实际应用场景

### 4.1 错误处理模式

```cpp
template<typename T>
using Result = std::variant<T, std::string>;

Result<int> parseNumber(const std::string& input) {
    try {
        return std::stoi(input);
    } catch (...) {
        return "无效的数字格式";
    }
}

void processInput(const std::string& input) {
    auto result = parseNumber(input);
    if (auto* err = std::get_if<std::string>(&result)) {
        std::cerr << "错误: " << *err << "\n";
    } else {
        int value = std::get<int>(result);
        std::cout << "解析结果: " << value << "\n";
    }
}
```

### 4.2 状态机实现

```cpp
struct Idle {};
struct Connecting {};
struct Connected { std::string address; };
struct Disconnected { std::string reason; };

using ConnectionState = std::variant<Idle, Connecting, Connected, Disconnected>;

void handleState(ConnectionState& state) {
    std::visit([](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Idle>) {
            std::cout << "开始连接...\n";
        } else if constexpr (std::is_same_v<T, Connecting>) {
            std::cout << "正在连接...\n";
        } else if constexpr (std::is_same_v<T, Connected>) {
            std::cout << "已连接到 " << arg.address << "\n";
        } else if constexpr (std::is_same_v<T, Disconnected>) {
            std::cout << "连接断开: " << arg.reason << "\n";
        }
    }, state);
}
```

### 4.3 解析JSON数据

```cpp
struct JSONNull {};
using JSONValue = std::variant<
    JSONNull,
    bool,
    int,
    double,
    std::string,
    std::map<std::string, JSONValue>,
    std::vector<JSONValue>
>;

JSONValue parseJSON(const std::string& jsonStr);

void printJSON(const JSONValue& value, int indent = 0) {
    std::string indentStr(indent, ' ');
    std::visit([&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, JSONNull>) {
            std::cout << indentStr << "null\n";
        } else if constexpr (std::is_same_v<T, bool>) {
            std::cout << indentStr << (arg ? "true" : "false") << "\n";
        } else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double>) {
            std::cout << indentStr << arg << "\n";
        } else if constexpr (std::is_same_v<T, std::string>) {
            std::cout << indentStr << "\"" << arg << "\"\n";
        } else if constexpr (std::is_same_v<T, std::map<std::string, JSONValue>>) {
            std::cout << indentStr << "{\n";
            for (const auto& [key, val] : arg) {
                std::cout << indentStr << "  \"" << key << "\": ";
                printJSON(val, indent + 2);
            }
            std::cout << indentStr << "}\n";
        } else if constexpr (std::is_same_v<T, std::vector<JSONValue>>) {
            std::cout << indentStr << "[\n";
            for (const auto& item : arg) {
                printJSON(item, indent + 2);
            }
            std::cout << indentStr << "]\n";
        }
    }, value);
}
```

## 5. 性能分析与最佳实践

### 5.1 性能特点

1. **内存布局**：`std::variant`的大小是其最大类型的大小加上少量类型鉴别开销
2. **访问开销**：
   - `std::get`和`std::get_if`：几乎无开销，编译时确定
   - `std::visit`：可能产生少量间接调用开销
3. **构造/赋值开销**：与所含类型的构造/赋值开销相同

### 5.2 最佳实践

1. **优先使用`std::visit`**：最安全且可扩展的访问方式
2. **避免重复类型**：`variant<int, int>`会导致访问歧义
3. **考虑类型顺序**：将最常用/最小的类型放在前面
4. **处理所有类型**：确保访问器处理所有可能的类型
5. **与`std::optional`结合**：当需要"无值"状态时考虑组合使用

### 5.3 与替代方案比较

| 场景                | 推荐方案               | 原因                                                                 |
|---------------------|------------------------|----------------------------------------------------------------------|
| 固定类型集合        | `std::variant`         | 类型安全、高效                                                       |
| 完全开放类型        | `std::any`             | 需要存储任意类型                                                     |
| 单一类型+空状态     | `std::optional`        | 更简单的语义                                                         |
| 多态行为            | 继承+虚函数            | 需要运行时多态                                                       |
| C兼容代码           | 传统`union`            | 需要与C交互                                                          |

## 6. 常见问题与解决方案

### 6.1 如何判断variant是否为空？

```cpp
// 方法1: 使用monostate
std::variant<std::monostate, int, std::string> v;
if (std::holds_alternative<std::monostate>(v)) {
    // 为空
}

// 方法2: 使用optional包装
std::optional<std::variant<int, std::string>> v;
if (!v.has_value()) {
    // 为空
}
```

### 6.2 如何处理variant的默认构造？

```cpp
struct NonDefaultConstructible {
    NonDefaultConstructible(int) {}
};

// 错误: 第一个类型不可默认构造
// std::variant<NonDefaultConstructible, int> v;  

// 解决方案1: 调整类型顺序
std::variant<int, NonDefaultConstructible> v1;  // OK

// 解决方案2: 显式初始化
std::variant<NonDefaultConstructible, int> v2{
    std::in_place_index<1>, 42
};
```

### 6.3 如何实现variant的转换？

```cpp
// 安全转换函数
template <typename... NewTypes, typename... OldTypes>
std::optional<std::variant<NewTypes...>> variant_cast(
    const std::variant<OldTypes...>& old) {
    
    return std::visit([](auto&& arg) -> std::optional<std::variant<NewTypes...>> {
        using T = std::decay_t<decltype(arg)>;
        if constexpr ((std::is_convertible_v<T, NewTypes> || ...)) {
            return std::variant<NewTypes...>{arg};
        } else {
            return std::nullopt;
        }
    }, old);
}

// 使用示例
std::variant<int, double, std::string> v1 = 3.14;
auto v2 = variant_cast<int, std::string>(v1);  // 成功转换为variant<int, string>
auto v3 = variant_cast<std::string>(v1);       // 失败返回nullopt
```

## 7. C++20/23中的增强

### 7.1 C++20的`std::visit`增强

```cpp
// 访问多个variant
std::variant<int, double> v1 = 42;
std::variant<std::string, char> v2 = 'A';

std::visit([](auto a, auto b) {
    std::cout << a << ", " << b << "\n";
}, v1, v2);
```

### 7.2 C++23的`std::variant`新特性

```cpp
// 1. 访问器返回类型推导
auto visitor = []<typename T>(T&& value) -> T {
    return value;
};
std::variant<int, double> v = 3.14;
auto result = std::visit(visitor, v);  // result类型为variant<int, double>

// 2. 更灵活的emplace
std::variant<std::vector<int>, std::string> v;
v.emplace<std::vector<int>>(3, 1);  // 构造vector{1, 1, 1}
```

## 8. 总结

`std::variant`是C++17引入的强大工具，它提供了类型安全的多类型存储方案。通过本指南，我们全面介绍了：

1. 基础用法：定义、初始化、访问方法
2. 高级特性：就地构造、递归variant、monostate
3. 实际应用：错误处理、状态机、JSON解析等模式
4. 性能分析与最佳实践
5. 常见问题解决方案
6. C++20/23中的增强特性

在以下场景中优先考虑使用`std::variant`：
- 需要存储一组预定义类型中的一种
- 需要类型安全且高效的访问
- 需要避免动态内存分配
- 实现状态机或解析异构数据
