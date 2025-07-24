# C++ 链表容器深度解析：std::list 和 std::forward_list

## 一、链表容器概述

### 1.1 链表基本概念

链表是一种线性数据结构，由一系列节点组成，每个节点包含数据域和指针域。C++标准库提供了两种链表容器实现：

1. **std::list**：双向链表
2. **std::forward_list**：单向链表

### 1.2 链表与数组的对比

| 特性            | 数组/vector | 链表(list) |
|----------------|------------|-----------|
| 内存布局        | 连续        | 非连续     |
| 随机访问        | O(1)       | O(n)      |
| 插入/删除       | O(n)       | O(1)      |
| 空间开销        | 小         | 较大       |
| 缓存友好性      | 好         | 差        |

## 二、std::list 深度解析

### 2.1 基本结构与特性

```cpp
template<class T, class Allocator = std::allocator<T>> 
class list;
```

- **实现方式**：双向循环链表
- **迭代器类型**：双向迭代器
- **时间复杂度**：
  - 插入/删除：O(1)
  - 随机访问：O(n)

### 2.2 核心操作与示例

#### 2.2.1 元素访问

```cpp
std::list<int> mylist = {1, 2, 3};

// 访问首尾元素
std::cout << "Front: " << mylist.front() << "\n";  // 1
std::cout << "Back: " << mylist.back() << "\n";    // 3

// 迭代器访问
for(auto it = mylist.begin(); it != mylist.end(); ++it) {
    std::cout << *it << " ";
}
```

#### 2.2.2 插入与删除

```cpp
std::list<int> nums = {1, 2, 4};

// 在指定位置前插入
auto it = nums.begin();
std::advance(it, 2);
nums.insert(it, 3);  // 1, 2, 3, 4

// 删除元素
nums.remove(2);      // 删除所有值为2的元素
nums.erase(it);      // 删除迭代器指向的元素
```

#### 2.2.3 splice 操作（高效链表合并）

```cpp
std::list<int> list1 = {1, 2}, list2 = {3, 4};

// 将list2全部元素转移到list1末尾
list1.splice(list1.end(), list2);  
// list1: {1, 2, 3, 4}, list2: 空

// 转移单个元素
std::list<int> list3 = {5};
list1.splice(list1.begin(), list3, list3.begin());
// list1: {5, 1, 2, 3, 4}, list3: 空

// 转移元素范围
std::list<int> list4 = {6, 7, 8};
auto first = list4.begin();
auto last = std::next(first, 2);
list1.splice(list1.end(), list4, first, last);
// list1: {5, 1, 2, 3, 4, 6, 7}, list4: {8}
```

### 2.3 特殊操作

#### 2.3.1 merge（合并有序链表）

```cpp
std::list<int> listA = {1, 3, 5};
std::list<int> listB = {2, 4, 6};

listA.merge(listB);  // 两个链表必须已排序
// listA: {1, 2, 3, 4, 5, 6}, listB: 空
```

#### 2.3.2 sort（链表排序）

```cpp
std::list<int> values = {8, 7, 5, 9, 0, 1};

// 默认升序排序
values.sort();
// 0, 1, 5, 7, 8, 9

// 使用自定义比较函数降序排序
values.sort(std::greater<int>());
// 9, 8, 7, 5, 1, 0
```

#### 2.3.3 unique（去除连续重复）

```cpp
std::list<int> nums = {1, 2, 2, 3, 3, 3, 2};

nums.sort();      // 先排序使相同元素连续
nums.unique();    // 移除连续重复
// 结果: 1, 2, 3
```

## 三、std::forward_list 深度解析

### 3.1 基本结构与特性

```cpp
template<class T, class Allocator = std::allocator<T>> 
class forward_list;
```

- **实现方式**：单向链表
- **迭代器类型**：前向迭代器
- **内存效率**：比std::list更高（每个节点少一个指针）

### 3.2 核心操作与示例

#### 3.2.1 插入操作（insert_after）

```cpp
std::forward_list<std::string> words = {"the", "frogurt", "is", "cursed"};

// 在开始位置后插入
auto it = words.insert_after(words.before_begin(), "strawberry");
// words: ["strawberry", "the", "frogurt", "is", "cursed"]

// 在指定位置后插入多个元素
words.insert_after(it, 2, "strawberry");
// words: ["strawberry", "strawberry", "strawberry", "the", ...]

// 插入范围
std::vector<std::string> fruits = {"apple", "banana"};
words.insert_after(it, fruits.begin(), fruits.end());
```

#### 3.2.2 emplace_after（原地构造）

