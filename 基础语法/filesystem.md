
在C++17标准中，引入了`<filesystem>`库，它提供了一套用于操作文件系统的类和函数。这个库基于Boost.Filesystem，并且已经被广泛使用。使用这个库可以方便地进行文件和目录的创建、复制、移动、删除、遍历等操作。

 首先，要使用`<filesystem>`库，需要包含头文件：
 ```cpp
 #include <filesystem>
 ```

 为了简化代码，通常使用命名空间：
 ```cpp
 namespace fs = std::filesystem;
 ```

 下面详细介绍一些常用的功能：

 1. **路径操作 (Path)**
    `fs::path` 类是文件系统库的核心，它表示一个文件系统路径。它可以自动处理不同操作系统的路径分隔符（例如，Windows使用`\`，而Unix-like系统使用`/`）。

    示例：
    ```cpp
    fs::path p1 = "C:/Windows/System32"; // Windows风格
    fs::path p2 = "/usr/bin";            // Unix风格
    ```

    `path`对象可以用于拼接路径：
    ```cpp
    fs::path p = "C:/Windows";
    p /= "System32"; // 现在p是"C:/Windows/System32"
    ```

    也可以使用成员函数来获取路径的各个部分：
    ```cpp
    fs::path p = "C:/Windows/System32/kernel32.dll";
    std::cout << "root_name: " << p.root_name() << '\n';   // 例如 "C:"
    std::cout << "root_directory: " << p.root_directory() << '\n'; // "/"
    std::cout << "root_path: " << p.root_path() << '\n';   // "C:/"
    std::cout << "relative_path: " << p.relative_path() << '\n'; // "Windows/System32/kernel32.dll"
    std::cout << "parent_path: " << p.parent_path() << '\n'; // "C:/Windows/System32"
    std::cout << "filename: " << p.filename() << '\n';     // "kernel32.dll"
    std::cout << "stem: " << p.stem() << '\n';             // "kernel32"
    std::cout << "extension: " << p.extension() << '\n';   // ".dll"
    ```

 2. **文件状态**
    可以使用`fs::status`函数获取文件的状态，包括文件类型（普通文件、目录、符号链接等）和权限。

    ```cpp
    fs::path p = "some_file.txt";
    fs::file_status s = fs::status(p);
    if (fs::is_regular_file(s)) {
        std::cout << "是普通文件\n";
    } else if (fs::is_directory(s)) {
        std::cout << "是目录\n";
    }
    ```

 3. **目录操作**
    - 创建目录：`fs::create_directory`, `fs::create_directories`（递归创建）
      ```cpp
      fs::create_directory("new_dir"); // 创建单个目录，如果父目录不存在则失败
      fs::create_directories("path/to/new_dir"); // 递归创建目录
      ```

    - 删除目录或文件：`fs::remove`, `fs::remove_all`（递归删除）
      ```cpp
      fs::remove("file.txt"); // 删除文件或空目录
      fs::remove_all("path/to/dir"); // 递归删除目录及其内容
      ```

    - 重命名或移动：`fs::rename`
      ```cpp
      fs::rename("old_name", "new_name"); // 移动或重命名
      ```

    - 复制：`fs::copy`, `fs::copy_file`（复制文件）, `fs::copy_directory`（复制目录，但只复制目录本身，不复制内容，通常用递归方式复制整个目录树）
      更常用的是`fs::copy`，它可以递归复制整个目录：
      ```cpp
      // 复制文件
      fs::copy_file("source.txt", "destination.txt");
      // 复制目录（递归）
      fs::copy("source_dir", "destination_dir", fs::copy_options::recursive);
      ```

 4. **遍历目录**
    使用`fs::directory_iterator`（遍历单个目录）和`fs::recursive_directory_iterator`（递归遍历目录树）来遍历目录中的文件和子目录。

    ```cpp
    // 遍历单个目录
    for (const auto& entry : fs::directory_iterator("path/to/dir")) {
        std::cout << entry.path() << '\n';
    }

    // 递归遍历
    for (const auto& entry : fs::recursive_directory_iterator("path/to/dir")) {
        std::cout << entry.path() << '\n';
    }
    ```

    每个`entry`是一个`fs::directory_entry`对象，它包含文件路径和状态信息。可以通过`entry.path()`获取路径，通过`entry.status()`获取状态。

 5. **文件属性**
    可以获取文件的大小、最后修改时间等。

    - 文件大小：`fs::file_size`
      ```cpp
      uintmax_t size = fs::file_size("file.txt");
      ```

    - 最后修改时间：`fs::last_write_time`
      ```cpp
      auto ftime = fs::last_write_time("file.txt");
      // 可以将文件时间转换为time_t，然后再转换为本地时间
      auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
      std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);
      std::cout << "Last modified: " << std::asctime(std::localtime(&cftime));
      ```

 6. **错误处理**
    文件系统操作可能会失败（例如文件不存在、权限不足等）。库提供了两种错误处理方式：
    - 使用异常：在创建`fs::path`对象时，可以指定使用异常。或者，在调用函数时，如果发生错误，函数会抛出`fs::filesystem_error`异常。
    - 使用错误码：很多函数有重载版本，可以传入一个`std::error_code`参数，用于获取错误信息而不抛出异常。

    示例（使用错误码）：
    ```cpp
    std::error_code ec;
    fs::remove("non_existent_file", ec);
    if (ec) {
        std::cout << "Error: " << ec.message() << '\n';
    }
    ```

 7. **其他功能**
    - 判断文件是否存在：`fs::exists`
    - 获取可用空间：`fs::space`，返回一个`fs::space_info`结构，包含容量、空闲空间和可用空间。
    - 创建符号链接：`fs::create_symlink`，`fs::create_directory_symlink`
    - 获取绝对路径：`fs::absolute`
    - 获取规范路径（去除冗余的.和..，解析符号链接）：`fs::canonical`

 注意：由于文件系统操作与操作系统紧密相关，不同平台的行为可能略有差异（例如权限管理）。另外，使用递归操作时要特别小心，避免误删重要文件。

 下面是一个综合示例，展示如何遍历目录并打印文件信息：

 ```cpp
 #include <iostream>
 #include <filesystem>
 #include <iomanip>
 #include <ctime>

 namespace fs = std::filesystem;

 void print_file_info(const fs::directory_entry& entry) {
     const auto& path = entry.path();
     auto status = entry.status();
     auto ftime = entry.last_write_time();
     auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
     std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);
     std::cout << std::put_time(std::localtime(&cftime), "%Y-%m-%d %H:%M:%S") << " ";
     if (fs::is_directory(status)) {
         std::cout << "<DIR>          ";
     } else {
         std::cout << "     " << std::setw(12) << fs::file_size(path) << " ";
     }
     std::cout << path.filename() << '\n';
 }

 int main() {
     fs::path dir_path = "."; // 当前目录
     for (const auto& entry : fs::directory_iterator(dir_path)) {
         print_file_info(entry);
     }
     return 0;
 }
 ```

 这个示例会列出当前目录下的文件和目录，显示修改时间、大小（目录不显示大小）和文件名。

 在使用`<filesystem>`库时，请确保编译器支持C++17，并在编译时启用C++17（例如，GCC使用`-std=c++17`，MSVC使用`/std:c++17`）。

 注意：文件时间转换部分在C++17中可能有些复杂，因为`fs::file_time_type`的时钟不一定是系统时钟。上面的转换方法是一种常见的做法，但请注意，C++20引入了更方便的时间处理工具。

 希望这个介绍能帮助你开始使用C++的filesystem库！


### C++ Filesystem 库详解

C++17 引入了 `<filesystem>` 库，提供了一套跨平台的文件系统操作接口，极大简化了文件和目录操作。以下是核心用法详解：

---

#### **1. 基础设置**
```cpp
#include <filesystem>
namespace fs = std::filesystem;  // 简化命名空间
```

---

#### **2. 路径操作 (`fs::path`)**
核心类，自动处理平台路径差异（Windows 的 `\` 和 Unix 的 `/`）。

```cpp
fs::path p1 = "C:/Users/Test";  // 从字符串构造
fs::path p2 = R"(D:\data\file.txt)";  // 原始字符串路径

