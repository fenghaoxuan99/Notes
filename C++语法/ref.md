# C++引用包装器：std::reference_wrapper, std::ref和std::cref详解

## 1. 概述

在C++中，引用(`&`)是一种强大的特性，允许我们直接操作对象而不需要拷贝。然而，标准容器如`std::vector`不能直接存储引用，许多算法和函数默认按值传递参数。为了解决这些问题，C++标准库在`<functional>`头文件中提供了`std::reference_wrapper`及其辅助函数`std::ref`和`std::cref`。

## 2. std::reference_wrapper

### 2.1 基本概念

`std::reference_wrapper`是一个模板类，它将引用包装成一个可复制、可赋值的对象。主要特点包括：

- 可以存储在容器中
- 可以按值传递
- 保留了引用的语义（可以修改原对象）

```cpp
template<class T>
class reference_wrapper;
```

### 2.2 核心功能

1. **构造和赋值**：
   ```cpp
   int x = 10;
   std::reference_wrapper<int> ref1(x);  // 直接构造
   auto ref2 = std::ref(x);             // 使用std::ref辅助函数
   ```

2. **访问被包装的引用**：
   - `get()`方法：显式获取底层引用
   - 隐式转换：可以自动转换为`T&`

### 2.3 使用示例

```cpp
#include <iostream>
#include <functional>
#include <vector>
#include <numeric>  // for std::iota

int main() {
    // 基本使用
    int a = 10;
    std::reference_wrapper<int> ref_a(a);
    
    // 访问原始对象
    std::cout << ref_a.get() << std::endl;  // 输出: 10
    std::cout << ref_a << std::endl;        // 输出: 10 (隐式转换)
    
    // 修改原始对象
    ref_a.get() = 20;
    std::cout << a << std::endl;            // 输出: 20
    
    // 在容器中使用
    std::list<int> l(10);
    std::iota(l.begin(), l.end(), -4);  // 填充-4到5
    
    std::vector<std::reference_wrapper<int>> v(l.begin(), l.end());
    for (auto& ref : v) {
        ref.get() *= 2;  // 修改原始list中的元素
    }
    
    // 多个引用包装
    int x = 1, y = 2, z = 3;
    std::vector<std::reference_wrapper<int>> vec{std::ref(x), std::ref(y), std::ref(z)};
    for (auto& ref : vec) ref.get() *= 2;
    std::cout << x << ", " << y << ", " << z << std::endl;  // 输出: 2, 4, 6
}
```

## 3. std::ref和std::cref

### 3.1 基本概念

`std::ref`和`std::cref`是用于创建`std::reference_wrapper`的辅助函数模板：

- `std::ref(T& t)`：创建非const引用包装器
- `std::cref(const T& t)`：创建const引用包装器

### 3.2 核心作用

1. **在需要按值传递的上下文中模拟引用传递**
2. **避免不必要的对象拷贝**
3. **允许修改原始对象（对于std::ref）**

### 3.3 使用场景

#### 3.3.1 与std::bind一起使用

```cpp
#include <functional>
#include <iostream>

void f(int& n1, int& n2, const int& n3) {
    std::cout << "In function: " << n1 << ' ' << n2 << ' ' << n3 << '\n';
    ++n1;  // 增加存储于函数对象的 n1 副本
    ++n2;  // 增加 main() 的 n2
    // ++n3; // 编译错误
}

int main() {
    int n1 = 1, n2 = 2, n3 = 3;
    std::function<void()> bound_f = std::bind(f, n1, std::ref(n2), std::cref(n3));
    
    n1 = 10;
    n2 = 11;
    n3 = 12;
    std::cout << "Before function: " << n1 << ' ' << n2 << ' ' << n3 << '\n';
    bound_f();
    std::cout << "After function: " << n1 << ' ' << n2 << ' ' << n3 << '\n';
}
```

输出结果：
```
Before function: 10 11 12
In function: 1 11 12
After function: 10 12 12
```

#### 3.3.2 在多线程中使用

```cpp
#include <iostream>
#include <thread>
#include <functional>

void modify(int& x) {
    x = 42;
}

void read(const int& x) {
    std::cout << "Value: " << x << std::endl;
}

int main() {
    int a = 0;
    int b = 0;

    // 错误：默认值传递，无法修改外部变量a
    std::thread t1(modify, a);
    t1.join();
    std::cout << a << std::endl;  // 输出 0

    // 正确：通过std::ref传递引用
    std::thread t2(modify, std::ref(a));
    t2.join();
    std::cout << a << std::endl;  // 输出 42

    // 正确：通过std::cref传递const引用
    std::thread t3(read, std::cref(b));
    t3.join();  // 输出 Value: 0
}
```

#### 3.3.3 在STL算法中使用

```cpp
#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

struct BigObject {
    int id;
    // 假设有大量数据...
};

void process(BigObject& obj) {
    obj.id *= 2;
}

int main() {
    std::vector<BigObject> objects{{1}, {2}, {3}};
    std::vector<std::reference_wrapper<BigObject>> refs(objects.begin(), objects.end());
    
    // 使用算法处理引用
    std::for_each(refs.begin(), refs.end(), [](BigObject& obj) {
        process(obj);
    });
    
    for (const auto& obj : objects) {
        std::cout << obj.id << " ";  // 输出: 2 4 6
    }
}
```

