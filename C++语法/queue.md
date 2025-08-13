# C++队列与优先级队列全面指南

## 1. std::queue 队列容器

### 1.1 基本概念

`std::queue` 是 C++ 标准库中的容器适配器，提供先进先出(FIFO)的数据结构功能。

```cpp
template<
    class T,
    class Container = std::deque<T>
> class queue;
```

**核心特性**：
- 先进先出(FIFO)原则
- 队尾插入数据，队头移除数据
- 不支持随机访问和遍历操作
- 默认使用 `std::deque` 作为底层容器

### 1.2 基本操作

| 操作 | 函数原型 | 时间复杂度 | 描述 |
|------|----------|------------|------|
| 插入 | `push(const T& value)` | O(1) | 在队尾插入元素 |
| 移除 | `pop()` | O(1) | 移除队头元素 |
| 访问队头 | `front()` | O(1) | 返回队头元素的引用 |
| 访问队尾 | `back()` | O(1) | 返回队尾元素的引用 |
| 大小 | `size()` | O(1) | 返回队列中元素数量 |
| 判空 | `empty()` | O(1) | 检查队列是否为空 |
| 原地构造 | `emplace(Args&&... args)` | O(1) | 在队尾直接构造元素 |

### 1.3 使用示例

```cpp
#include <iostream>
#include <queue>

int main() {
    std::queue<int> q;
    
    // 插入元素
    q.push(1);
    q.push(2);
    q.push(3);
    
    // 使用emplace原地构造
    q.emplace(4);
    
    std::cout << "队列大小: " << q.size() << std::endl;
    std::cout << "队头元素: " << q.front() << std::endl;
    std::cout << "队尾元素: " << q.back() << std::endl;
    
    // 遍历队列(注意:实际应用中通常不需要遍历队列)
    std::cout << "队列内容: ";
    while (!q.empty()) {
        std::cout << q.front() << " ";
        q.pop();  // 移除队头元素
    }
    std::cout << std::endl;
    
    return 0;
}
```

### 1.4 底层容器选择

`std::queue` 默认使用 `std::deque` 作为底层容器，但也可以指定其他容器：

```cpp
#include <list>

std::queue<int, std::list<int>> list_queue;  // 使用list作为底层容器
```

**可用底层容器要求**：
- 支持 `back()`、`push_back()`、`pop_front()` 操作
- 提供 `size()` 和 `empty()` 方法

## 2. std::priority_queue 优先级队列

### 2.1 基本概念

`std::priority_queue` 是另一种容器适配器，基于堆数据结构实现，提供优先级排序功能。

```cpp
template<
    class T,
    class Container = std::vector<T>,
    class Compare = std::less<typename Container::value_type>
> class priority_queue;
```

**核心特性**：
- 默认最大堆实现(顶部元素最大)
- 插入和删除操作时间复杂度为 O(log n)
- 顶部元素访问时间复杂度为 O(1)
- 不支持直接遍历和随机访问
- 默认使用 `std::vector` 作为底层容器

### 2.2 构造函数

1. **默认构造函数**
```cpp
std::priority_queue<int> pq;  // 最大堆
```

2. **指定比较函数**
```cpp
std::priority_queue<int, std::vector<int>, std::greater<int>> min_pq;  // 最小堆
```

3. **通过迭代器初始化**
```cpp
std::vector<int> data = {3, 1, 4, 1, 5};
std::priority_queue<int> pq(data.begin(), data.end());
```

4. **拷贝构造**
```cpp
std::priority_queue<int> pq1;
pq1.push(10);
std::priority_queue<int> pq2(pq1);  // 拷贝构造
```

### 2.3 成员函数

| 操作 | 函数原型 | 时间复杂度 | 描述 |
|------|----------|------------|------|
| 插入 | `push(const T& value)` | O(log n) | 插入元素并维护堆结构 |
| 原地构造 | `emplace(Args&&... args)` | O(log n) | 直接构造元素 |
| 移除顶部 | `pop()` | O(log n) | 移除顶部元素 |
| 访问顶部 | `top()` | O(1) | 返回顶部元素的常量引用 |
| 判空 | `empty()` | O(1) | 检查是否为空 |
| 大小 | `size()` | O(1) | 返回元素数量 |

