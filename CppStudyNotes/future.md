# C++异步编程：`std::future` 全面详解与实战指南

`std::future` 是 C++11 引入的异步编程核心组件，为现代 C++ 并发编程提供了强大的支持。本文将全面介绍 `std::future` 及其相关组件，从基础用法到高级模式，帮助开发者掌握异步编程的核心技术。

## 一、核心概念与组件

### 1. `std::future` 概述

`std::future` 是一个模板类，用于表示异步计算的结果。它提供了访问异步操作结果的机制，允许我们以同步或异步的方式获取计算结果。

**关键特性**：
- 异步结果持有者
- 提供结果获取机制
- 支持异常传播
- 线程安全

### 2. 相关组件

- **`std::promise`**：提供存储值或异常的机制，可通过 `get_future()` 获取关联的 `future` 对象
- **`std::packaged_task`**：将可调用对象包装为异步任务，自动管理 `future` 和 `promise`
- **`std::async`**：便捷函数，自动创建异步任务并返回 `future`

## 二、`std::future` 核心成员函数详解

### 1. `get()` - 获取结果值

**功能**：获取与 future 关联的共享状态中的值

**特性**：
- 阻塞调用，直到结果就绪
- 只能调用一次，第二次调用会抛出 `std::future_error`
- 会传递异步操作中抛出的异常

```cpp
#include <iostream>
#include <future>
#include <thread>

int main() {
    std::future<int> fut = std::async([]{ // 启动异步任务
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return 42; // 返回计算结果
    });

    // 阻塞直到结果就绪
    int result = fut.get();  // 此处会等待1秒
    std::cout << "Result: " << result << std::endl; // 输出42

    return 0;
}
```

### 2. `valid()` - 检查有效性

**功能**：检查 future 是否关联有效的共享状态

**使用场景**：
- 检查默认构造的 future
- 检查已经调用过 get() 的 future

```cpp
#include <cassert>
#include <future>

int main() {
    std::future<int> fut1;  // 默认构造，无效
    assert(!fut1.valid());

    auto fut2 = std::async([]{ return 1; });
    assert(fut2.valid());

    fut2.get();
    assert(!fut2.valid());  // get()后变为无效

    return 0;
}
```

### 3. `wait()` - 阻塞等待

**功能**：阻塞当前线程直到结果就绪

**与 get() 的区别**：
- 不获取结果，仅等待
- 可以多次调用

```cpp
#include <iostream>
#include <future>
#include <chrono>

int main() {
    auto fut = std::async([]{ // 启动异步计算
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return 3.14;
    });

    std::cout << "Waiting for result..." << std::endl;
    fut.wait();  // 阻塞2秒
    std::cout << "Result ready!" << std::endl;
    
    double result = fut.get();  // 立即返回
    std::cout << "Value: " << result << std::endl;

    return 0;
}
```

### 4. `wait_for()` - 超时等待

**功能**：带相对超时的等待

**返回值**：
- `future_status::ready` - 结果已就绪
- `future_status::timeout` - 超时
- `future_status::deferred` - 任务延迟执行

```cpp
#include <iostream>
#include <future>
#include <chrono>

int main() {
    auto fut = std::async([]{ // 启动异步任务
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return 'A';
    });

    auto status = fut.wait_for(std::chrono::milliseconds(200));
    if(status == std::future_status::ready) {
        std::cout << "Got result: " << fut.get() << std::endl;
    } else {
        std::cout << "Timeout, still waiting..." << std::endl;
        fut.wait();  // 继续等待
        std::cout << "Finally got: " << fut.get() << std::endl;
    }

    return 0;
}
```

### 5. `wait_until()` - 绝对时间等待

**功能**：带绝对时间点的等待

**参数**：`std::chrono::time_point`

```cpp
#include <iostream>
#include <future>
#include <chrono>

int main() {
    auto fut = std::async([]{ // 启动异步计算
        std::this_thread::sleep_for(std::chrono::milliseconds(800));
        return 42;
    });

    auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
    auto status = fut.wait_until(deadline);

    if(status == std::future_status::ready) {
        std::cout << "Result: " << fut.get() << std::endl;
    } else {
        std::cout << "Not ready yet" << std::endl;
        // 可以继续等待或采取其他措施
    }

    return 0;
}
```

## 三、`std::future` 使用模式与示例

### 1. 基本异步任务模式

