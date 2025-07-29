
### **一、基础路径变量**
1. **`CMAKE_RUNTIME_OUTPUT_DIRECTORY`**  
   **作用**：控制所有**可执行文件**（如 `.exe`）的输出路径。  
   **示例**：
   ```cmake
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
   # 所有可执行文件输出到项目根目录下的 bin 文件夹
   ```

2. **`CMAKE_LIBRARY_OUTPUT_DIRECTORY`**  
   **作用**：控制所有**共享库/动态库**（如 `.dll`, `.so`）的输出路径。  
   **示例**：
   ```cmake
   set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
   # 所有动态库输出到 lib 文件夹
   ```

3. **`CMAKE_ARCHIVE_OUTPUT_DIRECTORY`**  
   **作用**：控制所有**静态库**（如 `.lib`, `.a`）的输出路径。  
   **示例**：
   ```cmake
   set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/static)
   # 静态库输出到 lib/static 子目录
   ```

---

### **二、按构建类型分类的路径**
4. **`CMAKE_RUNTIME_OUTPUT_DIRECTORY_<CONFIG>`**  
   **作用**：针对特定构建类型（如 `Debug`, `Release`）设置可执行文件路径。  
   **示例**：
   ```cmake
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug)
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)
   ```

5. **`CMAKE_LIBRARY_OUTPUT_DIRECTORY_<CONFIG>`**  
   **作用**：按构建类型设置动态库路径。  
   **示例**：
   ```cmake
   set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/Debug)
   ```

6. **`CMAKE_ARCHIVE_OUTPUT_DIRECTORY_<CONFIG>`**  
   **作用**：按构建类型设置静态库路径。  
   **示例**：
   ```cmake
   set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/Release/static)
   ```

---

### **三、旧版兼容变量（不推荐）**
7. **`EXECUTABLE_OUTPUT_PATH`**  
   **作用**：旧版控制可执行文件路径（会被 `CMAKE_RUNTIME_OUTPUT_DIRECTORY` 覆盖）。  
   **示例**：  
   ```cmake
   set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/out)
   ```

8. **`LIBRARY_OUTPUT_PATH`**  
   **作用**：旧版控制库文件路径（会被 `CMAKE_LIBRARY_OUTPUT_DIRECTORY` 和 `CMAKE_ARCHIVE_OUTPUT_DIRECTORY` 覆盖）。  
   **示例**：
   ```cmake
   set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
   ```

> **注意**：旧版变量无法区分动态库/静态库，且不支持按构建类型分类，建议优先使用新版变量。

---

### **四、完整配置示例**
```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject)

# 设置全局输出路径（所有构建类型）
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/static)

# 按构建类型覆盖路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/Release/static)

# 添加可执行文件
add_executable(app_main main.cpp)

# 添加动态库
add_library(my_shared SHARED shared.cpp)

# 添加静态库
add_library(my_static STATIC static.cpp)
```

---

### **关键注意事项**
1. **作用域**：  
   路径变量需在 `add_executable()` 或 `add_library()` **之前**设置。
2. **路径覆盖规则**：  
   若同时设置 `CMAKE_XXX_OUTPUT_DIRECTORY` 和 `CMAKE_XXX_OUTPUT_DIRECTORY_<CONFIG>`，  
   则特定构建类型（如 `Debug`）的路径优先级更高。
3. **生成器表达式**：  
   高级场景可用生成器表达式动态指定路径：
   ```cmake
   set_target_properties(app_main PROPERTIES
     RUNTIME_OUTPUT_DIRECTORY $<IF:$<CONFIG:Debug>,debug_path,release_path>
   )
   ```

---

### **总结表格**
| **变量名**                                  | **控制内容**       | **是否区分构建类型** |
|---------------------------------------------|-------------------|---------------------|
| `CMAKE_RUNTIME_OUTPUT_DIRECTORY`            | 可执行文件（.exe） | ❌                  |
| `CMAKE_RUNTIME_OUTPUT_DIRECTORY_<CONFIG>`   | 可执行文件        | ✅ (e.g., `_DEBUG`) |
| `CMAKE_LIBRARY_OUTPUT_DIRECTORY`            | 动态库（.dll/.so）| ❌                  |
| `CMAKE_LIBRARY_OUTPUT_DIRECTORY_<CONFIG>`   | 动态库            | ✅                  |
| `CMAKE_ARCHIVE_OUTPUT_DIRECTORY`            | 静态库（.lib/.a） | ❌                  |
| `CMAKE_ARCHIVE_OUTPUT_DIRECTORY_<CONFIG>`   | 静态库            | ✅                  |
| `EXECUTABLE_OUTPUT_PATH` (旧版)             | 可执行文件        | ❌                  |
| `LIBRARY_OUTPUT_PATH` (旧版)                | 所有库文件        | ❌                  |

