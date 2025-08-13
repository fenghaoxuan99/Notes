

`std::put_time` 是 C++11 引入的一个时间格式化工具，用于将时间数据转换为指定格式的字符串。它通常与 `<iomanip>` 和 `<ctime>` 头文件中的函数结合使用，能够方便地将 `std::tm` 结构的时间信息输出到流中。

---

### **函数原型**
```cpp
template< class CharT >
/* 未指定类型 */ put_time(const std::tm* tmb, const CharT* fmt);
```
- **参数**：
  - `tmb`：指向 `std::tm` 结构的指针，包含时间信息。
  - `fmt`：格式字符串，定义输出格式（与 `strftime` 格式兼容）。
- **返回值**：一个未指定类型的对象，可直接用于流操作。

---

### **使用步骤**
1. **获取当前时间**：
   - 使用 `std::time` 获取当前时间的 `std::time_t` 表示。
   - 或使用 `std::chrono::system_clock` 获取高精度时间点。
2. **转换为 `std::tm` 结构**：
   - 通过 `std::localtime`（本地时间）或 `std::gmtime`（UTC 时间）转换。
3. **格式化输出**：
   - 使用 `std::put_time` 将 `std::tm` 按指定格式输出到流。

---

### **格式说明符**
常用格式符（完整列表参考 `strftime` 文档）：
- `%Y`：四位年份（如 2024）
- `%m`：两位月份（01-12）
- `%d`：两位日期（01-31）
- `%H`：24小时制小时（00-23）
- `%M`：分钟（00-59）
- `%S`：秒（00-60）
- `%F`：等价于 `%Y-%m-%d`
- `%T`：等价于 `%H:%M:%S`
- `%A`：完整星期名（如 Monday）
- `%B`：完整月份名（如 January）

---

### **示例代码**

#### **示例 1：输出当前本地时间**
```cpp
#include <iostream>
#include <iomanip>
#include <ctime>

int main() {
    std::time_t t = std::time(nullptr);          // 获取当前时间
    std::tm tm = *std::localtime(&t);            // 转换为本地时间的 tm 结构
    // 格式化为 "YYYY-MM-DD HH:MM:SS"
    std::cout << std::put_time(&tm, "%F %T") << std::endl;
    return 0;
}
```

#### **示例 2：使用 chrono 库获取时间**
```cpp
#include <iostream>
#include <iomanip>
#include <chrono>

int main() {
    auto now = std::chrono::system_clock::now(); // 高精度时间点
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&t);
    // 格式化为本地日期和时间（如 "Mon Jun 15 10:30:45 2024"）
    std::cout << std::put_time(&tm, "%c") << std::endl;
    return 0;
}
```

#### **示例 3：自定义时间并格式化**
```cpp
#include <iostream>
#include <iomanip>
#include <ctime>

int main() {
    std::tm tm = {};
    tm.tm_year = 2024 - 1900;  // 年份从 1900 开始计算
    tm.tm_mon = 6 - 1;         // 月份从 0 开始（6 表示七月）
    tm.tm_mday = 15;
    tm.tm_hour = 10;
    tm.tm_min = 30;
    tm.tm_sec = 0;
    std::mktime(&tm);          // 标准化时间（计算星期等信息）

    // 格式化为 "Saturday, July 15, 2024 10:30 AM"
    std::cout << std::put_time(&tm, "%A, %B %d, %Y %I:%M %p") << std::endl;
    return 0;
}
```

#### **示例 4：输出到字符串流**
```cpp
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

int main() {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::stringstream ss;
    ss << std::put_time(&tm, "Today is %A, %B %d.");
    std::cout << ss.str() << std::endl;
    return 0;
}
```

---

### **注意事项**
1. **线程安全性**：
   - `std::localtime` 和 `std::gmtime` 返回静态内存指针，多线程环境下应使用线程安全版本（如 `localtime_r`，但需平台支持）。
2. **错误处理**：
   - 若传入无效的 `std::tm` 指针，行为未定义。
3. **格式兼容性**：
   - 不同平台可能支持不同的格式符（如 `%z` 时区）。

