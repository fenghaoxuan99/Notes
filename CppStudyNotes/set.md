# C++关联容器：set与multiset全面指南

## 一、关联容器概述

关联容器是C++标准库中重要的数据结构，它们基于键(key)来存储和访问元素。主要关联容器包括：
- `set`：唯一键的集合，按键排序
- `multiset`：键集合，允许重复键，按键排序
- `map`：键值对集合，按键排序
- `multimap`：键值对集合，允许重复键，按键排序

这些容器通常以红黑树实现，提供了高效的查找、插入和删除操作（时间复杂度为O(log n)）。

## 二、set容器详解

### 2.1 基本特性

`std::set`是C++标准模板库(STL)中的关联容器，具有以下特点：
- **元素自动排序（默认升序）**
- 元素唯一（不允许重复）
- 基于红黑树实现
- 查找、插入和删除操作的时间复杂度为O(log n)

### 2.2 构造函数与初始化

```cpp
#include <set>
#include <vector>

int main() {
    // 默认构造函数
    std::set<int> set1;
    
    // 初始化列表构造
    std::set<int> set2 = {1, 3, 5, 7, 9};
    
    // 迭代器范围构造
    std::vector<int> vec = {2, 4, 6, 8};
    std::set<int> set3(vec.begin(), vec.end());
    
    // 自定义比较函数
    struct CaseInsensitiveCompare {
        bool operator()(const std::string& a, const std::string& b) const {
            return std::lexicographical_compare(
                a.begin(), a.end(),
                b.begin(), b.end(),
                [](char c1, char c2) { return tolower(c1) < tolower(c2); });
        }
    };
    
    std::set<std::string, CaseInsensitiveCompare> caseInsensitiveSet;
    caseInsensitiveSet.insert("Apple");
    caseInsensitiveSet.insert("banana");
    caseInsensitiveSet.insert("apple");  // 不会插入，因为"Apple"已存在（不区分大小写）
    
    return 0;
}
```

### 2.3 元素插入操作

`set`提供了多种插入元素的方式：

#### 基本插入

```cpp
std::set<int> mySet;

// 1. insert(value) - 返回pair<iterator, bool>
auto result = mySet.insert(10);
if (result.second) {
    std::cout << "Inserted successfully\n";
}

// 2. insert(hint, value) - 使用提示位置提高插入效率
auto hint = mySet.begin();
mySet.insert(hint, 20);

// 3. 范围插入
std::vector<int> nums = {5, 15, 25};
mySet.insert(nums.begin(), nums.end());

// 4. 初始化列表插入
mySet.insert({30, 40, 50});
```

#### 高效插入(C++11及以上)

```cpp
// emplace - 直接在容器内构造元素
auto res = mySet.emplace(60);
if (res.second) {
    std::cout << "Emplaced successfully\n";
}

// emplace_hint - 带提示位置的原地构造
auto it = mySet.emplace_hint(mySet.end(), 70);
```

### 2.4 元素访问与查找

```cpp
std::set<int> numbers = {10, 20, 30, 40, 50};

// 1. find - 查找元素
auto it = numbers.find(30);
if (it != numbers.end()) {
    std::cout << "Found: " << *it << std::endl;
}

// 2. count - 检查元素是否存在
if (numbers.count(25) > 0) {
    std::cout << "25 exists\n";
} else {
    std::cout << "25 does not exist\n";
}

// 3. lower_bound - 第一个不小于key的元素
auto lb = numbers.lower_bound(25);
if (lb != numbers.end()) {
    std::cout << "Lower bound for 25: " << *lb << std::endl;  // 输出30
}

// 4. upper_bound - 第一个大于key的元素
auto ub = numbers.upper_bound(30);
if (ub != numbers.end()) {
    std::cout << "Upper bound for 30: " << *ub << std::endl;  // 输出40
}

// 5. equal_range - 返回匹配元素的范围
auto range = numbers.equal_range(30);
for (auto i = range.first; i != range.second; ++i) {
    std::cout << *i << " ";  // 输出30
}
```

### 2.5 元素删除操作

```cpp
std::set<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9};

// 1. erase(key) - 删除指定键的元素
size_t removed = data.erase(5);  // 返回删除的元素数量(0或1)

// 2. erase(iterator) - 删除迭代器指向的元素
auto it = data.find(3);
if (it != data.end()) {
    data.erase(it);
}

// 3. erase(range) - 删除范围内的元素
auto first = data.find(6);
auto last = data.find(9);
data.erase(first, last);  // 删除6,7,8

// 4. clear() - 删除所有元素
data.clear();
```

### 2.6 高级特性(C++17及以上)

#### 节点操作

