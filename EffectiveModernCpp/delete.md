# C++模板特化与删除函数的高级应用

### 1. 模板特化与删除函数的概念

#### 1.1 模板特化
模板特化允许我们为特定类型提供模板的特殊实现。当编译器遇到与特化匹配的类型时，会使用特化版本而非通用模板。

#### 1.2 删除函数
C++11引入了`= delete`语法，用于显式禁止某些函数的使用。当尝试调用被删除的函数时，编译器会报错。

### 2. 指针处理的特殊情况

在指针处理中，有两种特殊情况需要特别注意：

1. **void***指针：
   - 不能解引用
   - 不能进行指针算术运算
   - 需要特殊处理

2. **char***指针：
   - 通常表示C风格字符串
   - 与指向单个字符的指针语义不同
   - 可能需要特殊处理

### 3. 解决方案：使用模板特化和删除函数

我们可以结合模板特化和删除函数来禁止特定类型的指针被处理：

```cpp
template<typename T>
void processPointer(T* ptr) {
    // 通用指针处理逻辑
    // ...
}

// 显式删除void*特化
template<>
void processPointer<void>(void*) = delete;

// 显式删除char*特化
template<>
void processPointer<char>(char*) = delete;
```

### 4. 使用方法和示例

#### 4.1 基本用法

```cpp
int main() {
    int* intPtr = new int(42);
    double* doublePtr = new double(3.14);
    
    processPointer(intPtr);      // 正常调用通用模板
    processPointer(doublePtr);   // 正常调用通用模板
    
    // 以下调用会导致编译错误
    void* voidPtr = nullptr;
    processPointer(voidPtr);     // 错误：尝试使用已删除的函数
    
    char* charPtr = "hello";
    processPointer(charPtr);     // 错误：尝试使用已删除的函数
    
    delete intPtr;
    delete doublePtr;
    return 0;
}
```

#### 4.2 更复杂的示例

我们可以扩展这个模式来处理更多特殊情况：

```cpp
// 删除const char*特化（C风格字符串字面量）
template<>
void processPointer<const char>(const char*) = delete;

// 删除wchar_t*特化（宽字符字符串）
template<>
void processPointer<wchar_t>(wchar_t*) = delete;

int main() {
    const char* str = "example";
    processPointer(str);         // 错误：尝试使用已删除的函数
    
    wchar_t* wstr = L"wide";
    processPointer(wstr);        // 错误：尝试使用已删除的函数
    
    return 0;
}
```

### 5. 替代方案比较

#### 5.1 使用static_assert

另一种实现方式是使用`static_assert`在编译时进行检查：

```cpp
template<typename T>
void processPointer(T* ptr) {
    static_assert(!std::is_same<T, void>::value, 
                 "Cannot process void pointers");
    static_assert(!std::is_same<T, char>::value,
                 "Cannot process char pointers");
    // 处理逻辑...
}
```

#### 5.2 两种方式的比较

| 特性               | 删除函数特化              | static_assert               |
|--------------------|--------------------------|-----------------------------|
| 错误消息           | 标准删除函数错误          | 自定义错误消息              |
| 编译阶段           | 重载决议阶段              | 模板实例化阶段              |
| 可扩展性           | 需要为每种类型特化        | 可在单个函数中处理多种情况  |
| 与其他特性交互     | 可与SFINAE配合使用        | 可能导致硬错误              |