## 4. 比较与总结

### 4.1 特性对比

| 特性               | `std::ref()`                | `std::cref()`               | 原始引用`&` |
|-------------------|-----------------------------|-----------------------------|------------|
| **可复制**         | 是                          | 是                          | 否         |
| **可存储于容器**    | 是                          | 是                          | 否         |
| **允许修改对象**    | 是                          | 否                          | 是         |
| **隐式转换**       | 转换为`T&`                  | 转换为`const T&`            | -          |
| **主要用途**       | 需要修改的引用传递           | 只读的引用传递               | 常规引用    |

### 4.2 使用建议

1. **优先使用`std::ref`/`std::cref`**：在需要引用传递但上下文要求按值传递时使用
2. **注意生命周期**：确保被引用对象的生命周期足够长
3. **性能考虑**：对于小型简单类型，直接按值传递可能更高效
4. **线程安全**：多线程中使用时要注意同步问题

### 4.3 常见陷阱

1. **悬垂引用**：
   ```cpp
   std::reference_wrapper<int> bad_ref;
   {
       int x = 10;
       bad_ref = std::ref(x);
   }  // x被销毁
   // 使用bad_ref会导致未定义行为
   ```

2. **类型不匹配**：
   ```cpp
   void foo(int&);
   const int x = 10;
   foo(std::cref(x));  // 错误：不能将const引用转换为非const
   ```

3. **不必要的使用**：
   ```cpp
   // 对于简单类型，直接按值传递可能更好
   void bar(int);
   int y = 5;
   bar(std::ref(y));  // 不必要，直接传值即可
   ```

## 5. 高级用法

### 5.1 与元组一起使用

```cpp
#include <tuple>
#include <functional>
#include <iostream>

int main() {
    int a = 1, b = 2;
    auto t = std::make_tuple(std::ref(a), std::cref(b));
    
    std::get<0>(t) = 10;  // 修改a
    // std::get<1>(t) = 20;  // 错误：cref是const引用
    
    std::cout << a << ", " << b << std::endl;  // 输出: 10, 2
}
```

### 5.2 与多态对象一起使用

```cpp
#include <functional>
#include <iostream>
#include <vector>

class Base {
public:
    virtual void print() const { std::cout << "Base\n"; }
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    void print() const override { std::cout << "Derived\n"; }
};

void call_print(const Base& b) {
    b.print();
}

int main() {
    Derived d;
    std::vector<std::reference_wrapper<const Base>> objects;
    objects.push_back(std::cref(d));
    
    for (const auto& ref : objects) {
        call_print(ref);  // 输出: Derived (多态行为保留)
    }
}
```

## 6. 实现原理

`std::reference_wrapper`通常实现为一个包含指针的轻量级包装器：

```cpp
namespace std {
    template<class T>
    class reference_wrapper {
    public:
        // 构造函数
        explicit reference_wrapper(T& ref) noexcept : ptr_(&ref) {}
        
        // 获取引用
        operator T& () const noexcept { return *ptr_; }
        T& get() const noexcept { return *ptr_; }
        
        // 禁止默认构造和绑定到临时对象
        reference_wrapper() = delete;
        reference_wrapper(T&&) = delete;
        
    private:
        T* ptr_;
    };
    
    // std::ref和std::cref的实现
    template<class T>
    reference_wrapper<T> ref(T& t) noexcept {
        return reference_wrapper<T>(t);
    }
    
    template<class T>
    reference_wrapper<const T> cref(const T& t) noexcept {
        return reference_wrapper<const T>(t);
    }
}
```

## 7. 最佳实践总结

1. **明确使用场景**：
   - 需要在容器中存储引用时
   - 需要将引用传递给按值接收参数的函数/算法时
   - 需要绑定引用到函数对象时

2. **生命周期管理**：
   ```cpp
   // 好: 对象生命周期足够长
   std::vector<int> values{1, 2, 3};
   std::vector<std::reference_wrapper<int>> refs(values.begin(), values.end());
   
   // 危险: 临时对象
   auto dangerous = std::ref(std::string("temporary"));  // 悬垂引用!
   ```

3. **性能优化**：
   ```cpp
   // 对于大型对象
   struct Heavy { /* 大量数据 */ };
   Heavy heavy;
   
   // 好: 避免拷贝
   std::thread t(process_heavy, std::ref(heavy));
   
   // 对于小型基本类型，直接传值
   int x = 42;
   std::thread t2(process_int, x);  // 比std::ref(x)更高效
   ```

4. **代码可读性**：
   ```cpp
   // 明确使用std::ref表明意图
   auto worker = std::bind(&Processor::run, std::ref(processor));
   
   // 比使用指针更清晰
   auto worker2 = std::bind(&Processor::run, &processor);  // 也可以，但语义不如ref明确
   ```