// 路径拼接
p1 /= "subdir";       // p1 = "C:/Users/Test/subdir"
fs::path p3 = p1 / "data.txt";  // "C:/Users/Test/subdir/data.txt"

// 获取路径组件
std::cout << p3.root_name() << "\n";    // "C:" (Windows)
std::cout << p3.parent_path() << "\n";  // "C:/Users/Test/subdir"
std::cout << p3.filename() << "\n";     // "data.txt"
std::cout << p3.extension() << "\n";    // ".txt"
```

---

#### **3. 文件/目录状态**
```cpp
fs::path p = "test.txt";

// 检查存在性
if (fs::exists(p)) {
    // 文件类型检查
    if (fs::is_regular_file(p)) std::cout << "普通文件\n";
    if (fs::is_directory(p)) std::cout << "目录\n";
    if (fs::is_symlink(p)) std::cout << "符号链接\n";

    // 文件属性
    auto size = fs::file_size(p);  // 文件大小（字节）
    auto mod_time = fs::last_write_time(p);  // 最后修改时间
}
```

---

#### **4. 目录操作**
```cpp
// 创建目录
fs::create_directory("new_dir");               // 单层目录
fs::create_directories("a/b/c");               // 递归创建

// 删除
fs::remove("file.txt");                        // 删除文件或空目录
fs::remove_all("temp");                        // 递归删除目录树