```cpp
struct Sum {
    std::string desc;
    int value;
    Sum(std::string d, int v) : desc(std::move(d)), value(v) {}
    void print() const { std::cout << desc << " = " << value << '\n'; }
};

std::forward_list<Sum> list;
auto iter = list.before_begin();

for (int i=1, sum=1; i<=5; ++i) {
    std::string expr = (i == 1) ? "1" : expr + " + " + std::to_string(i);
    sum += i;
    iter = list.emplace_after(iter, expr, sum);
}

// 输出:
// 1 = 1
// 1 + 2 = 3
// 1 + 2 + 3 = 6
// 1 + 2 + 3 + 4 = 10
// 1 + 2 + 3 + 4 + 5 = 15
```

## 四、性能分析与最佳实践

### 4.1 时间复杂度对比

| 操作          | std::vector | std::list | std::forward_list |
|--------------|------------|----------|------------------|
| 随机访问      | O(1)       | O(n)     | O(n)             |
| 头部插入/删除 | O(n)       | O(1)     | O(1)             |
| 尾部插入/删除 | O(1)       | O(1)     | O(n)             |
| 中间插入/删除 | O(n)       | O(1)     | O(1)             |

### 4.2 使用场景建议

**使用 std::list 当：**
- 需要频繁在中间位置插入/删除元素
- 需要双向遍历
- 需要稳定的迭代器（元素插入删除不影响其他元素的迭代器）

**使用 std::forward_list 当：**
- 只需要单向遍历
- 极度关注内存使用
- 只需要前向操作

**避免使用链表当：**
- 需要频繁随机访问
- 缓存性能是关键因素
- 存储的是小数据类型（指针开销占比大）

## 五、综合应用示例

### 5.1 LRU缓存实现

```cpp
template<typename K, typename V>
class LRUCache {
    using Pair = std::pair<K, V>;
    std::list<Pair> cache;
    std::unordered_map<K, typename std::list<Pair>::iterator> map;
    size_t capacity;
    
public:
    LRUCache(size_t cap) : capacity(cap) {}
    
    V get(K key) {
        auto it = map.find(key);
        if (it == map.end()) throw std::runtime_error("Key not found");
        
        // 移动到链表头部
        cache.splice(cache.begin(), cache, it->second);
        return it->second->second;
    }
    
    void put(K key, V value) {
        auto it = map.find(key);
        if (it != map.end()) {
            it->second->second = value;
            cache.splice(cache.begin(), cache, it->second);
            return;
        }
        
        if (cache.size() == capacity) {
            // 移除最久未使用的
            auto last = cache.back();
            map.erase(last.first);
            cache.pop_back();
        }
        
        // 插入新元素到头部
        cache.emplace_front(key, value);
        map[key] = cache.begin();
    }
};
```

### 5.2 多项式相加

```cpp
struct Term {
    int coeff;
    int exp;
    Term(int c, int e) : coeff(c), exp(e) {}
};

std::forward_list<Term> addPolynomials(
    std::forward_list<Term>& poly1, 
    std::forward_list<Term>& poly2) {
    
    std::forward_list<Term> result;
    auto it1 = poly1.begin(), it2 = poly2.begin();
    auto res_it = result.before_begin();
    
    while (it1 != poly1.end() && it2 != poly2.end()) {
        if (it1->exp > it2->exp) {
            res_it = result.insert_after(res_it, *it1);
            ++it1;
        } else if (it1->exp < it2->exp) {
            res_it = result.insert_after(res_it, *it2);
            ++it2;
        } else {
            int sum = it1->coeff + it2->coeff;
            if (sum != 0) {
                res_it = result.insert_after(res_it, Term(sum, it1->exp));
            }
            ++it1; ++it2;
        }
    }
    
    // 添加剩余项
    while (it1 != poly1.end()) {
        res_it = result.insert_after(res_it, *it1);
        ++it1;
    }
    while (it2 != poly2.end()) {
        res_it = result.insert_after(res_it, *it2);
        ++it2;
    }
    
    return result;
}
```

## 六、总结

std::list 和 std::forward_list 是C++标准库中重要的顺序容器，它们提供了高效的插入和删除操作，特别适合于以下场景：

1. 需要频繁在任意位置插入/删除元素
2. 不需要随机访问元素
3. 需要稳定的迭代器（元素插入删除不影响其他元素的迭代器）

在实际应用中，应根据具体需求选择合适的链表类型：
- 需要双向遍历时选择 std::list
- 追求极致内存效率时选择 std::forward_list