### 2.4 使用示例

#### 基本使用
```cpp
#include <iostream>
#include <queue>

int main() {
    std::priority_queue<int> max_pq;
    
    max_pq.push(3);
    max_pq.push(1);
    max_pq.push(4);
    max_pq.emplace(2);
    
    std::cout << "优先级队列大小: " << max_pq.size() << std::endl;
    std::cout << "顶部元素: " << max_pq.top() << std::endl;
    
    std::cout << "元素出队顺序: ";
    while (!max_pq.empty()) {
        std::cout << max_pq.top() << " ";
        max_pq.pop();
    }
    std::cout << std::endl;
    
    return 0;
}
```

#### 最小堆实现
```cpp
#include <queue>
#include <vector>
#include <functional>

std::priority_queue<int, std::vector<int>, std::greater<int>> min_pq;

min_pq.push(5);
min_pq.push(2);
min_pq.push(8);

// 将输出 2 5 8
while (!min_pq.empty()) {
    std::cout << min_pq.top() << " ";
    min_pq.pop();
}
```

#### 自定义比较函数
```cpp
#include <queue>
#include <string>
#include <vector>

struct Task {
    int priority;
    std::string name;
    
    bool operator<(const Task& other) const {
        return priority < other.priority;  // 优先级值大的排在前面
    }
};

int main() {
    std::priority_queue<Task> task_queue;
    
    task_queue.push({3, "低优先级任务"});
    task_queue.push({1, "最低优先级任务"});
    task_queue.push({5, "高优先级任务"});
    
    while (!task_queue.empty()) {
        auto task = task_queue.top();
        std::cout << task.name << " (优先级:" << task.priority << ")\n";
        task_queue.pop();
    }
    
    return 0;
}
```

### 2.5 性能分析与应用场景

**时间复杂度**：
- 插入元素：O(log n)
- 删除顶部元素：O(log n)
- 访问顶部元素：O(1)

**应用场景**：
1. **任务调度系统**：按优先级处理任务
2. **Top K问题**：快速找到数据流中最大的K个元素
3. **图算法**：Dijkstra算法中的优先级队列
4. **事件驱动模拟**：按时间顺序处理事件
5. **数据压缩**：Huffman编码中的频率排序

### 2.6 底层容器选择

默认使用 `std::vector`，但可以替换为支持以下操作的容器：
- `front()`：访问第一个元素
- `push_back()`：尾部插入元素
- `pop_back()`：尾部删除元素
- 随机访问迭代器

**示例**：
```cpp
std::priority_queue<int, std::deque<int>> pq;  // 使用deque作为底层容器
```

## 3. 队列与优先级队列对比

| 特性 | std::queue | std::priority_queue |
|------|------------|---------------------|
| **数据结构** | FIFO队列 | 堆(默认最大堆) |
| **插入时间复杂度** | O(1) | O(log n) |
| **删除时间复杂度** | O(1) | O(log n) |
| **访问顶部元素** | front()/back() | top() |
| **底层容器** | 默认deque | 默认vector |
| **元素顺序** | 插入顺序 | 优先级顺序 |
| **典型应用** | 广度优先搜索、缓冲区 | 任务调度、Top K问题 |

## 4. 高级用法与技巧

### 4.1 使用自定义比较函数

```cpp
struct Person {
    std::string name;
    int age;
};

struct CompareByAge {
    bool operator()(const Person& a, const Person& b) const {
        return a.age < b.age;  // 年龄大的优先级高
    }
};

std::priority_queue<Person, std::vector<Person>, CompareByAge> age_queue;
```

### 4.2 处理指针或智能指针

```cpp
struct Task {
    int priority;
    std::string description;
};

auto cmp = [](const std::shared_ptr<Task>& a, const std::shared_ptr<Task>& b) {
    return a->priority < b->priority;
};

std::priority_queue<
    std::shared_ptr<Task>,
    std::vector<std::shared_ptr<Task>>,
    decltype(cmp)
> task_queue(cmp);
```

### 4.3 实现固定大小的优先级队列