// 重命名/移动
fs::rename("old.txt", "new.txt");              // 重命名
fs::rename("dir1", "dir2");                    // 移动目录
```

---

#### **5. 文件操作**
```cpp
// 复制文件
fs::copy_file("src.txt", "dst.txt");           // 默认覆盖
fs::copy_file("src.txt", "dst.txt", fs::copy_options::skip_existing); // 跳过已存在

// 复制目录（递归）
fs::copy("src_dir", "dst_dir", fs::copy_options::recursive);

// 创建符号链接（需管理员权限）
fs::create_symlink("target.txt", "link.lnk");
```

---

#### **6. 目录遍历**
```cpp
// 遍历单层目录
for (const auto& entry : fs::directory_iterator(".")) {
    std::cout << entry.path() << "\n";  // 输出子项路径
}

// 递归遍历
for (const auto& entry : fs::recursive_directory_iterator("src")) {
    if (entry.is_regular_file()) {
        std::cout << "文件: " << entry.path().filename() << "\n";
    }
}
```

---

#### **7. 空间信息**
```cpp
fs::space_info info = fs::space("/");
std::cout << "总空间: " << info.capacity / (1024*1024) << " MB\n";
std::cout << "可用空间: " << info.free / (1024*1024) << " MB\n";
```

---

#### **8. 错误处理**
```cpp
try {
    fs::copy_file("nonexistent.txt", "copy.txt");
} catch (const fs::filesystem_error& e) {
    std::cerr << "错误: " << e.what() << "\n";
    std::cerr << "路径1: " << e.path1() << "\n";
    std::cerr << "路径2: " << e.path2() << "\n";
}

// 非异常方式
std::error_code ec;
fs::create_directories("/invalid/path", ec);
if (ec) std::cerr << "错误码: " << ec.message() << "\n";
```

---

#### **9. 实用函数**
```cpp
fs::path abs_path = fs::absolute("file.txt");  // 绝对路径
fs::path canon_path = fs::canonical("a/../b"); // 规范路径（解析 .. 和符号链接）
fs::path rel_path = fs::relative("/a/b", "/a"); // 相对路径 -> "b"
```

---

#### **10. 完整示例：备份目录**
```cpp
void backup_dir(const fs::path& src, const fs::path& dst) {
    fs::create_directories(dst);
    for (const auto& entry : fs::recursive_directory_iterator(src)) {
        auto rel_path = fs::relative(entry.path(), src);
        auto target = dst / rel_path;
        
        if (entry.is_directory()) {
            fs::create_directories(target);
        } else if (entry.is_regular_file()) {
            fs::copy_file(entry.path(), target, 
                fs::copy_options::overwrite_existing);
        }
    }
}
```

