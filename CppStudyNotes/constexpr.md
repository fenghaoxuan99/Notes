

# C++ `constexpr` 完全指南：从原理到实战

## 一、核心概念与设计哲学
1. **编译时计算模型**
   - `constexpr` 建立了一个编译时可求值的子语言系统
   - 核心优势：将运行时成本转移到编译期，实现零开销抽象

2. **类型系统增强**
   ```cpp
   // 编译时类型验证示例
   template<typename T>
   constexpr bool is_arithmetic_v = std::is_arithmetic<T>::value;
   
   static_assert(is_arithmetic_v<int>, "类型检查失败");
   ```

3. **与 `const` 的对比矩阵**
   | 特性            | `const`            | `constexpr`        |
   |----------------|-------------------|-------------------|
   | 求值时机        | 运行时             | 编译时            |
   | 初始化要求      | 可延迟初始化       | 必须立即初始化    |
   | 函数修饰        | 保证不修改成员状态 | 保证编译时可求值  |
   | 内存占用        | 占用存储空间       | 可能完全优化掉    |

## 二、深度用法与模式

### 1. 变量声明进阶
```cpp
// 现代C++推荐写法
constexpr auto kConfig = []{
    struct Config {
        int timeout = 1000;
        string_view name = "default";
    };
    return Config{};
}();

// 编译时字符串处理
constexpr auto compile_time_str = []{
    char arr[10] = {};
    arr[0] = 'H'; arr[1] = 'i';
    return arr;
}();
```

### 2. 函数编写技巧
**递归模式 (C++11兼容)**
```cpp
constexpr int fibonacci(int n) {
    return (n <= 1) ? n : (fibonacci(n-1) + fibonacci(n-2));
}

static_assert(fibonacci(10) == 55, "编译期计算验证");
```

**迭代模式 (C++14+)**
```cpp
constexpr auto compile_time_sort = []{
    std::array arr{3, 1, 4, 1, 5};
    std::sort(arr.begin(), arr.end());
    return arr;
}();
```

### 3. 类与对象设计
**编译时多态示例**
```cpp
class Shape {
public:
    constexpr virtual double area() const = 0;
    virtual ~Shape() = default;  // 仍需虚析构
};

class Circle : public Shape {
    double r;
public:
    constexpr Circle(double radius) : r(radius) {}
    constexpr double area() const override { 
        return 3.1415926 * r * r; 
    }
};

constexpr Circle unit_circle(1.0);
static_assert(unit_circle.area() > 3.14, "");
```

## 三、实战应用模式

### 1. 资源预计算
```cpp
// 游戏开发中的技能数据预计算
struct SkillData {
    int base_damage;
    int level_multiplier;
    constexpr int calculate(int level) const {
        return base_damage + level * level_multiplier;
    }
};

constexpr SkillData fireball{100, 15};
constexpr int max_damage = fireball.calculate(10);  // 250
```

### 2. 类型安全接口
```cpp
template<size_t N>
class FixedString {
    char data_[N+1] = {};
public:
    constexpr FixedString(const char (&str)[N]) {
        std::copy_n(str, N, data_);
    }
    constexpr operator string_view() const { 
        return {data_, N}; 
    }
};

constexpr FixedString hello = "Hello";
static_assert(hello.size() == 5, "");
```

### 3. 编译时策略选择
```cpp
template<typename T>
constexpr auto type_handler() {
    if constexpr (std::is_integral_v<T>) {
        return "integer";
    } else if constexpr (std::is_floating_point_v<T>) {
        return "float";
    } else {
        return "other";
    }
}

static_assert(string_view(type_handler<int>()) == "integer", "");
```

## 四、性能优化技巧

1. **编译期/运行时混合计算**
```cpp
constexpr int optimized_fib(int n) {
    if (std::is_constant_evaluated()) {
        // 编译期分支
        return (n <= 1) ? n : (optimized_fib(n-1) + optimized_fib(n-2));
    } else {
        // 运行时分支
        int a = 0, b = 1;
        for (int i = 0; i < n; ++i) {
            int tmp = a + b;
            a = b;
            b = tmp;
        }
        return a;
    }
}
```

2. **内存布局优化**
```cpp
constexpr size_t calculate_padding(size_t offset, size_t alignment) {
    return (alignment - (offset % alignment)) % alignment;
}

struct PackedData {
    char header[4];
    // 编译时计算padding
    char padding[calculate_padding(sizeof(header), alignof(double))];
    double payload;
};
```

## 五、跨版本兼容方案

```cpp
#if __cplusplus >= 201402L
#define CONSTEXPR14 constexpr
#else
#define CONSTEXPR14 inline
#endif

CONSTEXPR14 int modern_feature() {
    // C++14+代码
    int x = 0;
    for (int i = 0; i < 10; ++i) x += i;
    return x;
}
```

## 六、调试与测试方法

1. **静态断言验证**
```cpp
constexpr int compute() { /*...*/ }
static_assert(compute() == expected, "Test failed");
```

2. **编译时打印技巧**
```cpp
template<auto> struct Debug;
Debug<compile_time_value> debug;  // 查看编译错误信息
```

3. **运行时fallback检查**
```cpp
constexpr auto result = compile_time_computation();
if constexpr (result.is_valid()) {
    use_result(result);
} else {
    fallback_implementation();
}
```

## 七、最佳实践清单

1. 优先用`constexpr`替代宏定义常量
2. 对数学运算、配置数据等使用编译时计算
3. 模板元编程中结合`constexpr`简化代码
4. 用`if constexpr`替换SFINAE复杂逻辑
5. 注意编译时计算的复杂度限制
6. 为关键路径提供运行时fallback

