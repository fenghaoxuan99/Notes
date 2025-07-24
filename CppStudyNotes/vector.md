# C++ STL Vector 全面指南

## 一、STL 六大组件概述

STL (Standard Template Library) 是 C++ 标准库的核心组成部分，包含六大组件：

1. **容器 (Containers)**：模板化的数据结构，用于存储和管理数据集合
   - 序列容器：`vector`, `string`, `deque`, `list`
   - 关联容器：`set`, `map`, `multiset`, `multimap`
   - 无序容器：`unordered_set`, `unordered_map` (C++11)

2. **算法 (Algorithms)**：操作容器中元素的函数模板
   - 排序：`sort`, `stable_sort`
   - 查找：`find`, `binary_search`
   - 修改：`copy`, `transform`, `replace`
   - 遍历：`for_each`

3. **迭代器 (Iterators)**：容器和算法之间的桥梁
   - 输入迭代器、输出迭代器
   - 前向迭代器、双向迭代器、随机访问迭代器

4. **配接器 (Adapters)**：修改组件接口的包装器
   - 容器适配器：`stack`, `queue`, `priority_queue`
   - 迭代器适配器：反向迭代器
   - 函数适配器：`bind`, `function` (C++11)

5. **仿函数 (Functors)**：行为类似函数的对象
   - 算术类：`plus`, `minus`
   - 关系类：`less`, `greater`
   - 逻辑类：`logical_and`, `logical_or`

6. **空间配置器 (Allocators)**：内存管理的底层机制
   - 负责内存分配和释放
   - 默认使用 `std::allocator`

## 二、Vector 容器详解

### 1. 构造函数 

Vector 提供了多种构造方式：

```cpp
#include <vector>
#include <iostream>

void printVector(const std::vector<int>& v) {
    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

int main() {
    // 1. 默认构造
    std::vector<int> v1;
    
    // 2. 范围构造
    int arr[] = {1, 2, 3, 4, 5};
    std::vector<int> v2(arr, arr + sizeof(arr)/sizeof(int));
    
    // 3. 填充构造
    std::vector<int> v3(5, 10); // 5个10
    
    // 4. 拷贝构造
    std::vector<int> v4(v3);
    
    // 5. 移动构造 (C++11)
    std::vector<int> v5(std::move(v4));
    
    // 6. 初始化列表构造 (C++11)
    std::vector<int> v6 = {1, 2, 3, 4, 5};
    
    printVector(v1); // 
    printVector(v2); // 1 2 3 4 5 
    printVector(v3); // 10 10 10 10 10 
    printVector(v5); // 10 10 10 10 10 
    printVector(v6); // 1 2 3 4 5 
    
    return 0;
}
```

### 2. 容量与大小操作

| 函数 | 描述 | 时间复杂度 |
|------|------|------------|
| `size()` | 返回元素数量 | O(1) |
| `capacity()` | 返回当前分配的存储容量 | O(1) |
| `empty()` | 检查是否为空 | O(1) |
| `resize(n)` | 调整容器大小为n | O(n) |
| `reserve(n)` | 预留存储空间 | O(n) |
| `shrink_to_fit()` | 减少容量以适应大小 | O(n) |

**示例：**

```cpp
std::vector<int> vec;
std::cout << "初始状态 - size: " << vec.size() 
          << ", capacity: " << vec.capacity() << std::endl;

for (int i = 0; i < 100; ++i) {
    vec.push_back(i);
    if (i % 10 == 0) {
        std::cout << "添加 " << i << " 个元素后 - size: " << vec.size() 
                  << ", capacity: " << vec.capacity() << std::endl;
    }
}

vec.resize(50);
std::cout << "resize(50)后 - size: " << vec.size() 
          << ", capacity: " << vec.capacity() << std::endl;

vec.shrink_to_fit();
std::cout << "shrink_to_fit()后 - size: " << vec.size() 
          << ", capacity: " << vec.capacity() << std::endl;
```

### 3. 元素访问

| 方法 | 描述 | 异常安全 |
|------|------|----------|
| `operator[]` | 随机访问 | 无边界检查 |
| `at()` | 随机访问，带边界检查 | 超出范围抛出`out_of_range` |
| `front()` | 访问第一个元素 | 空vector未定义行为 |
| `back()` | 访问最后一个元素 | 空vector未定义行为 |
| `data()` | 直接访问底层数组 | C++11引入 |

**示例：**

