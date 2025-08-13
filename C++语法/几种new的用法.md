在C++中，`new` 操作符主要有以下几种形式，每种都有特定的用途和行为。下面我会详细介绍每一种 `new` 的用法、特点以及适用场景。

---

## 1. **普通 `new`（标准 `new` 表达式）**
### **语法**
```cpp
T* p = new T;      // 默认构造
T* p = new T(args); // 带参数构造
```
### **特点**
1. **动态分配内存**：在堆（heap）上分配 `sizeof(T)` 字节的内存。
2. **调用构造函数**：如果 `T` 是类类型，会调用匹配的构造函数。
3. **可能抛出 `std::bad_alloc`**：如果内存不足，默认会抛出异常（而不是返回 `nullptr`）。
4. **需要手动 `delete`**：必须显式调用 `delete` 释放内存，否则会导致内存泄漏。

### **示例**
```cpp
int* p = new int(42);  // 分配一个 int 并初始化为 42
delete p;              // 必须手动释放
```

---

## 2. **`new[]`（数组 `new`）**
### **语法**
```cpp
T* p = new T[N];  // 分配 N 个 T 的数组
```
### **特点**
1. **分配连续内存**：分配 `N * sizeof(T)` 字节的内存。
2. **调用默认构造函数**：对每个元素调用 `T` 的默认构造函数（如果没有默认构造，编译错误）。
3. **必须用 `delete[]` 释放**：如果用 `delete` 而不是 `delete[]`，行为未定义（UB）。

### **示例**
```cpp
int* arr = new int[10];  // 分配 10 个 int
delete[] arr;            // 必须用 delete[] 释放
```

---

## 3. **`nothrow new`（不抛异常的 `new`）**
### **语法**
```cpp
T* p = new (std::nothrow) T;
```
### **特点**
1. **失败时返回 `nullptr`**：如果分配失败，不会抛出 `std::bad_alloc`，而是返回 `nullptr`。
2. **适用于避免异常的场景**：比如嵌入式系统或某些异常禁用环境。

### **示例**
```cpp
int* p = new (std::nothrow) int;
if (!p) {
    std::cerr << "Allocation failed!" << std::endl;
} else {
    *p = 42;
    delete p;
}
```

---

## 4. **`placement new`（定位 `new`）**
### **语法**
```cpp
void* buffer = /* 预先分配的内存 */;
T* p = new (buffer) T(args);  // 在指定内存上构造对象
```
### **特点**
1. **不分配内存**，只在给定的内存地址构造对象。
2. **适用于自定义内存管理**（如内存池、栈分配、共享内存等）。
3. **需要显式调用析构函数**：`p->~T()`（不能直接 `delete`，因为内存不是 `new` 分配的）。

### **示例**
```cpp
alignas(int) char buffer[sizeof(int)];  // 预先分配的内存
int* p = new (buffer) int(42);          // 在 buffer 上构造 int
p->~int();                              // 显式调用析构函数
// 注意：不能 delete p，因为 buffer 不是 new 分配的
```

---

## 5. **`aligned new`（对齐 `new`，C++17 引入）**
### **语法**
```cpp
T* p = new (std::align_val_t(align)) T;  // 分配对齐内存
```
### **特点**
1. **保证内存对齐**：适用于 SIMD（如 `__m256`）、硬件访问等需要特定对齐的场景。
2. **必须用 `delete` 的 `align_val_t` 版本释放**：
   ```cpp
   operator delete(p, std::align_val_t(align));
   ```

### **示例**
```cpp
#include <new>
struct alignas(64) CacheLine {  // 64 字节对齐
    char data[64];
};
CacheLine* p = new (std::align_val_t(64)) CacheLine;
operator delete(p, std::align_val_t(64));  // 对齐释放
```

---

## 6. **自定义 `operator new`（重载 `new`）**
### **语法**
```cpp
void* operator new(size_t size, CustomAllocator& alloc) {
    return alloc.allocate(size);
}
T* p = new (alloc) T;  // 使用自定义分配器
```
### **特点**
1. **允许自定义内存分配逻辑**（如内存池、日志记录等）。
2. **可以结合 `placement new` 使用**。

### **示例**
```cpp
class MemoryPool {
public:
    void* allocate(size_t size) { /* ... */ }
    void deallocate(void* p) { /* ... */ }
};

void* operator new(size_t size, MemoryPool& pool) {
    return pool.allocate(size);
}

MemoryPool pool;
int* p = new (pool) int(42);  // 使用 MemoryPool 分配
// 需要手动调用析构 + 自定义释放
p->~int();
pool.deallocate(p);
```

---

## 总结对比表
| **`new` 类型**       | **语法**                  | **特点**                          | **释放方式**            |
|----------------------|--------------------------|----------------------------------|------------------------|
| 标准 `new`           | `new T`                  | 可能抛 `bad_alloc`               | `delete p`             |
| 数组 `new`           | `new T[N]`               | 分配连续内存                     | `delete[] p`           |
| `nothrow new`        | `new (nothrow) T`        | 失败返回 `nullptr`               | `delete p`             |
| `placement new`      | `new (buffer) T`         | 不分配内存，仅构造               | `p->~T()`              |
| `aligned new` (C++17)| `new (align_val_t(N)) T` | 保证对齐                         | `operator delete(p, align_val_t(N))` |
| 自定义 `new`         | `new (alloc) T`          | 用户控制分配逻辑                 | 依赖自定义释放         |

---

## 常见问题
### **1. `new` 和 `malloc` 的区别？**
- `new` 会调用构造函数，`malloc` 不会。
- `new` 是类型安全的，`malloc` 返回 `void*`。
- `new` 可以重载，`malloc` 是 C 标准库函数。

### **2. 为什么 `new[]` 必须用 `delete[]`？**
因为 `new[]` 可能存储额外的元信息（如数组大小），`delete[]` 会正确解析并调用每个元素的析构函数，而 `delete` 不会。

### **3. 什么时候用 `placement new`？**
- 内存池管理
- 避免动态分配（如嵌入式系统）
- 在共享内存上构造对象

### **4. C++17 的 `aligned new` 和 `alignas` 有什么关系？**
`alignas` 指定类型的对齐要求，`aligned new` 确保分配的内存满足对齐要求。

---

掌握这些 `new` 的变种，可以更灵活地控制内存分配和对象构造，适用于不同场景（如高性能计算、嵌入式开发等）。