```cpp
std::set<int> setA = {1, 2, 3};
std::set<int> setB = {4, 5, 6};

// 1. 提取节点
auto node = setA.extract(2);

// 2. 检查节点是否为空
if (!node.empty()) {
    std::cout << "Extracted value: " << node.value() << std::endl;
}

// 3. 插入节点到另一个set
setB.insert(std::move(node));

// 结果: setA = {1, 3}, setB = {4, 5, 6, 2}
```

#### 合并容器(C++17)

```cpp
std::set<int> x = {1, 2, 3};
std::set<int> y = {2, 3, 4};

// 合并两个set，重复元素保留在y中
x.merge(y);

// 结果: x = {1, 2, 3, 4}, y = {2, 3}
```

### 2.7 性能优化技巧

1. **有序插入优化**：当需要按顺序插入大量元素时，可以缓存上一次插入的位置作为提示

```cpp
std::set<int> orderedSet;
auto hint = orderedSet.begin();

for (int i = 0; i < 10000; ++i) {
    hint = orderedSet.insert(hint, i);
}
```

2. **批量操作**：优先使用范围插入/删除而不是单元素操作

3. **自定义比较函数**：对于复杂类型，设计高效的比较函数

```cpp
struct Point {
    int x, y;
};

struct PointCompare {
    bool operator()(const Point& a, const Point& b) const {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    }
};

std::set<Point, PointCompare> pointSet;
```

## 三、multiset容器

`std::multiset`与`std::set`类似，但允许存储重复的元素。主要区别在于：

1. 插入操作总是成功（返回迭代器而非pair）
2. count()可能返回大于1的值
3. equal_range()对于重复元素返回有效范围

### 3.1 基本用法示例

```cpp
#include <set>
#include <iostream>

int main() {
    std::multiset<int> ms;
    
    // 插入重复元素
    ms.insert(10);
    ms.insert(20);
    ms.insert(10);  // 允许重复
    
    std::cout << "Count of 10: " << ms.count(10) << std::endl;  // 输出2
    
    // 遍历所有元素
    for (const auto& val : ms) {
        std::cout << val << " ";  // 输出10 10 20
    }
    
    // 删除所有值为10的元素
    ms.erase(10);
    
    return 0;
}
```

### 3.2 查找重复元素

```cpp
std::multiset<int> numbers = {1, 2, 2, 2, 3, 4, 4, 5};

// 查找所有值为2的元素
auto range = numbers.equal_range(2);

std::cout << "Number of 2s: " << std::distance(range.first, range.second) << std::endl;

for (auto it = range.first; it != range.second; ++it) {
    std::cout << *it << " ";  // 输出2 2 2
}
```

## 四、实际应用案例

### 4.1 统计单词出现频率

```cpp
#include <set>
#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>

int main() {
    std::multiset<std::string> words;
    std::string word;
    
    // 从标准输入读取单词
    while (std::cin >> word) {
        words.insert(word);
    }
    
    // 获取所有唯一单词
    std::set<std::string> uniqueWords(words.begin(), words.end());
    
    // 输出每个单词的频率
    for (const auto& w : uniqueWords) {
        std::cout << w << ": " << words.count(w) << std::endl;
    }
    
    return 0;
}
```

### 4.2 维护有序数据集

```cpp
#include <set>
#include <vector>
#include <iostream>
#include <random>

int main() {
    std::vector<int> data(100);
    
    // 生成随机数
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000);
    
    for (auto& num : data) {
        num = dis(gen);
    }
    
    // 将数据插入set自动排序
    std::set<int> sortedData(data.begin(), data.end());
    
    // 输出排序后的数据
    for (const auto& num : sortedData) {
        std::cout << num << " ";
    }
    
    // 查找范围[300, 700]内的元素
    auto low = sortedData.lower_bound(300);
    auto high = sortedData.upper_bound(700);
    
    std::cout << "\nElements in range [300, 700]: ";
    for (auto it = low; it != high; ++it) {
        std::cout << *it << " ";
    }
    
    return 0;
}
```

## 五、最佳实践与注意事项

1. **选择合适的容器**：
   - 需要唯一元素且有序访问：使用`set`
   - 允许重复元素且有序访问：使用`multiset`
   - 需要键值对：使用`map`或`multimap`
   - 只需要检查存在性而不需要顺序：考虑`unordered_set`

2. **自定义比较函数**：
   - 确保比较函数实现严格弱序
   - 对于复杂类型，比较函数应该高效

3. **迭代器失效**：
   - 删除元素会使指向该元素的迭代器失效
   - 其他迭代器通常保持有效

4. **性能考虑**：
   - 避免频繁的单元素插入/删除
   - 预分配空间（对于`unordered_set`更有效）
   - 考虑使用`emplace`代替`insert`以避免不必要的拷贝

5. **线程安全**：
   - STL容器本身不是线程安全的
   - 需要同步机制来保证多线程环境下的安全访问

通过深入理解`set`和`multiset`的特性和使用方法，可以有效地解决许多需要有序、唯一或可重复元素集合的问题场景。