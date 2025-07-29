
### **一、核心构建路径变量**
1. **`CMAKE_BINARY_DIR`**  
   - **含义**：构建树的**根目录**（即运行 `cmake` 命令的目录）
   - **特点**：
     - 在 `cmake -B build` 命令中，`build` 目录就是 `CMAKE_BINARY_DIR`
     - 在整个项目中保持不变（即使在使用 `add_subdirectory` 时）
   - **别名**：`PROJECT_BINARY_DIR`（在顶级项目中与之相同）
   - **示例**：
     ```cmake
     message("构建目录: ${CMAKE_BINARY_DIR}")
     # 输出示例: /home/user/project/build
     ```

2. **`CMAKE_SOURCE_DIR`**  
   - **含义**：源代码树的**根目录**（包含顶级 `CMakeLists.txt` 的目录）
   - **特点**：始终指向项目的最顶层源码目录
   - **示例**：
     ```cmake
     configure_file(
       ${CMAKE_SOURCE_DIR}/config.h.in 
       ${CMAKE_BINARY_DIR}/config.h
     )
     ```

3. **`PROJECT_BINARY_DIR`**  
   - **含义**：**当前项目**的构建目录（在子项目中与顶级构建目录不同）
   - **特点**：
     - 在 `add_subdirectory(subdir)` 中，`subdir` 有自己的 `PROJECT_BINARY_DIR`
     - 等价于 `<projectname>_BINARY_DIR`
   - **示例**：
     ```cmake
     project(MyApp)
     message("项目构建目录: ${PROJECT_BINARY_DIR}")
     # 输出: /build/MyApp
     ```

4. **`PROJECT_SOURCE_DIR`**  
   - **含义**：**当前项目**的源代码目录
   - **特点**：对于子项目，指向其自己的源码目录
   - **示例**：
     ```cmake
     add_subdirectory(lib)
     # 在 lib/CMakeLists.txt 中:
     #   PROJECT_SOURCE_DIR = /project/lib
     ```

---

### **二、当前上下文路径变量**
5. **`CMAKE_CURRENT_BINARY_DIR`**  
   - **含义**：**当前处理的** CMakeLists.txt 对应的构建目录
   - **特点**：在 `add_subdirectory` 时会自动变化
   - **示例**：
     ```cmake
     # 顶级 CMakeLists.txt
     add_subdirectory(src)
     # 在 src/CMakeLists.txt 中:
     #   CMAKE_CURRENT_BINARY_DIR = /build/src
     ```

6. **`CMAKE_CURRENT_SOURCE_DIR`**  
   - **含义**：**当前处理的** CMakeLists.txt 所在的源码目录
   - **使用场景**：引用当前目录下的文件
   - **示例**：
     ```cmake
     add_executable(app ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
     ```

---

### **三、安装路径变量**
7. **`CMAKE_INSTALL_PREFIX`**  
   - **含义**：项目**安装的根目录**（类似 Unix 的 `/usr/local`）
   - **默认值**：
     - Unix: `/usr/local`
     - Windows: `C:/Program Files`
   - **设置方式**：
     ```cmake
     set(CMAKE_INSTALL_PREFIX "/opt/myapp")
     # 或命令行: cmake -DCMAKE_INSTALL_PREFIX=/custom/path
     ```

8. **安装组件路径**：
   - `CMAKE_INSTALL_BINDIR`：可执行文件（默认 `bin`）
   - `CMAKE_INSTALL_LIBDIR`：库文件（默认 `lib`）
   - `CMAKE_INSTALL_INCLUDEDIR`：头文件（默认 `include`）
   ```cmake
   install(TARGETS myapp DESTINATION ${CMAKE_INSTALL_BINDIR})
   ```

---

### **四、其他关键变量**
9. **`CMAKE_MODULE_PATH`**  
   - **含义**：CMake 模块的搜索路径
   - **用法**：
     ```cmake
     list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
     ```

10. **`CMAKE_PREFIX_PATH`**  
    - **含义**：查找依赖包的路径（类似环境变量 `PATH`）
    - **用法**：
      ```cmake
      list(APPEND CMAKE_PREFIX_PATH "/opt/some_library")
      ```

11. **`CMAKE_BUILD_TYPE`**  
    - **含义**：构建类型（Debug/Release/等）
    - **示例**：
      ```cmake
      set(CMAKE_BUILD_TYPE Debug)
      ```

---

### **五、路径关系示意图**
```
项目目录结构:
  /project
    ├── CMakeLists.txt         (顶级)
    ├── src/
    │    ├── CMakeLists.txt    (子项目)
    │    └── main.cpp
    └── build/                 (构建目录)

变量对应关系:
  在顶级 CMakeLists.txt 中:
    CMAKE_SOURCE_DIR    = /project
    CMAKE_BINARY_DIR    = /project/build
    PROJECT_SOURCE_DIR  = /project
    PROJECT_BINARY_DIR  = /project/build

  在 src/CMakeLists.txt 中:
    CMAKE_CURRENT_SOURCE_DIR = /project/src
    CMAKE_CURRENT_BINARY_DIR = /project/build/src
```

---

### **六、最佳实践示例**
```cmake
cmake_minimum_required(VERSION 3.20)
project(MyApp)

# 设置安装路径
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# 添加子项目
add_subdirectory(src)

# 安装规则
install(TARGETS myapp
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
```

```cmake
# src/CMakeLists.txt
add_executable(myapp main.cpp)

# 使用当前目录下的头文件
target_include_directories(myapp PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)
```

---

### **总结表格**
| **变量名**                 | **含义**                          | **作用域**       | **是否变化**        |
|----------------------------|-----------------------------------|------------------|---------------------|
| `CMAKE_BINARY_DIR`         | 构建根目录                        | 全局             | ❌ 固定              |
| `CMAKE_SOURCE_DIR`         | 源码根目录                        | 全局             | ❌ 固定              |
| `PROJECT_BINARY_DIR`       | 当前项目的构建目录                | 项目作用域       | ✅ 随项目变化        |
| `PROJECT_SOURCE_DIR`       | 当前项目的源码目录                | 项目作用域       | ✅ 随项目变化        |
| `CMAKE_CURRENT_BINARY_DIR` | 当前CMakeLists的构建目录          | 当前文件作用域   | ✅ 随目录变化        |
| `CMAKE_CURRENT_SOURCE_DIR` | 当前CMakeLists的源码目录          | 当前文件作用域   | ✅ 随目录变化        |
| `CMAKE_INSTALL_PREFIX`     | 安装根目录                        | 全局             | 可配置              |
| `CMAKE_MODULE_PATH`        | CMake模块搜索路径                 | 全局             | 可扩展              |
| `CMAKE_PREFIX_PATH`        | 依赖包搜索路径                    | 全局             | 可扩展              |