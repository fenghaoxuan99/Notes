
# Lambda 与 std::function 的区别
## 1. 核心知识点解析
### Lambda 表达式（C++11）
- **本质**：Lambda 是一种**语法糖**，编译器会为每个 lambda 表达式生成一个**唯一的匿名函数对象（functor）类型**。
- **特点**：
  - 类型是**编译期确定的、唯一的闭包类型**（closure type），无法显式写出其类型。
  - 可以捕获外部变量（值捕获 `[=]`、引用捕获 `[&]`、混合捕获等）。
  - 支持指定返回类型（`-> ReturnType`）和函数体。
  - 性能高，因为是**静态类型**，编译器可以内联优化。

### std::function（C++11）
- **本质**：`std::function` 是一个**类型擦除（type erasure）的包装器**，可以存储、复制和调用任何可调用目标（函数、lambda、函数对象、成员函数指针等）。
- **特点**：
  - 是一个**具体类型**，可以统一接口调用不同类型的可调用对象。
  - 有运行时开销（可能涉及堆分配、虚函数调用）。
  - 支持赋值、拷贝、移动。
  - 可以为空（通过 `operator bool()` 判断是否可调用）。

### 易混淆点
| 特性             | Lambda 表达式                  | std::function                         |
|------------------|-------------------------------|----------------------------------------|
| 类型             | 匿名、唯一闭包类型             | std::function<R(Args...)>              |
| 类型擦除         | 否                            | 是                                     |
| 性能             | 高（静态绑定）                | 可能低（动态绑定）                     |
| 可赋值性         | 否（每个 lambda 是不同类型）  | 是                                     |
| 捕获             | 支持                          | 不直接支持，但可包装捕获的 lambda      |

### 实际应用场景
- **Lambda**：适合局部使用、性能敏感、不需要统一接口的场景。
- **std::function**：适合需要统一接口、回调注册、多态调用的场景。

---

## 2. 标准化面试回答模板

**面试官：请解释 lambda 与 std::function 的区别？**

### 回答结构：

1. **定义与本质**：
   - Lambda 是一种语法糖，编译器会为每个 lambda 生成一个唯一的函数对象类型。
   - `std::function` 是一个类型擦除的包装器，可以存储任何可调用对象。

2. **类型与性能**：
   - Lambda 的类型是编译期确定的闭包类型，性能高，适合局部使用。
   - `std::function` 是统一类型，但有运行时开销，适合需要统一接口的场景。

3. **使用场景**：
   - Lambda 用于局部逻辑封装、STL 算法等。
   - `std::function` 用于回调系统、事件处理、策略模式等。

4. **C++11/14/17 新特性支持**：
   - Lambda 支持捕获、泛型 lambda（C++14）、constexpr（C++17）。
   - `std::function` 是 C++11 引入的标准工具。

---

## 3. 代码示例与最佳实践

### Lambda 示例
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> vec = {1, 3, 2, 5, 4};

    int threshold = 3;
    // Lambda 捕获外部变量
    auto is_greater = [threshold](int x) { return x > threshold; };

    // 使用 lambda 进行过滤
    vec.erase(std::remove_if(vec.begin(), vec.end(), is_greater), vec.end());

    for (int x : vec) {
        std::cout << x << " ";
    }
    // 输出：1 3 2
}
```

### std::function 示例
```cpp
#include <iostream>
#include <functional>

void greet() {
    std::cout << "Hello!" << std::endl;
}

struct Greeter {
    void operator()() const {
        std::cout << "Hi from functor!" << std::endl;
    }
};

int main() {
    std::function<void()> f1 = greet;
    std::function<void()> f2 = Greeter{};
    std::function<void()> f3 = []() { std::cout << "Hi from lambda!" << std::endl; };

    f1(); // Hello!
    f2(); // Hi from functor!
    f3(); // Hi from lambda!
}
```

### 最佳实践
- **优先使用 lambda**：在不需要类型擦除的场景中，lambda 更高效。
- **使用 std::function 时注意性能**：避免频繁调用或性能敏感路径。
- **避免不必要的 std::function 拷贝**：使用引用或移动语义优化。

---

## 4. 面试官视角分析

### 这个问题想考察什么能力？
- 对现代 C++ 特性的理解（lambda、std::function）
- 对类型系统、性能、编译期与运行时差异的掌握
- 实际应用中的选择能力

### 优秀回答应包含哪些层次？
1. **概念清晰**：准确区分两者本质
2. **性能对比**：指出运行时 vs 编译时开销
3. **使用场景**：能举例说明何时用哪个
4. **语言特性掌握**：提及 C++11/14/17 的支持

### 可能的深入追问及应对策略

#### Q: Lambda 能否赋值给另一个 lambda？
**A**: 不行，每个 lambda 是不同的类型，即使内容一样。

#### Q: std::function 内部是如何实现类型擦除的？
**A**: 通过模板 + 虚函数表或类似机制实现统一接口调用。

#### Q: 泛型 lambda 是什么？（C++14）
**A**: 使用 `auto` 参数的 lambda，编译器会为每种调用生成特化版本。

---

## 5. 学习建议与知识扩展

### 相关知识点延伸
- **可调用对象**：函数指针、函数对象、lambda、std::bind、成员函数指针
- **类型擦除技术**：std::any、std::variant、虚函数表
- **现代 C++ 特性**：auto、decltype、移动语义、完美转发

### 常见面试陷阱提醒
- ❌ 混淆 lambda 与 std::function 的性能差异
- ❌ 忽略 lambda 捕获的副作用（引用悬空、值拷贝）
- ❌ 误以为 lambda 可赋值或统一类型
