# C++关联容器深度解析：map与set完全指南

## 一、关联容器概述

关联容器是C++标准库中重要的数据结构，它们通过键(key)来高效地存储和检索数据。主要分为两大类：

1. **有序关联容器**：基于红黑树实现，元素自动排序
   - map：键值对(key-value)集合，键唯一
   - multimap：键值对集合，允许多个元素拥有相同键
   - set：键集合，键唯一
   - multiset：键集合，允许多个相同键

2. **无序关联容器**：基于哈希表实现，元素无序
   - unordered_map
   - unordered_multimap
   - unordered_set
   - unordered_multiset

## 二、map容器详解

### 1. 基本特性

```cpp
template<
    class Key,
    class T,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<std::pair<const Key, T>>
> class map;
```

- **底层实现**：红黑树（自平衡二叉搜索树）
- **时间复杂度**：搜索、插入、删除操作均为O(log n)
- **元素特性**：元素是`std::pair<const Key, T>`，键唯一且不可修改
- **排序方式**：默认按Key升序排列，可通过Compare参数自定义

### 2. 创建与初始化

```cpp
// 默认构造
std::map<std::string, int> map1;

// 初始化列表构造
std::map<std::string, int> map2 {
    {"apple", 5},
    {"banana", 3},
    {"orange", 8}
};

// 拷贝构造
std::map<std::string, int> map3(map2);

// 范围构造
std::map<std::string, int> map4(map2.begin(), map2.end());
```

### 3. 元素插入

#### 多种插入方式对比

```cpp
std::map<std::string, int> m;

// 1. 使用insert和pair
m.insert(std::pair<std::string, int>("apple", 5));

// 2. 使用make_pair
m.insert(std::make_pair("banana", 3));

// 3. 使用value_type
m.insert(std::map<std::string, int>::value_type("orange", 8));

// 4. 使用数组下标(若键不存在则插入，存在则修改)
m["pear"] = 4;

// 5. C++17 insert_or_assign
m.insert_or_assign("apple", 6);  // 已存在则修改

// 6. emplace构造
m.emplace("grape", 7);  // 直接构造，避免临时对象

// 7. try_emplace (C++17)
m.try_emplace("melon", 9);  // 仅当键不存在时插入
```

**性能建议**：
- 对于已知不存在的键，`emplace`比`insert`更高效
- 需要覆盖值时，`insert_or_assign`比先`erase`再`insert`更高效
- `try_emplace`在键可能已存在时性能最优

### 4. 元素访问

```cpp
std::map<std::string, int> m{{"apple", 5}, {"banana", 3}};

// 1. 使用at访问(安全，键不存在时抛出异常)
try {
    int count = m.at("apple");
} catch (const std::out_of_range& e) {
    std::cerr << e.what() << '\n';
}

// 2. 使用[]访问(键不存在时会插入默认值)
int count = m["orange"];  // 插入{"orange", 0}

// 3. 使用find查找
auto it = m.find("banana");
if (it != m.end()) {
    std::cout << it->second << '\n';
}

// 4. 使用contains检查存在性(C++20)
if (m.contains("apple")) {
    std::cout << "Found apple\n";
}
```

### 5. 元素删除

```cpp
std::map<int, std::string> m{
    {1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}
};

// 1. 通过迭代器删除
auto it = m.find(2);
if (it != m.end()) {
    m.erase(it);
}

// 2. 通过键删除
size_t num_removed = m.erase(3);  // 返回删除的元素数量(0或1)

// 3. 删除范围
m.erase(m.begin(), m.find(4));  // 删除键小于4的所有元素

// 4. 清空整个map
m.clear();
```

### 6. 查找与统计

```cpp
std::map<std::string, int> m{
    {"apple", 5}, {"banana", 3}, {"orange", 8}
};

// 1. find查找
auto it = m.find("banana");
if (it != m.end()) {
    std::cout << "Found: " << it->second << '\n';
}

// 2. count统计
if (m.count("apple") > 0) {
    std::cout << "Apple exists\n";
}

// 3. lower_bound/upper_bound范围查找
auto low = m.lower_bound("a");  // 第一个不小于"a"的元素
auto up = m.upper_bound("c");   // 第一个大于"c"的元素
for (auto it = low; it != up; ++it) {
    std::cout << it->first << ": " << it->second << '\n';
}

// 4. equal_range获取相等范围
auto range = m.equal_range("banana");
for (auto it = range.first; it != range.second; ++it) {
    std::cout << it->first << ": " << it->second << '\n';
}
```

## 三、set容器详解

### 1. 基本特性

```cpp
template<
    class Key,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<Key>
> class set;
```

