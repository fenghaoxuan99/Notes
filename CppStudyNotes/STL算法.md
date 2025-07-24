# C++ STL算法全面指南

## 1. 概述

STL(Standard Template Library)算法是C++标准库中一组强大的工具，用于对容器中的元素执行各种操作。这些算法通过迭代器与容器交互，提供了一种通用且高效的方式来处理数据。

## 2. 遍历算法

### 2.1 for_each函数

**功能**：对容器中的每个元素执行指定操作

**原型**：
```cpp
for_each(iterator begin, iterator end, _func);
```

**参数**：
- `begin`：容器起始迭代器
- `end`：容器结束迭代器
- `_func`：函数指针、函数对象或lambda表达式

**示例**：
```cpp
#include <vector>
#include <algorithm>
#include <iostream>

void print(int val) {
    std::cout << val << " ";
}

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::for_each(v.begin(), v.end(), print);  // 输出: 1 2 3 4 5
    return 0;
}
```

### 2.2 transform函数

**功能**：将源容器中的元素转换后存储到目标容器

**原型**：
```cpp
transform(iterator beg1, iterator end1, iterator beg2, _func);
```

**注意事项**：
- 目标容器需要提前分配足够空间（使用resize()）

**示例**：
```cpp
#include <vector>
#include <algorithm>
#include <iostream>

int square(int val) {
    return val * val;
}

int main() {
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::vector<int> v2(v1.size());
    
    std::transform(v1.begin(), v1.end(), v2.begin(), square);
    
    for (int num : v2) {
        std::cout << num << " ";  // 输出: 1 4 9 16 25
    }
    return 0;
}
```

## 3. 查找算法

### 3.1 find函数

**功能**：在容器中查找指定值

**原型**：
```cpp
find(iterator beg, iterator end, value);
```

**自定义类型查找**：需要重载==运算符

**示例**：
```cpp
#include <vector>
#include <algorithm>
#include <iostream>

int main() {
    std::vector<int> v = {10, 20, 30, 40, 50};
    auto it = std::find(v.begin(), v.end(), 30);
    
    if (it != v.end()) {
        std::cout << "Found: " << *it << std::endl;  // 输出: Found: 30
    } else {
        std::cout << "Not found" << std::endl;
    }
    return 0;
}
```

### 3.2 find_if函数

**功能**：按条件查找元素

**原型**：
```cpp
find_if(iterator beg, iterator end, _Pred);
```

**示例**：
```cpp
#include <vector>
#include <algorithm>
#include <iostream>

bool isEven(int num) {
    return num % 2 == 0;
}

int main() {
    std::vector<int> v = {1, 3, 5, 7, 8, 9};
    auto it = std::find_if(v.begin(), v.end(), isEven);
    
    if (it != v.end()) {
        std::cout << "First even number: " << *it << std::endl;  // 输出: First even number: 8
    }
    return 0;
}
```

### 3.3 binary_search函数

**功能**：二分查找（容器必须有序）

**原型**：
```cpp
bool binary_search(iterator beg, iterator end, value);
```

**示例**：
```cpp
#include <vector>
#include <algorithm>
#include <iostream>

int main() {
    std::vector<int> v = {10, 20, 30, 40, 50};
    bool found = std::binary_search(v.begin(), v.end(), 30);
    
    std::cout << (found ? "Found" : "Not found") << std::endl;  // 输出: Found
    return 0;
}
```

## 4. 排序算法

### 4.1 sort函数

**功能**：对容器元素进行排序

**原型**：
```cpp
sort(iterator beg, iterator end, _Pred);
```

**示例**：
```cpp
#include <vector>
#include <algorithm>
#include <iostream>

int main() {
    std::vector<int> v = {5, 3, 1, 4, 2};
    
    // 升序排序
    std::sort(v.begin(), v.end());
    for (int num : v) {
        std::cout << num << " ";  // 输出: 1 2 3 4 5
    }
    std::cout << std::endl;
    
    // 降序排序
    std::sort(v.begin(), v.end(), std::greater<int>());
    for (int num : v) {
        std::cout << num << " ";  // 输出: 5 4 3 2 1
    }
    return 0;
}
```

### 4.2 random_shuffle函数

**功能**：随机打乱元素顺序

**原型**：
```cpp
random_shuffle(iterator beg, iterator end);
```

**示例**：
```cpp
#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <cstdlib>

int main() {
    std::srand(std::time(0));  // 设置随机种子
    std::vector<int> v = {1, 2, 3, 4, 5};
    
    std::random_shuffle(v.begin(), v.end());
    
    for (int num : v) {
        std::cout << num << " ";  // 输出随机顺序，如: 3 1 5 2 4
    }
    return 0;
}
```

## 5. 替换算法

### 5.1 replace函数

**功能**：替换指定值

**原型**：
```cpp
replace(iterator beg, iterator end, oldvalue, newvalue);
```

**示例**：
```cpp
#include <vector>
#include <algorithm>
#include <iostream>

int main() {
    std::vector<int> v = {1, 2, 3, 2, 5};
    
    std::replace(v.begin(), v.end(), 2, 99);
    
    for (int num : v) {
        std::cout << num << " ";  // 输出: 1 99 3 99 5
    }
    return 0;
}
```

### 5.2 replace_if函数

**功能**：按条件替换元素

**原型**：
```cpp
replace_if(iterator beg, iterator end, pred, newvalue);
```

**示例**：
```cpp
#include <vector>
#include <algorithm>
#include <iostream>

bool isOdd(int num) {
    return num % 2 != 0;
}

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    
    std::replace_if(v.begin(), v.end(), isOdd, 0);
    
    for (int num : v) {
        std::cout << num << " ";  // 输出: 0 2 0 4 0
    }
    return 0;
}
```

## 6. 高级应用技巧

### 6.1 使用lambda表达式简化代码

```cpp
#include <vector>
#include <algorithm>
#include <iostream>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    
    // 使用lambda表达式作为谓词
    auto it = std::find_if(v.begin(), v.end(), [](int num) {
        return num > 3;
    });
    
    if (it != v.end()) {
        std::cout << "First number > 3: " << *it << std::endl;  // 输出: First number > 3: 4
    }
    
    // 使用lambda表达式进行转换
    std::vector<int> squares(v.size());
    std::transform(v.begin(), v.end(), squares.begin(), [](int num) {
        return num * num;
    });
    
    for (int num : squares) {
        std::cout << num << " ";  // 输出: 1 4 9 16 25
    }
    return 0;
}
```