```cpp
template<typename T, typename Compare = std::less<T>>
class fixed_priority_queue {
    std::priority_queue<T, std::vector<T>, Compare> pq;
    size_t max_size;
    
public:
    fixed_priority_queue(size_t size) : max_size(size) {}
    
    void push(const T& value) {
        pq.push(value);
        if (pq.size() > max_size) {
            pq.pop();
        }
    }
    
    // 其他方法委托给底层pq...
};
```

## 5. 常见问题与解决方案

### 5.1 如何遍历优先级队列？

标准库的 `priority_queue` 不支持直接遍历。如果需要遍历：
1. 拷贝队列并逐个弹出元素
2. 直接访问底层容器(需要小心维护堆性质)

```cpp
// 方法1: 拷贝队列
auto temp = pq;
while (!temp.empty()) {
    process(temp.top());
    temp.pop();
}

// 方法2: 继承并暴露底层容器(不推荐)
```

### 5.2 如何修改优先级队列中的元素？

直接修改会破坏堆结构。标准做法：
1. 移除元素
2. 修改元素
3. 重新插入

```cpp
// 假设我们需要修改顶部元素的优先级
auto top = pq.top();
pq.pop();
top.priority = new_priority;
pq.push(top);
```

### 5.3 如何实现稳定的优先级队列？

当多个元素具有相同优先级时，保持它们插入的顺序：

```cpp
template<typename T>
struct StableElement {
    T value;
    size_t sequence;
    
    bool operator<(const StableElement& other) const {
        if (value == other.value) {
            return sequence > other.sequence;  // 后插入的优先级低
        }
        return value < other.value;
    }
};

std::priority_queue<StableElement<int>> stable_pq;
```

## 6. 性能优化建议

1. **预分配内存**：对于已知大小的队列，预先reserve底层容器空间
2. **使用emplace**：避免不必要的拷贝构造
3. **选择合适的底层容器**：
   - `vector`：通常性能最好
   - `deque`：当需要频繁在两端操作时
4. **批量操作**：使用迭代器范围构造函数比逐个插入更高效
5. **自定义分配器**：对于特定内存需求的情况

## 7. 实际应用案例

### 7.1 任务调度系统

```cpp
struct Task {
    enum Priority { Low, Normal, High };
    Priority priority;
    std::function<void()> job;
    std::chrono::system_clock::time_point scheduled_time;
    
    bool operator<(const Task& other) const {
        if (priority != other.priority) {
            return priority < other.priority;
        }
        return scheduled_time > other.scheduled_time;
    }
};

class TaskScheduler {
    std::priority_queue<Task> tasks;
    
public:
    void schedule(Task::Priority p, std::function<void()> job, 
                 std::chrono::seconds delay = std::chrono::seconds(0)) {
        tasks.push({
            p, 
            job, 
            std::chrono::system_clock::now() + delay
        });
    }
    
    void run() {
        while (!tasks.empty()) {
            auto task = tasks.top();
            tasks.pop();
            
            if (task.scheduled_time > std::chrono::system_clock::now()) {
                // 还未到执行时间，重新插入队列
                tasks.push(task);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            
            task.job();
        }
    }
};
```

### 7.2 合并K个有序链表

```cpp
struct ListNode {
    int val;
    ListNode* next;
};

struct CompareNode {
    bool operator()(const ListNode* a, const ListNode* b) {
        return a->val > b->val;  // 最小堆
    }
};

ListNode* mergeKLists(std::vector<ListNode*>& lists) {
    std::priority_queue<ListNode*, std::vector<ListNode*>, CompareNode> pq;
    
    for (auto node : lists) {
        if (node) pq.push(node);
    }
    
    ListNode dummy(0);
    ListNode* tail = &dummy;
    
    while (!pq.empty()) {
        auto node = pq.top();
        pq.pop();
        
        tail->next = node;
        tail = tail->next;
        
        if (node->next) {
            pq.push(node->next);
        }
    }
    
    return dummy.next;
}
```

## 8. 总结

`std::queue` 和 `std::priority_queue` 是 C++ 标准库中两种重要的容器适配器，各有其适用场景：

- **`std::queue`**：
  - 适用于严格的先进先出场景
  - 操作简单高效
  - 常用于广度优先搜索、消息队列等

- **`std::priority_queue`**：
  - 适用于需要按优先级处理元素的场景
  - 提供高效的插入和删除操作
  - 常用于任务调度、图算法、Top K问题等