- **底层实现**：红黑树
- **元素特性**：只存储键，没有值，键唯一
- **典型应用**：去重、存在性检查、有序集合操作

### 2. 基本操作示例

```cpp
#include <iostream>
#include <set>

int main() {
    std::set<int> s = {5, 2, 8, 2, 1};  // 重复的2只会保留一个
    
    // 插入元素
    s.insert(4);
    auto [it, inserted] = s.insert(5);  // inserted为false，因为5已存在
    
    // 遍历集合
    for (int x : s) {
        std::cout << x << ' ';  // 输出: 1 2 4 5 8
    }
    
    // 查找元素
    if (s.find(3) != s.end()) {
        std::cout << "3 found\n";
    } else {
        std::cout << "3 not found\n";
    }
    
    // 删除元素
    s.erase(2);
    std::cout << "Size after erase: " << s.size() << '\n';
    
    return 0;
}
```

## 四、multimap与multiset

### 1. 主要区别

- 允许重复键
- 没有`operator[]`和`at()`方法
- `insert`总是成功(返回迭代器而非pair)
- `count`可能返回大于1的值

### 2. multimap示例

```cpp
#include <iostream>
#include <map>

int main() {
    std::multimap<std::string, int> mm;
    
    // 插入多个相同键的元素
    mm.insert({"apple", 5});
    mm.insert({"apple", 7});
    mm.insert({"banana", 3});
    
    // 查找所有"apple"
    auto range = mm.equal_range("apple");
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << it->first << ": " << it->second << '\n';
    }
    
    // 统计"apple"的数量
    std::cout << "Number of apples: " << mm.count("apple") << '\n';
    
    return 0;
}
```

## 五、性能对比与选择指南

| 操作          | map/set | unordered_map/unordered_set |
|---------------|---------|-----------------------------|
| 插入          | O(log n)| O(1)平均, O(n)最坏          |
| 查找          | O(log n)| O(1)平均, O(n)最坏          |
| 删除          | O(log n)| O(1)平均, O(n)最坏          |
| 遍历顺序      | 有序    | 无序                        |
| 内存使用      | 较少    | 较多(哈希表需要额外空间)    |

**选择建议**：
1. 需要元素有序 → 选择map/set
2. 需要最高查找性能 → 选择unordered_map/unordered_set
3. 需要键重复 → 选择multimap/multiset
4. 内存受限 → 考虑map/set

## 六、高级技巧与最佳实践

### 1. 自定义比较函数

```cpp
struct CaseInsensitiveCompare {
    bool operator()(const std::string& a, const std::string& b) const {
        return std::lexicographical_compare(
            a.begin(), a.end(),
            b.begin(), b.end(),
            [](char c1, char c2) {
                return tolower(c1) < tolower(c2);
            });
    }
};

std::map<std::string, int, CaseInsensitiveCompare> caseInsensitiveMap;
```

### 2. 高效合并两个map

```cpp
template<typename Map>
void merge_maps(Map& dest, const Map& source) {
    dest.insert(source.begin(), source.end());
}

// C++17更高效的方式
template<typename Map>
void merge_maps(Map& dest, Map&& source) {
    dest.merge(std::move(source));
}
```

### 3. 使用结构化绑定(C++17)

```cpp
std::map<std::string, int> m{{"apple", 5}, {"banana", 3}};

for (const auto& [key, value] : m) {
    std::cout << key << ": " << value << '\n';
}

if (auto [it, inserted] = m.insert({"pear", 4}); inserted) {
    std::cout << "Inserted new element\n";
}
```

### 4. 提取节点(C++17)

```cpp
std::map<int, std::string> m{{1, "one"}, {2, "two"}};
auto node = m.extract(1);  // 提取而不分配/释放内存

if (!node.empty()) {
    node.key() = 3;        // 可以修改键而不重新分配
    m.insert(std::move(node));
}
```

## 七、常见问题与解决方案

**Q1: map的[]操作符和insert有什么区别？**
- `operator[]`会先构造默认值再赋值，可能效率较低
- `operator[]`在键不存在时会插入新元素
- `insert`在键已存在时不会修改值

**Q2: 如何高效地遍历并删除元素？**
```cpp
for (auto it = m.begin(); it != m.end(); ) {
    if (should_remove(*it)) {
        it = m.erase(it);  // erase返回下一个有效迭代器
    } else {
        ++it;
    }
}
```

**Q3: 为什么map的键是const的？**
- 因为修改键会影响红黑树的排序结构
- 如果需要修改键，应先提取节点再修改

**Q4: 如何实现大小写不敏感的map？**
- 提供自定义比较函数(如前文示例)
- 或存储时将键统一转换为小写/大写