
# std::forward_list 完整指南
## 1. 概述
`std::forward_list` 是 C++11 引入的单向链表容器，定义在 `<forward_list>` 头文件中。
### 基本特性
```cpp
template<
    class T,
    class Allocator = std::allocator<T>
> class forward_list;
```
- 单向链表实现
- 仅支持前向迭代
- 每个元素只存储指向下一个元素的指针
- 比 `std::list` 更节省内存（每个节点少一个指针）
- 不支持 `size()` 操作（C++11 起）
- 时间复杂度：
  - 插入/删除：O(1)
  - 随机访问：O(n)

## 2. 基本操作

### 2.1 构造与赋值

#### 构造函数
```cpp
// 默认构造
forward_list<int> list1;

// 带初始大小的构造
forward_list<int> list2(5);  // 5个默认初始化的int

// 带初始值和数量的构造
forward_list<int> list3(5, 10);  // 5个值为10的int

// 从迭代器范围构造
std::vector<int> vec{1,2,3,4,5};
forward_list<int> list4(vec.begin(), vec.end());

// 从初始化列表构造
forward_list<int> list5{1,2,3,4,5};

// 拷贝构造
forward_list<int> list6(list5);

// 移动构造
forward_list<int> list7(std::move(list6));
```

#### 赋值操作
```cpp
forward_list<int> list = {1,2,3};

// 拷贝赋值
forward_list<int> list8 = list;

// 移动赋值
forward_list<int> list9 = std::move(list8);

// 初始化列表赋值
list = {4,5,6};

// assign方法
list.assign(3, 7);       // 3个7
list.assign(vec.begin(), vec.end());  // 从迭代器范围
list.assign({8,9,10});   // 从初始化列表
```

### 2.2 元素访问

```cpp
forward_list<int> flist = {1,2,3};

// 访问第一个元素
int first = flist.front();  // 1

// 没有back()方法，因为是单向链表
```

### 2.3 迭代器

```cpp
forward_list<int> flist = {1,2,3,4,5};

// 前向迭代
for (auto it = flist.begin(); it != flist.end(); ++it) {
    std::cout << *it << " ";
}

// 常量迭代器
for (auto it = flist.cbegin(); it != flist.cend(); ++it) {
    // *it = 10;  // 错误，不能修改
}

// C++11起支持
for (int val : flist) {
    std::cout << val << " ";
}
```

## 3. 插入操作

### 3.1 `insert_after` 系列

```cpp
forward_list<int> flist = {1,2,3};

// 在指定位置后插入单个值
auto it = flist.insert_after(flist.before_begin(), 0); 
// flist: 0,1,2,3

// 插入多个相同值
flist.insert_after(it, 2, -1); 
// flist: 0,-1,-1,1,2,3

// 从迭代器范围插入
std::vector<int> vec{10,20};
flist.insert_after(it, vec.begin(), vec.end());
// flist: 0,10,20,-1,-1,1,2,3

// 从初始化列表插入
flist.insert_after(it, {30,40});
// flist: 0,30,40,10,20,-1,-1,1,2,3
```

### 3.2 `emplace_after`

```cpp
struct Point {
    int x, y;
    Point(int a, int b) : x(a), y(b) {}
};

forward_list<Point> points;
points.emplace_after(points.before_begin(), 1, 2);
// 相当于就地构造 Point(1, 2)
```

### 3.3 `push_front` 和 `emplace_front`

```cpp
forward_list<int> flist;

// 在头部插入
flist.push_front(1);      // 拷贝插入
flist.push_front(2);      // flist: 2,1

int x = 3;
flist.push_front(std::move(x));  // 移动插入

// 就地构造
flist.emplace_front(4);   // flist: 4,2,1
```

## 4. 删除操作

### 4.1 `erase_after`

```cpp
forward_list<int> flist = {1,2,3,4,5};

// 删除单个元素
auto it = flist.begin();  // 指向1
it = flist.erase_after(it);  // 删除2，it现在指向3
// flist: 1,3,4,5

// 删除范围
flist.erase_after(flist.before_begin(), it); 
// 删除从头部到it(3)之前的所有元素
// flist: 3,4,5
```

### 4.2 `pop_front`

```cpp
forward_list<int> flist = {1,2,3};
flist.pop_front();  // flist: 2,3
```

### 4.3 `clear`

```cpp
forward_list<int> flist = {1,2,3};
flist.clear();  // 清空链表
```

## 5. 特殊操作

### 5.1 `splice_after` - 链表拼接

```cpp
forward_list<int> list1 = {1,2,3};
forward_list<int> list2 = {4,5,6};

// 移动整个list2到list1的开头
list1.splice_after(list1.before_begin(), list2);
// list1: 4,5,6,1,2,3
// list2: 空

forward_list<int> list3 = {7,8,9};
// 移动list3的单个元素(8)到list1的第二个位置
auto it = list3.begin();  // 指向7
list1.splice_after(std::next(list1.begin()), list3, it);
// list1: 4,5,8,6,1,2,3
// list3: 7,9

forward_list<int> list4 = {10,11,12,13};
// 移动list4的范围到list1的末尾
auto it1 = list4.begin();  // 指向10
auto it2 = std::next(it1, 2);  // 指向12
list1.splice_after(std::next(list1.begin(), 5), list4, it1, it2);
// list1: 4,5,8,6,1,2,11,3
// list4: 10,12,13
```

