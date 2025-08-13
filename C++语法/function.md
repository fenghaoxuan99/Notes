
# C++函数对象工具完整指南

## 1. std::function - 通用函数包装器

### 基本概念
```cpp
template< class R, class... Args >
class function<R(Args...)>;
```
std::function是通用多态函数封装器，可以存储、复制和调用任何可调用目标。

### 核心功能
- 存储普通函数
- 存储lambda表达式
- 存储bind表达式
- 存储成员函数指针
- 存储函数对象

### 使用示例
```cpp
#include <iostream>
#include <functional>

// 普通函数
void print_num(int i) { std::cout << i << '\n'; }

// 函数对象
struct PrintNum {
    void operator()(int i) const { std::cout << i << '\n'; }
};

// 类成员函数
struct Foo {
    void print_add(int i) const { std::cout << num_ + i << '\n'; }
    int num_ = 10;
};

int main() {
    // 1. 存储普通函数
    std::function<void(int)> f1 = print_num;
    f1(42);  // 输出: 42

    // 2. 存储lambda表达式
    std::function<void()> f2 = []() { print_num(123); };
    f2();  // 输出: 123

    // 3. 存储函数对象
    std::function<void(int)> f3 = PrintNum();
    f3(456);  // 输出: 456

    // 4. 存储成员函数
    std::function<void(const Foo&, int)> f4 = &Foo::print_add;
    Foo foo;
    f4(foo, 1);  // 输出: 11

    // 5. 存储成员变量
    std::function<int(const Foo&)> f5 = &Foo::num_;
    std::cout << f5(foo) << '\n';  // 输出: 10
}
```

## 2. std::bind - 函数参数绑定器

### 基本概念
```cpp
template< class F, class... Args >
/*unspecified*/ bind( F&& f, Args&&... args );
```
生成f的转发调用包装器，可以绑定部分参数或调整参数顺序。

### 核心功能
- 参数绑定
- 参数顺序调整
- 成员函数绑定
- 嵌套绑定

### 使用示例
```cpp
#include <functional>
#include <iostream>

void print_sum(int a, int b, int c) {
    std::cout << a + b + c << '\n';
}

struct Point {
    int x, y;
    void print() const { std::cout << x << ", " << y << '\n'; }
};

int main() {
    using namespace std::placeholders;  // 引入占位符 _1, _2, _3...

    // 1. 基本参数绑定
    auto f1 = std::bind(print_sum, 1, 2, 3);
    f1();  // 输出: 6

    // 2. 使用占位符
    auto f2 = std::bind(print_sum, _1, _2, 10);
    f2(5, 6);  // 输出: 21 (5+6+10)

    // 3. 调整参数顺序
    auto f3 = std::bind(print_sum, _3, _1, _2);
    f3(1, 2, 3);  // 输出: 6 (3+1+2)

    // 4. 绑定成员函数
    Point p{10, 20};
    auto f4 = std::bind(&Point::print, _1);
    f4(p);  // 输出: 10, 20

    // 5. 绑定成员变量
    auto f5 = std::bind(&Point::x, _1);
    std::cout << f5(p) << '\n';  // 输出: 10

    // 6. 嵌套绑定
    auto f6 = std::bind(print_sum, 
        std::bind(&Point::x, _1),
        std::bind(&Point::y, _1),
        100);
    f6(p);  // 输出: 130 (10+20+100)
}
```

## 3. std::mem_fn - 成员函数包装器

### 基本概念
```cpp
template< class M, class T >
/*unspecified*/ mem_fn(M T::* pm) noexcept;
```
将成员函数或成员变量包装成可调用对象。

### 核心功能
- 成员函数包装
- 成员变量包装
- 与STL算法配合使用
- 支持多种调用方式

### 使用示例
```cpp
#include <functional>
#include <iostream>
#include <vector>
#include <algorithm>

struct Widget {
    void draw() const { std::cout << "Drawing widget " << id << '\n'; }
    int get_id() const { return id; }
    int id;
};

int main() {
    std::vector<Widget> widgets{{1}, {2}, {3}};

    // 1. 包装成员函数
    auto draw = std::mem_fn(&Widget::draw);
    for (const auto& w : widgets) {
        draw(w);  // 分别输出: Drawing widget 1, 2, 3
    }

    // 2. 与STL算法配合
    std::for_each(widgets.begin(), widgets.end(), 
        std::mem_fn(&Widget::draw));

    // 3. 包装成员变量
    auto get_id = std::mem_fn(&Widget::id);
    for (const auto& w : widgets) {
        std::cout << get_id(w) << ' ';  // 输出: 1 2 3
    }
    std::cout << '\n';

    // 4. 多种调用方式
    Widget w{42};
    auto fn = std::mem_fn(&Widget::draw);
    
    fn(w);          // 通过对象调用
    fn(&w);         // 通过指针调用
    fn(std::ref(w));// 通过引用包装器调用
}
```

## 4. 相关类型特性

### std::is_bind_expression
检查类型是否为bind表达式结果。

```cpp
#include <functional>
#include <type_traits>

int main() {
    auto bound = std::bind([](int x) { return x * 2; }, 10);
    
    if constexpr (std::is_bind_expression_v<decltype(bound)>) {
        std::cout << "This is a bind expression\n";
    }
    
    return 0;
}
```

### std::is_placeholder
检查类型是否为占位符。

```cpp
#include <functional>
#include <iostream>

int main() {
    using namespace std::placeholders;
    
    std::cout << "_1 is placeholder " 
              << std::is_placeholder<decltype(_1)>::value << '\n';
    std::cout << "_2 is placeholder " 
              << std::is_placeholder<decltype(_2)>::value << '\n';
    
    return 0;
}
```

## 5. 综合比较

| 特性            | std::function | std::bind | std::mem_fn |
|----------------|---------------|-----------|-------------|
| 包装普通函数     | ✓             | ✓         | ✗           |
| 包装成员函数     | ✓             | ✓         | ✓           |
| 包装成员变量     | ✓             | ✓         | ✓           |
| 参数绑定        | ✗             | ✓         | ✗           |
| 参数顺序调整    | ✗             | ✓         | ✗           |
| STL算法友好     | ✓             | ✗         | ✓           |
| 调用时传对象    | 可选          | 可选       | 必须        |

## 6. 最佳实践建议

1. **优先使用lambda**：在C++14及以上版本中，lambda通常比bind更清晰
2. **需要存储函数时用function**：当需要类型擦除存储可调用对象时
3. **与STL算法配合用mem_fn**：处理对象容器时更简洁
4. **复杂参数绑定用bind**：需要部分绑定或调整参数顺序时

```cpp
// 示例：综合使用
#include <functional>
#include <vector>
#include <algorithm>

struct Employee {
    std::string name;
    int salary;
    void print() const { 
        std::cout << name << ": " << salary << '\n'; 
    }
};

void process_employees(const std::vector<Employee>& emps, 
                      std::function<bool(int)> filter) {
    std::for_each(emps.begin(), emps.end(), 
        [&filter](const Employee& e) {
            if (filter(e.salary)) {
                e.print();
            }
        });
}

int main() {
    std::vector<Employee> employees{
        {"Alice", 80000},
        {"Bob", 60000},
        {"Charlie", 120000}
    };

    // 使用bind创建过滤器
    auto above = std::bind(std::greater<int>(), _1, 70000);
    process_employees(employees, above);

    // 使用mem_fn简化成员访问
    std::vector<int> salaries;
    std::transform(employees.begin(), employees.end(),
                  std::back_inserter(salaries),
                  std::mem_fn(&Employee::salary));
}
```