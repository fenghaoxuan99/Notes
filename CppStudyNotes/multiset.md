
# C++关联容器全面指南

## 1. 关联容器概述

C++标准库提供了四种主要的关联容器，它们都基于键值对数据进行存储和管理：

| 容器类型 | 特点 | 底层实现 |
|---------|------|---------|
| `std::set` | 唯一键集合，自动排序 | 通常红黑树 |
| `std::multiset` | 允许重复键的集合，自动排序 | 通常红黑树 |
| `std::map` | 键值对映射，键唯一，自动排序 | 通常红黑树 |
| `std::multimap` | 键值对映射，允许重复键，自动排序 | 通常红黑树 |

### 1.1 核心区别

- **set/multiset**：只存储键(key)，没有额外的值(value)
- **map/multimap**：存储键值对(key-value pair)
- **set/map**：键必须唯一
- **multiset/multimap**：允许重复键

## 2. std::multiset 深度解析

### 2.1 基本定义

```cpp
template<
    class Key,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<Key>
> class multiset;
```

- **Key**：存储的元素类型
- **Compare**：比较函数对象类型，默认为`std::less<Key>`
- **Allocator**：内存分配器类型

### 2.2 常用操作

#### 2.2.1 插入元素

```cpp
// 1. 直接插入
std::multiset<int> ms;
ms.insert(5);
ms.insert(2);
ms.insert(8);
ms.insert(2);  // 允许重复

// 2. 使用迭代器插入
std::vector<int> vec = {7, 3, 1};
ms.insert(vec.begin(), vec.end());

// 3. 使用初始化列表
ms.insert({4, 4, 6});  // 插入4,4,6

// 4. 使用emplace构造元素
ms.emplace(9);
```

#### 2.2.2 查找元素

```cpp
// 查找元素
auto it = ms.find(2);
if (it != ms.end()) {
    std::cout << "Found: " << *it << std::endl;
}

// 统计元素出现次数
size_t count = ms.count(4);  // 返回2，因为我们插入了两个4
```

#### 2.2.3 删除元素

```cpp
// 1. 通过值删除（会删除所有匹配项）
ms.erase(2);  // 删除所有值为2的元素

// 2. 通过迭代器删除
auto it = ms.find(4);
if (it != ms.end()) {
    ms.erase(it);  // 只删除第一个找到的4
}

// 3. 删除范围
ms.erase(ms.begin(), ms.find(5));  // 删除所有小于5的元素
```

#### 2.2.4 节点操作(C++17)

```cpp
// 提取节点
auto node = ms.extract(4);
if (!node.empty()) {
    node.value() = 10;  // 修改节点值
    ms.insert(std::move(node));  // 重新插入
}
```

### 2.3 完整示例

```cpp
#include <iostream>
#include <set>
#include <vector>

void print_multiset(const std::multiset<int>& ms) {
    for (const auto& elem : ms) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::multiset<int> numbers = {5, 2, 8, 2, 4, 4, 6};

    // 1. 初始状态
    std::cout << "Initial multiset: ";
    print_multiset(numbers);  // 输出: 2 2 4 4 5 6 8

    // 2. 插入操作
    numbers.insert(3);
    numbers.insert({1, 1, 9});
    std::cout << "After insertion: ";
    print_multiset(numbers);  // 输出: 1 1 2 2 3 4 4 5 6 8 9

    // 3. 查找和统计
    std::cout << "Count of 1: " << numbers.count(1) << std::endl;  // 2
    std::cout << "Count of 7: " << numbers.count(7) << std::endl;  // 0

    // 4. 删除操作
    numbers.erase(2);  // 删除所有2
    auto it = numbers.find(4);
    if (it != numbers.end()) {
        numbers.erase(it);  // 只删除第一个4
    }
    std::cout << "After erasure: ";
    print_multiset(numbers);  // 输出: 1 1 3 4 5 6 8 9

    // 5. 节点操作(C++17)
    auto node = numbers.extract(1);
    if (!node.empty()) {
        node.value() = 10;
        numbers.insert(std::move(node));
    }
    std::cout << "After node operation: ";
    print_multiset(numbers);  // 输出: 1 3 4 5 6 8 9 10

    return 0;
}
```

## 3. 性能特点

- **插入**：O(log n)
- **查找**：O(log n)
- **删除**：O(log n)
- **遍历**：O(n)

## 4. 使用场景

1. 需要维护有序集合且允许重复元素
2. 需要频繁查找、插入和删除操作
3. 需要统计元素出现次数
4. 需要按顺序遍历元素

## 5. 最佳实践

1. 当不需要重复元素时，优先使用`std::set`
2. 对于复杂类型，提供高效的比较函数
3. 批量插入时使用范围插入更高效
4. C++17及以上版本可以利用节点操作减少拷贝开销