### 5.2 `merge` - 合并有序链表

```cpp
forward_list<int> list1 = {1,3,5};
forward_list<int> list2 = {2,4,6};

list1.merge(list2);  // 两个链表必须已排序
// list1: 1,2,3,4,5,6
// list2: 空
```

### 5.3 `sort` - 排序

```cpp
forward_list<int> flist = {3,1,4,2,5};
flist.sort();  // 升序排序: 1,2,3,4,5

// 自定义排序
flist.sort(std::greater<int>());  // 降序排序: 5,4,3,2,1
```

### 5.4 `unique` - 删除连续重复元素

```cpp
forward_list<int> flist = {1,1,2,3,3,3,2,1};
flist.sort();       // 先排序: 1,1,1,2,2,3,3,3
flist.unique();     // 去重: 1,2,3

// 自定义唯一性条件
flist = {1,2,3,4,5};
flist.unique([](int a, int b) { return b == a+1; });
// 删除连续递增1的元素: 1,3,5
```

### 5.5 `remove` 和 `remove_if`

```cpp
forward_list<int> flist = {1,2,3,4,5};

// 移除所有等于3的元素
flist.remove(3);  // 1,2,4,5

// 移除所有偶数
flist.remove_if([](int n) { return n%2 == 0; });  // 1,5
```

## 6. 容量操作

```cpp
forward_list<int> flist = {1,2,3};

// 检查是否为空
bool empty = flist.empty();  // false

// 没有size()方法，因为计算size是O(n)操作
// 如果需要大小，可以:
size_t count = std::distance(flist.begin(), flist.end());  // 3

// 调整链表大小
flist.resize(5);      // 扩展到5个元素，新增元素默认初始化: 1,2,3,0,0
flist.resize(2);      // 缩小到2个元素: 1,2
flist.resize(4, 10);  // 扩展到4个元素，新增元素初始化为10: 1,2,10,10
```

## 7. 实际应用示例

### 7.1 高效处理大型数据集

```cpp
// 处理大型数据集时，forward_list比vector更节省内存
forward_list<LargeData> processData(const vector<LargeData>& input) {
    forward_list<LargeData> result;
    auto it = result.before_begin();
    
    for (const auto& data : input) {
        if (shouldKeep(data)) {
            it = result.insert_after(it, data);
        }
    }
    
    return result;
}
```

### 7.2 实现简单的LRU缓存

```cpp
template<typename K, typename V>
class LRUCache {
    using Pair = std::pair<K, V>;
    forward_list<Pair> cache;
    size_t capacity;
    
public:
    LRUCache(size_t cap) : capacity(cap) {}
    
    V* get(const K& key) {
        auto prev = cache.before_begin();
        auto curr = cache.begin();
        
        while (curr != cache.end()) {
            if (curr->first == key) {
                // 找到元素，移动到链表头部
                auto node = *curr;
                cache.erase_after(prev);
                cache.push_front(node);
                return &cache.front().second;
            }
            ++prev;
            ++curr;
        }
        return nullptr;
    }
    
    void put(const K& key, const V& value) {
        // 如果已存在，先删除
        if (get(key)) {
            cache.pop_front();  // get已经把它移到头部了
        }
        
        // 插入新元素到头部
        cache.push_front({key, value});
        
        // 如果超出容量，删除最久未使用的
        if (std::distance(cache.begin(), cache.end()) > capacity) {
            auto it = cache.before_begin();
            std::advance(it, capacity - 1);
            cache.erase_after(it);
        }
    }
};
```

## 8. 性能考虑与最佳实践

1. **内存效率**：forward_list比list更节省内存，特别是对于小元素类型
2. **插入/删除性能**：在已知位置的操作是O(1)，但查找位置可能是O(n)
3. **缓存友好性**：不如vector等连续存储容器
4. **使用场景**：
   - 需要频繁在任意位置插入/删除
   - 不需要随机访问
   - 内存受限环境
   - 不需要反向遍历

5. **避免的陷阱**：
   - 不要频繁计算size()
   - 注意迭代器失效规则：
     - 插入操作不会使任何迭代器失效
     - 删除操作只会使被删除元素的迭代器失效

## 9. 与其它容器的比较

| 特性                | forward_list | list | vector | deque |
|---------------------|-------------|------|--------|-------|
| 内存布局            | 单向链表    | 双向链表 | 连续数组 | 分块数组 |
| 随机访问            | 不支持      | 不支持 | O(1)   | O(1)  |
| 头部插入/删除       | O(1)        | O(1) | O(n)   | O(1)  |
| 中间插入/删除       | O(1)*       | O(1) | O(n)   | O(n)  |
| 尾部插入/删除       | O(n)        | O(1) | O(1)   | O(1)  |
| 内存开销(每个元素)  | 1指针       | 2指针 | 无额外 | 多指针 |
| 迭代器类型          | 前向        | 双向 | 随机   | 随机  |

*注：虽然插入/删除本身是O(1)，但找到位置可能是O(n)