```cpp
#include <iostream>
#include <future>
#include <string>

// 模拟异步数据获取
std::future<std::string> fetchData(const std::string& url) {
    return std::async(std::launch::async, [url]{
        // 模拟网络请求延迟
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return "Data from " + url;
    });
}

void process() {
    std::cout << "Starting data fetch..." << std::endl;
    auto data_future = fetchData("example.com");
    
    // 主线程可以继续执行其他工作
    std::cout << "Doing other work while waiting..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    try {
        // 获取异步结果
        std::string result = data_future.get();
        std::cout << "Received: " << result << std::endl;
    } catch(const std::exception& e) {
        std::cerr << "Failed to fetch data: " << e.what() << std::endl;
    }
}

int main() {
    process();
    return 0;
}
```

### 2. 超时控制模式

```cpp
#include <iostream>
#include <future>
#include <chrono>

bool process_with_timeout(std::future<void>& fut, int timeout_ms) {
    auto status = fut.wait_for(std::chrono::milliseconds(timeout_ms));
    if(status == std::future_status::ready) {
        try {
            fut.get();  // 可能获取异常
            return true;
        } catch(const std::exception& e) {
            std::cerr << "Task failed: " << e.what() << std::endl;
            return false;
        }
    }
    std::cout << "Operation timed out" << std::endl;
    return false;  // 超时
}

int main() {
    std::promise<void> prom;
    auto fut = prom.get_future();
    
    // 模拟长时间运行的任务
    std::thread([&prom]{
        std::this_thread::sleep_for(std::chrono::seconds(2));
        prom.set_value();
    }).detach();
    
    // 设置1秒超时
    if(process_with_timeout(fut, 1000)) {
        std::cout << "Task completed successfully" << std::endl;
    } else {
        std::cout << "Task did not complete in time" << std::endl;
    }
    
    return 0;
}
```

### 3. 多 future 组合模式

```cpp
#include <iostream>
#include <future>
#include <vector>
#include <numeric>

template<typename T>
std::vector<T> wait_all(std::vector<std::future<T>>& futures) {
    std::vector<T> results;
    for(auto& fut : futures) {
        results.push_back(fut.get());
    }
    return results;
}

void parallel_compute() {
    const int num_tasks = 5;
    std::vector<std::future<int>> futures;
    
    // 启动多个并行任务
    for(int i = 0; i < num_tasks; ++i) {
        futures.push_back(std::async([i]{
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return i * i;
        }));
    }
    
    // 等待所有任务完成并收集结果
    auto results = wait_all(futures);
    
    // 输出结果
    std::cout << "Results: ";
    for(int r : results) {
        std::cout << r << " ";
    }
    std::cout << std::endl;
    
    // 计算总和
    int sum = std::accumulate(results.begin(), results.end(), 0);
    std::cout << "Total sum: " << sum << std::endl;
}

int main() {
    parallel_compute();
    return 0;
}
```

### 4. 异常处理模式

```cpp
#include <iostream>
#include <future>
#include <stdexcept>

void async_task() {
    auto fut = std::async([]{
        // 模拟可能抛出异常的操作
        if(rand() % 2) {
            throw std::runtime_error("Something went wrong");
        }
        return 42;
    });

    try {
        int result = fut.get();
        std::cout << "Success! Result: " << result << std::endl;
    } catch(const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << std::endl;
        // 执行恢复操作...
    }
}

int main() {
    srand(time(nullptr));
    for(int i = 0; i < 5; ++i) {
        std::cout << "Attempt " << i+1 << ": ";
        async_task();
    }
    return 0;
}
```

## 四、`std::shared_future` 深入解析

### 1. 基本概念与创建方式

`std::shared_future` 允许多个线程等待和访问同一个异步结果，解决了 `std::future` 只能被单次访问的限制。

**创建方式**：

```cpp
#include <iostream>
#include <future>

int main() {
    // 方式1：从promise创建
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();
    std::shared_future<int> shared_fut = fut.share();  // 转移所有权
    
    // 方式2：直接从async创建
    std::shared_future<int> shared_fut2 = std::async([]{
        return 2023;
    }).share();
    
    // 方式3：拷贝构造
    std::shared_future<int> shared_fut3 = shared_fut2;
    
    std::cout << shared_fut2.get() << std::endl;
    std::cout << shared_fut3.get() << std::endl;
    
    return 0;
}
```

### 2. 多线程共享示例

```cpp
#include <iostream>
#include <future>
#include <vector>
#include <thread>

void worker(std::shared_future<int> sf, int id) {
    // 所有线程共享同一个结果
    int result = sf.get();
    std::cout << "Worker " << id << " got " << result << "\n";
}

int main() {
    std::promise<int> prom;
    auto shared_fut = prom.get_future().share();
    
    std::vector<std::thread> threads;
    const int num_threads = 5;
    
    // 创建多个工作线程
    for(int i = 0; i < num_threads; ++i) {
        threads.emplace_back(worker, shared_fut, i);
    }
    
    // 模拟一些准备工作
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // 设置结果值，唤醒所有等待的线程
    prom.set_value(42);
    
    // 等待所有线程完成
    for(auto& t : threads) {
        t.join();
    }
    
    return 0;
}
```