```cpp
std::vector<int> nums = {1, 2, 3, 4, 5};

// 安全访问
try {
    std::cout << "第3个元素: " << nums.at(2) << std::endl;
    std::cout << "第10个元素: " << nums.at(9) << std::endl; // 抛出异常
} catch (const std::out_of_range& e) {
    std::cerr << "错误: " << e.what() << std::endl;
}

// 快速访问（不检查边界）
std::cout << "第一个元素: " << nums.front() << std::endl;
std::cout << "最后一个元素: " << nums.back() << std::endl;
std::cout << "第2个元素: " << nums[1] << std::endl;

// 原始指针访问
int* p = nums.data();
std::cout << "通过指针访问: " << p[3] << std::endl;
```

### 4. 修改操作

#### 插入与删除

| 方法 | 描述 | 时间复杂度 |
|------|------|------------|
| `push_back()` | 末尾添加元素 | 平摊O(1) |
| `pop_back()` | 删除末尾元素 | O(1) |
| `insert()` | 在指定位置插入元素 | O(n) |
| `erase()` | 删除指定位置元素 | O(n) |
| `clear()` | 清空容器 | O(n) |
| `emplace()` | 原位构造插入 | O(n) |
| `emplace_back()` | 末尾原位构造 | 平摊O(1) |

**示例：高效插入**

```cpp
struct Employee {
    std::string name;
    int id;
    double salary;
    
    Employee(std::string n, int i, double s) 
        : name(std::move(n)), id(i), salary(s) {
        std::cout << "构造Employee: " << name << std::endl;
    }
    
    Employee(const Employee& other)
        : name(other.name), id(other.id), salary(other.salary) {
        std::cout << "拷贝Employee: " << name << std::endl;
    }
};

int main() {
    std::vector<Employee> staff;
    
    // 传统方法 - 需要构造临时对象
    staff.push_back(Employee("Alice", 1001, 5000.0));
    
    // 高效方法 - 原位构造
    staff.emplace_back("Bob", 1002, 6000.0);
    
    // 在指定位置插入
    auto it = staff.emplace(staff.begin(), "Charlie", 1003, 5500.0);
    
    // 批量插入
    staff.insert(staff.end(), {
        Employee("David", 1004, 4500.0),
        Employee("Eve", 1005, 5200.0)
    });
    
    // 删除操作
    staff.pop_back(); // 删除Eve
    staff.erase(staff.begin() + 1); // 删除Bob
    
    return 0;
}
```

#### 交换与空间优化

```cpp
void optimizeMemory() {
    std::vector<int> bigVec;
    
    // 填充大量数据
    for (int i = 0; i < 1000000; ++i) {
        bigVec.push_back(i);
    }
    
    std::cout << "初始容量: " << bigVec.capacity() << std::endl;
    
    // 只保留前10个元素
    bigVec.resize(10);
    std::cout << "resize后容量: " << bigVec.capacity() << std::endl;
    
    // 方法1: swap技巧 (C++03)
    std::vector<int>(bigVec).swap(bigVec);
    std::cout << "swap后容量: " << bigVec.capacity() << std::endl;
    
    // 方法2: shrink_to_fit (C++11)
    bigVec.shrink_to_fit();
    std::cout << "shrink_to_fit后容量: " << bigVec.capacity() << std::endl;
}
```

### 5. 迭代器与算法

Vector 支持多种迭代器操作：

```cpp
std::vector<int> nums = {5, 2, 8, 1, 9, 3};

// 1. 常规遍历
for (auto it = nums.begin(); it != nums.end(); ++it) {
    std::cout << *it << " ";
}
std::cout << std::endl;

// 2. 反向遍历
for (auto rit = nums.rbegin(); rit != nums.rend(); ++rit) {
    std::cout << *rit << " ";
}
std::cout << std::endl;

// 3. 使用算法
#include <algorithm>

// 排序
std::sort(nums.begin(), nums.end());

// 查找
auto found = std::find(nums.begin(), nums.end(), 8);
if (found != nums.end()) {
    std::cout << "找到8在位置: " << found - nums.begin() << std::endl;
}

// 遍历算法
std::for_each(nums.begin(), nums.end(), [](int n) {
    std::cout << n * 2 << " ";
});
std::cout << std::endl;
```

## 三、高级特性与最佳实践

### 1. 性能优化技巧

1. **预分配空间**：使用 `reserve()` 减少重新分配次数
   ```cpp
   std::vector<int> data;
   data.reserve(1000); // 预分配1000个元素的空间
   ```

2. **移动语义**：使用 `std::move` 转移大型对象
   ```cpp
   std::vector<std::string> strings;
   std::string largeStr(1000, 'a');
   strings.push_back(std::move(largeStr)); // 移动而非拷贝
   ```

3. **emplace系列**：优先使用 `emplace_back` 而非 `push_back`
   ```cpp
   std::vector<std::pair<int, std::string>> pairs;
   pairs.emplace_back(1, "one"); // 直接构造
   ```

