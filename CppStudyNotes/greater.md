
# C++比较函数对象：std::greater和std::less

## 概述

std::greater和std::less是C++标准库中定义在`<functional>`头文件中的函数对象类模板，用于比较两个值的大小关系。它们通常用作算法（如sort、priority_queue等）的比较器。

## 详细说明

### std::less

#### 定义
```cpp
template <class T = void>
struct less;
```

#### operator()实现
```cpp
constexpr bool operator()(const T &lhs, const T &rhs) const 
{
    return lhs < rhs;
}
```
**功能**：检查lhs是否**小于**rhs

### std::greater

#### 定义
```cpp
template <class T = void>
struct greater;
```

#### operator()实现
```cpp
constexpr bool operator()(const T &lhs, const T &rhs) const 
{
    return lhs > rhs;
}
```
**功能**：检查lhs是否**大于**rhs

## 使用场景

1. 作为标准库算法的比较器
2. 定义容器的排序规则
3. 优先队列(priority_queue)的排序规则
4. 需要自定义比较操作的任何场景

## 使用方法与示例

### 基本使用示例
```cpp
#include <functional>
#include <iostream>

int main() {
    std::less<int> less_compare;
    std::greater<int> greater_compare;
    
    std::cout << std::boolalpha;
    std::cout << "5 < 10: " << less_compare(5, 10) << '\n';
    std::cout << "5 > 10: " << greater_compare(5, 10) << '\n';
}
```

### 与STL算法结合使用
```cpp
#include <algorithm>
#include <vector>
#include <functional>
#include <iostream>

int main() {
    std::vector<int> v{5, 2, 8, 1, 9};
    
    // 使用std::less升序排序
    std::sort(v.begin(), v.end(), std::less<int>());
    for (int n : v) std::cout << n << ' ';
    std::cout << '\n';
    
    // 使用std::greater降序排序
    std::sort(v.begin(), v.end(), std::greater<int>());
    for (int n : v) std::cout << n << ' ';
    std::cout << '\n';
}
```

### 在priority_queue中使用
```cpp
#include <queue>
#include <functional>
#include <iostream>

int main() {
    // 默认是最大堆(使用std::less)
    std::priority_queue<int, std::vector<int>, std::less<int>> max_heap;
    
    // 最小堆(使用std::greater)
    std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap;
    
    for(int n : {1, 8, 5, 6, 3}) {
        max_heap.push(n);
        min_heap.push(n);
    }
    
    std::cout << "Max heap: ";
    while(!max_heap.empty()) {
        std::cout << max_heap.top() << ' ';
        max_heap.pop();
    }
    std::cout << '\n';
    
    std::cout << "Min heap: ";
    while(!min_heap.empty()) {
        std::cout << min_heap.top() << ' ';
        min_heap.pop();
    }
    std::cout << '\n';
}
```

### 通用比较器模板示例
```cpp
#include <functional>
#include <iostream>
 
template <typename A, typename B, typename U = std::less<>>
bool compare(A a, B b, U u = U()) {
    return u(a, b);
}
 
int main() {
    std::cout << std::boolalpha;   
    std::cout << "5 < 20: " << compare(5, 20) << '\n';         // 使用默认std::less
    std::cout << "5 > 20: " << compare(5, 20, std::greater<>()) << '\n';
    std::cout << "3.14 < 3: " << compare(3.14, 3) << '\n';     // 不同类型比较
}
```

## 高级特性

1. **透明比较器(C++14)**：
   可以使用`std::less<>`或`std::greater<>`而不指定类型，支持不同类型的比较
   ```cpp
   std::less<> cmp;  // 透明比较器
   bool result = cmp(5, 5.5);  // 比较int和double
   ```

2. **自定义类型比较**：
   对于自定义类型，需要重载`operator<`或`operator>`才能使用std::less或std::greater
   ```cpp
   struct Point {
       int x, y;
       bool operator<(const Point& other) const {
           return x < other.x || (x == other.x && y < other.y);
       }
   };
   
   std::vector<Point> points{{1,2}, {3,4}, {1,1}};
   std::sort(points.begin(), points.end(), std::less<Point>());
   ```

## 性能考虑

1. 函数对象通常比函数指针更高效，因为它们更容易被内联
2. 透明比较器(std::less<>)可以避免不必要的类型转换
3. 在性能关键代码中，直接使用`<`或`>`可能比函数对象调用略快

## 总结对比表

| 特性        | std::less | std::greater |
|------------|----------|-------------|
| 比较操作    | lhs < rhs | lhs > rhs |
| 默认排序    | 升序      | 降序       |
| 优先队列    | 最大堆    | 最小堆     |
| 透明比较器  | std::less<> | std::greater<> |