## 五、高级应用场景

### 1. 实现异步接口

```cpp
#include <iostream>
#include <future>
#include <string>
#include <vector>

class AsyncDataProcessor {
public:
    AsyncDataProcessor() = default;
    
    // 异步处理接口
    std::future<std::vector<int>> processData(const std::vector<int>& input) {
        return std::async(std::launch::async, [this, input]{
            return this->computeResults(input);
        });
    }
    
private:
    // 耗时的计算操作
    std::vector<int> computeResults(const std::vector<int>& input) {
        std::vector<int> results;
        results.reserve(input.size());
        
        for(int value : input) {
            // 模拟复杂计算
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            results.push_back(value * value);
        }
        
        return results;
    }
};

int main() {
    AsyncDataProcessor processor;
    std::vector<int> data = {1, 2, 3, 4, 5};
    
    auto future_results = processor.processData(data);
    
    // 主线程可以做其他工作
    std::cout << "Processing data in background..." << std::endl;
    
    try {
        auto results = future_results.get();
        std::cout << "Results: ";
        for(int val : results) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    } catch(...) {
        std::cerr << "Error processing data" << std::endl;
    }
    
    return 0;
}
```

### 2. 实现屏障模式

```cpp
#include <iostream>
#include <future>
#include <memory>
#include <vector>

std::future<void> createBarrier(int count) {
    auto prom = std::make_shared<std::promise<void>>();
    auto fut = prom->get_future();
    
    for(int i = 0; i < count; ++i) {
        std::async([prom, i]{
            // 模拟工作
            std::this_thread::sleep_for(std::chrono::milliseconds(100 * (i + 1)));
            std::cout << "Task " << i << " completed" << std::endl;
            
            if(i == count - 1) { // 最后一个任务完成时
                prom->set_value();
            }
        });
    }
    
    return fut;
}

int main() {
    const int num_tasks = 5;
    std::cout << "Starting " << num_tasks << " tasks..." << std::endl;
    
    auto barrier = createBarrier(num_tasks);
    barrier.wait();  // 等待所有任务完成
    
    std::cout << "All tasks completed" << std::endl;
    return 0;
}
```

## 六、最佳实践与常见陷阱

### 1. 必须遵守的规则

- **单次读取**：`std::future` 只能调用一次 `get()`
- **有效性检查**：调用成员函数前应检查 `valid()`
- **生命周期管理**：确保 `std::promise` 生命周期足够长

### 2. 性能优化建议

- 避免频繁创建 `std::async` 任务（考虑线程池）
- 对大量小任务使用 `std::launch::deferred`
- 使用 `wait_for` 替代忙等待

### 3. 常见错误与正确示例

**错误1：忽略返回值导致异步任务被阻塞**

```cpp
// 错误方式
std::async([]{ // 返回值被忽略，可能立即阻塞
    std::cout << "Fire and forget?";
});

// 正确方式（C++20前）
auto fut = std::async([]{ // 保留future对象
    std::cout << "Really async";
});
// 或者使用线程
std::thread([]{ // 真正的异步
    std::cout << "Really async";
}).detach();
```

**错误2：引用捕获局部变量**

```cpp
// 危险：引用捕获局部变量
std::future<void> bad_future() {
    int x = 42;
    return std::async([&x]{ // x可能已销毁
        std::cout << x; 
    });
}

// 正确方式：值捕获
std::future<void> good_future() {
    int x = 42;
    return std::async([x]{ // 复制x的值
        std::cout << x;
    });
}
```

**错误3：忘记处理异常**

```cpp
// 危险：忽略可能的异常
auto fut = std::async([]{ 
    throw std::runtime_error("Oops!");
    return 42;
});

// 正确方式：处理异常
try {
    auto result = fut.get();
} catch(const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```

## 七、C++20/23 新特性

### 1. `std::future` 扩展

C++20 引入了 `std::future` 的扩展功能：

- **`then` 连续调用**（未最终进入标准）
- **取消操作支持**（实验性）

### 2. `std::jthread` 与取消支持

C++20 的 `std::jthread` 提供了更好的线程管理：

```cpp
#include <iostream>
#include <thread>
#include <future>

void cancellable_task(std::stop_token stop) {
    while(!stop.stop_requested()) {
        std::cout << "Working..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "Task cancelled" << std::endl;
}

int main() {
    std::jthread worker(cancellable_task);
    
    std::this_thread::sleep_for(std::chrono::seconds(3));
    worker.request_stop(); // 请求停止
    
```