### 2. 常见陷阱

1. **迭代器失效**：
   ```cpp
   std::vector<int> v = {1, 2, 3, 4, 5};
   auto it = v.begin() + 2;
   v.insert(v.begin(), 0); // 可能导致it失效
   // 此时使用*it是未定义行为
   ```

2. **引用失效**：
   ```cpp
   std::vector<int> v = {1, 2, 3};
   int& ref = v[1];
   v.push_back(4); // 可能导致重新分配
   // 此时ref可能引用无效内存
   ```

3. **size_type 与 signed类型混用**：
   ```cpp
   std::vector<int> v(10);
   for (int i = 0; i < v.size() - 11; ++i) { // 危险的无符号运算
       // 当size() < 11时，v.size()-11会变成很大的正数
   }
   ```

### 3. 自定义分配器示例

```cpp
#include <memory>
#include <cstdlib>

template <typename T>
struct CustomAllocator {
    using value_type = T;
    
    CustomAllocator() = default;
    
    template <typename U>
    CustomAllocator(const CustomAllocator<U>&) {}
    
    T* allocate(std::size_t n) {
        std::cout << "分配 " << n << " 个元素" << std::endl;
        return static_cast<T*>(std::malloc(n * sizeof(T)));
    }
    
    void deallocate(T* p, std::size_t n) {
        std::cout << "释放 " << n << " 个元素" << std::endl;
        std::free(p);
    }
};

int main() {
    std::vector<int, CustomAllocator<int>> customVec;
    for (int i = 0; i < 5; ++i) {
        customVec.push_back(i);
    }
    return 0;
}
```

## 四、实际应用案例

### 1. 二维动态数组

```cpp
// 创建5x5的矩阵
std::vector<std::vector<int>> matrix(5, std::vector<int>(5));

// 初始化对角线为1
for (size_t i = 0; i < matrix.size(); ++i) {
    matrix[i][i] = 1;
}

// 锯齿状数组
std::vector<std::vector<int>> jagged;
jagged.push_back(std::vector<int>(1, 1));
jagged.push_back(std::vector<int>(2, 2));
jagged.push_back(std::vector<int>(3, 3));

// 打印锯齿数组
for (const auto& row : jagged) {
    for (int val : row) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}
```

### 2. 高效字符串拼接

```cpp
std::vector<std::string> parts = {"Hello", "world", "from", "C++"};

// 预计算总长度
size_t totalLength = 0;
for (const auto& s : parts) {
    totalLength += s.size();
}

// 预留空间
std::string result;
result.reserve(totalLength + parts.size() - 1); // 加上空格

// 拼接
for (size_t i = 0; i < parts.size(); ++i) {
    if (i != 0) {
        result += " ";
    }
    result += parts[i];
}

std::cout << result << std::endl; // "Hello world from C++"
```

### 3. 数据过滤与转换

```cpp
std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// 过滤偶数
numbers.erase(
    std::remove_if(numbers.begin(), numbers.end(), 
                  [](int n) { return n % 2 == 0; }),
    numbers.end()
);

// 转换为字符串
std::vector<std::string> strNumbers;
std::transform(numbers.begin(), numbers.end(), 
              std::back_inserter(strNumbers),
              [](int n) { return std::to_string(n); });

// 连接字符串
std::ostringstream oss;
std::copy(strNumbers.begin(), strNumbers.end(),
         std::ostream_iterator<std::string>(oss, ", "));

std::cout << "奇数: " << oss.str() << std::endl;
```

## 五、C++20 新特性

### 1. ranges 库支持

```cpp
#include <ranges>

std::vector<int> nums = {5, 2, 8, 1, 9, 3};

// 过滤和转换视图
auto even_squares = nums | std::views::filter([](int n) { return n % 2 == 0; })
                         | std::views::transform([](int n) { return n * n; });

for (int n : even_squares) {
    std::cout << n << " "; // 4 64
}
std::cout << std::endl;
```

### 2. span 视图

```cpp
#include <span>

void processData(std::span<int> data) {
    for (int& n : data) {
        n *= 2;
    }
}

std::vector<int> values = {1, 2, 3, 4, 5};
processData(values); // 传递vector视图

for (int n : values) {
    std::cout << n << " "; // 2 4 6 8 10
}
```

## 总结

Vector 是 C++ 中最常用的容器之一，掌握其特性和最佳实践对编写高效 C++ 代码至关重要。关键点包括：

1. 理解 vector 的内存管理机制和自动扩容策略
2. 熟练使用各种构造和修改操作
3. 掌握迭代器和算法的配合使用
4. 了解性能优化技巧和常见陷阱
5. 关注 C++11/14/17/20 引入的新特性