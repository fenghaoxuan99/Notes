# 手写简化版 Vector 实现

下面是一个简化版的 `vector` 实现，包含 `push_back` 和 `pop_back` 功能，并考虑了扩容和缩容策略。这个实现使用了模板以支持多种数据类型。

```cpp
#include <iostream>
#include <algorithm> // for std::copy
#include <stdexcept> // for std::out_of_range

template <typename T>
class MyVector {
private:
    T* data_;          // 存储元素的数组指针
    size_t size_;      // 当前元素数量
    size_t capacity_;  // 当前容量

    // 扩容函数
    void expand(size_t new_capacity) {
        if (new_capacity <= capacity_) return;
        
        T* new_data = new T[new_capacity];
        // 复制原有元素到新空间
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }
        
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

    // 缩容函数
    void shrink() {
        // 当元素数量小于容量的1/4时，容量减半
        if (size_ * 4 < capacity_ && capacity_ > 4) {
            size_t new_capacity = capacity_ / 2;
            T* new_data = new T[new_capacity];
            
            for (size_t i = 0; i < size_; ++i) {
                new_data[i] = data_[i];
            }
            
            delete[] data_;
            data_ = new_data;
            capacity_ = new_capacity;
        }
    }

public:
    // 构造函数
    MyVector() : data_(nullptr), size_(0), capacity_(0) {}
    
    // 带初始容量的构造函数
    explicit MyVector(size_t initial_capacity) 
        : data_(new T[initial_capacity]), size_(0), capacity_(initial_capacity) {}
    
    // 析构函数
    ~MyVector() {
        delete[] data_;
    }
    
    // 拷贝构造函数
    MyVector(const MyVector& other) 
        : data_(new T[other.capacity_]), size_(other.size_), capacity_(other.capacity_) {
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }
    
    // 拷贝赋值运算符
    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            delete[] data_;
            data_ = new T[other.capacity_];
            size_ = other.size_;
            capacity_ = other.capacity_;
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
        return *this;
    }
    
    // 添加元素到末尾
    void push_back(const T& value) {
        // 如果空间不足，先扩容
        if (size_ >= capacity_) {
            // 初始容量为1，之后每次翻倍
            expand(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[size_++] = value;
    }
    
    // 移除末尾元素
    void pop_back() {
        if (size_ == 0) {
            throw std::out_of_range("Vector is empty");
        }
        --size_;
        // 检查是否需要缩容
        shrink();
    }
    
    // 获取元素数量
    size_t size() const { return size_; }
    
    // 获取当前容量
    size_t capacity() const { return capacity_; }
    
    // 访问元素
    T& operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }
    
    // const版本访问元素
    const T& operator[](size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }
    
    // 打印vector内容（用于测试）
    void print() const {
        std::cout << "[";
        for (size_t i = 0; i < size_; ++i) {
            std::cout << data_[i];
            if (i != size_ - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
};
```

## 关键点说明

1. **扩容策略**：
   - 初始容量为0
   - 第一次插入元素时容量设为1
   - 之后每次空间不足时容量翻倍
   - 这种策略保证了均摊时间复杂度为O(1)

2. **缩容策略**：
   - 当元素数量小于容量的1/4时，容量减半
   - 最小容量为4（避免频繁扩容缩容）
   - 这样可以节省内存，同时避免频繁的内存分配

3. **异常处理**：
   - 在pop_back和访问元素时检查边界
   - 抛出std::out_of_range异常

4. **内存管理**：
   - 使用new[]和delete[]分配和释放内存
   - 实现了拷贝构造函数和拷贝赋值运算符（遵循三法则）

5. **时间复杂度**：
   - push_back: 均摊O(1)
   - pop_back: O(1)（不考虑缩容时的内存分配）

## 使用示例

```cpp
int main() {
    MyVector<int> vec;
    
    // 测试push_back和扩容
    for (int i = 0; i < 10; ++i) {
        vec.push_back(i);
        std::cout << "Size: " << vec.size() 
                  << ", Capacity: " << vec.capacity() << std::endl;
    }
    vec.print();
    
    // 测试pop_back和缩容
    while (vec.size() > 0) {
        vec.pop_back();
        std::cout << "Size: " << vec.size() 
                  << ", Capacity: " << vec.capacity() << std::endl;
    }
    
    return 0;
}
```

这个实现简化了标准库vector的许多功能，但包含了核心的push_back和pop_back操作，并实现了基本的扩容和缩容策略。