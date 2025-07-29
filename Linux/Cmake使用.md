
### 1. 设置 CMake 最低版本要求
```cmake
cmake_minimum_required(VERSION 3.10)
```
- 指定构建该项目所需的最低 CMake 版本。
- **注意**：这个命令必须在项目的最开始调用，通常放在 CMakeLists.txt 的第一行。

### 2. 定义项目名称
```cmake
project(MyProject VERSION 1.0 LANGUAGES CXX)
```
- 定义项目名称，并可指定版本号和语言（如C、CXX、Fortran等）。
- 这会设置一些变量，如 `PROJECT_NAME`, `PROJECT_VERSION` 等。
- **注意**：如果没有指定语言，默认支持C和CXX。

### 3. 添加可执行文件
```cmake
add_executable(my_app main.cpp)
```
- 将源文件编译成可执行文件。第一个参数是目标名称，后面是源文件列表。
- 可以一次添加多个源文件，如：`add_executable(my_app main.cpp util.cpp)`。

### 4. 添加库
```cmake
add_library(my_lib STATIC lib.cpp)
```
- 创建库文件。类型可以是 `STATIC`（静态库）、`SHARED`（动态库）或 `MODULE`（模块库）。
- 如果没有指定类型，默认是 `STATIC` 或通过 `BUILD_SHARED_LIBS` 变量控制（如果设置为ON，则默认创建共享库）。

### 5. 包含头文件目录
```cmake
include_directories(include)
target_include_directories(my_lib PUBLIC include)
```
- `include_directories` 用于为当前目录及子目录的所有目标添加头文件搜索路径（全局）。
- `target_include_directories` 则针对特定目标添加头文件路径，推荐使用这种方式，因为它更精确。
  - `PUBLIC` 表示不仅该目标使用，链接该目标的其他目标也使用。
  - `PRIVATE` 表示仅该目标使用。
  - `INTERFACE` 表示仅链接该目标的其他目标使用。

### 6. 链接库
```cmake
target_link_libraries(my_app PRIVATE my_lib)
```
- 将库链接到可执行文件或其他库。
- 同样有 `PUBLIC`、`PRIVATE` 和 `INTERFACE` 关键字，含义同上。
- **注意**：现代 CMake 推荐使用目标（target）为中心的指令（如 `target_include_directories` 和 `target_link_libraries`），而不是全局指令（如 `include_directories` 和 `link_directories`）。

### 7. 设置变量
```cmake
set(SOURCES src/main.cpp src/util.cpp)
```
- 设置变量。变量名通常大写。
- 使用 `${SOURCES}` 来引用变量。

### 8. 添加子目录
```cmake
add_subdirectory(src)
```
- 添加一个子目录，该子目录中应该包含另一个 CMakeLists.txt 文件。
- 子目录中的 CMakeLists.txt 会被执行。

### 9. 查找库
```cmake
find_package(OpenCV REQUIRED)
```
- 查找外部包（如 OpenCV）。`REQUIRED` 表示必须找到，否则报错。
- 成功后，通常会提供导入的目标（如 `OpenCV::OpenCV`）或变量（如 `OpenCV_INCLUDE_DIRS` 和 `OpenCV_LIBRARIES`）。
- **注意**：尽量使用导入的目标（如果可用），例如：
  ```cmake
  target_link_libraries(my_app PRIVATE OpenCV::core OpenCV::imgproc)
  ```

### 10. 设置编译选项
```cmake
add_compile_options(-Wall -Wextra)
```
- 添加编译选项（全局）。
- 或者针对特定目标：
  ```cmake
  target_compile_options(my_lib PRIVATE -Wall)
  ```

### 11. 设置目标属性
```cmake
set_target_properties(my_lib PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)
```
- 设置目标的属性，如C++标准、输出名称等。

### 12. 安装规则
```cmake
install(TARGETS my_app DESTINATION bin)
install(FILES my_header.h DESTINATION include)
```
- 定义安装规则，用于 `make install` 或构建安装包。
- `TARGETS` 用于安装目标（可执行文件、库等），`FILES` 用于安装文件。

### 13. 条件判断
```cmake
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()
```
- 条件判断，可以根据变量值执行不同的操作。

### 14. 循环
```cmake
foreach(file ${SOURCES})
    message(STATUS "source file: ${file}")
endforeach()
```
- 循环处理列表。

### 15. 打印消息
```cmake
message(STATUS "This is a status message")
```
- 打印消息。消息类型可以是 `STATUS`（一般信息）、`WARNING`、`AUTHOR_WARNING`、`SEND_ERROR`、`FATAL_ERROR` 等。

### 注意事项：
1. **作用域**：CMake 有目录作用域（`add_subdirectory` 会创建新的作用域）和函数作用域。在子目录中定义的变量默认在父目录不可见，除非使用 `PARENT_SCOPE`。
2. **目标为中心的指令**：现代 CMake（3.0+）推荐使用针对特定目标的指令（如 `target_include_directories`、`target_link_libraries`）而不是全局指令（如 `include_directories`、`link_libraries`），以避免污染全局作用域。
3. **避免使用过时的指令**：如 `link_libraries`、`include_directories` 等全局指令，尽量使用目标级别的指令。
4. **最小 CMake 版本**：根据你使用的特性设置合适的 `cmake_minimum_required`。
5. **变量引用**：使用 `${}` 引用变量，但在条件判断中不需要。
6. **路径分隔符**：CMake 使用 `/` 作为路径分隔符，即使在 Windows 上也是如此。
7. **区分大小写**：CMake 的指令、变量名和参数是大小写不敏感的，但变量名和文件名是大小写敏感的（在Unix系统上）。
8. **构建类型**：常用的构建类型有 Debug、Release、RelWithDebInfo 和 MinSizeRel。可以通过 `set(CMAKE_BUILD_TYPE Debug)` 设置（注意：对于多配置生成器如Visual Studio，则不需要设置，而是在生成时指定）。
9. **生成器表达式**：CMake 支持生成器表达式（Generator Expressions），用于在生成构建系统时进行条件判断，例如：
   ```cmake
   target_compile_definitions(my_lib PRIVATE $<$<CONFIG:Debug>:DEBUG_MODE>)
   ```
10. **外部依赖**：使用 `find_package` 时，注意包是否提供导入目标（现代包通常提供），这样能自动传递依赖（如头文件目录和链接库）。

### 示例：一个简单的项目结构
假设项目结构如下：
```
project/
├── CMakeLists.txt
├── include/
│   └── utils.h
├── src/
│   ├── main.cpp
│   └── utils.cpp
└── lib/
    └── CMakeLists.txt
```

顶层 CMakeLists.txt:
```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject VERSION 1.0)

# 添加子目录，编译库
add_subdirectory(lib)

# 添加可执行文件
add_executable(my_app src/main.cpp)

# 包含头文件目录
target_include_directories(my_app PRIVATE include)

# 链接库
target_link_libraries(my_app PRIVATE my_lib)
```

lib/CMakeLists.txt:
```cmake
# 添加库
add_library(my_lib STATIC ../src/utils.cpp)

# 库的头文件目录
target_include_directories(my_lib PUBLIC ../include)
```


### CMakeLists.txt 常用指令详解与快速上手指南

---

#### **一、基础指令**
1. **`cmake_minimum_required(VERSION x.x)`**  
   - **作用**：指定最低 CMake 版本  
   - **示例**：`cmake_minimum_required(VERSION 3.10)`  
   - **注意**：**必须放在第一行**，避免兼容性问题。

2. **`project(<PROJECT_NAME> [LANGUAGES])`**  
   - **作用**：定义项目名称和支持语言  
   - **示例**：`project(MyApp LANGUAGES C CXX)`  
   - **注意**：  
     - 自动生成变量 `PROJECT_NAME`、`PROJECT_SOURCE_DIR`  
     - 未指定语言时默认启用 `C` 和 `CXX`

3. **`add_executable(<TARGET> <sources>)`**  
   - **作用**：生成可执行文件  
   - **示例**：  
     ```cmake
     add_executable(my_app 
         main.cpp 
         utils.cpp
     )
     ```

4. **`add_library(<TARGET> [STATIC|SHARED] <sources>)`**  
   - **作用**：创建库文件  
   - **示例**：  
     ```cmake
     add_library(math_lib STATIC math.cpp)  # 静态库
     add_library(core SHARED core.cpp)      # 动态库
     ```
   - **注意**：  
     - 默认生成静态库（可通过 `BUILD_SHARED_LIBS=ON` 全局修改为动态库）

---

#### **二、依赖管理**
5. **`target_link_libraries(<TARGET> <PRIVATE|PUBLIC|INTERFACE> <libs>)`**  
   - **作用**：链接库到目标  
   - **示例**：  
     ```cmake
     target_link_libraries(my_app 
         PUBLIC math_lib       # 传递依赖
         PRIVATE thread_lib    # 仅内部使用
     )
     ```
   - **关键区别**：  
     | 关键字    | 当前目标 | 依赖目标 | 下游目标 |
     |-----------|----------|----------|----------|
     | `PRIVATE` | ✓        | ✗        | ✗        |
     | `INTERFACE`| ✗        | ✓        | ✓        |
     | `PUBLIC`  | ✓        | ✓        | ✓        |

6. **`find_package(<Package> [REQUIRED])`**  
   - **作用**：查找外部依赖包  
   - **示例**：  
     ```cmake
     find_package(OpenCV 4.0 REQUIRED)  # 必须找到
     target_link_libraries(my_app PRIVATE OpenCV::core)
     ```
   - **注意**：  
     - 优先使用 `IMPORTED` 目标（如 `OpenCV::core`）而非旧式变量（`${OpenCV_LIBS}`）

7. **`target_include_directories(<TARGET> <INTERFACE|PUBLIC|PRIVATE> <dirs>)`**  
   - **作用**：添加头文件搜索路径  
   - **示例**：  
     ```cmake
     target_include_directories(math_lib
         PUBLIC include     # 使用者自动包含此路径
         PRIVATE src        # 仅内部使用
     )
     ```

---

#### **三、文件与目录操作**
8. **`aux_source_directory(<DIR> <VAR>)`**  
   - **作用**：收集目录下的源文件  
   - **示例**：`aux_source_directory(src SOURCE_LIST)`  
   - **注意**：**慎用！** 不会自动检测新增/删除文件，推荐显式列出源文件。

9. **`file(GLOB <VAR> [LIST_DIRECTORIES] <pattern>)`**  
   - **作用**：通配符匹配文件  
   - **示例**：  
     ```cmake
     file(GLOB SOURCES "src/*.cpp")  # 匹配所有cpp
     ```
   - **警告**：**避免用于源文件**！可能导致构建系统不感知新增文件。

10. **`include_directories(<dirs>)`**  
    - **作用**：全局添加头文件路径（旧式）  
    - **替代方案**：优先使用 `target_include_directories()`

---

#### **四、编译控制**
11. **`target_compile_definitions(<TARGET> <INTERFACE|PUBLIC|PRIVATE> <defs>)`**  
    - **作用**：添加预处理器宏  
    - **示例**：  
      ```cmake
      target_compile_definitions(my_app 
          PRIVATE USE_DEBUG=1
      )
      ```

12. **`target_compile_options(<TARGET> <INTERFACE|PUBLIC|PRIVATE> <options>)`**  
    - **作用**：添加编译选项  
    - **示例**：  
      ```cmake
      target_compile_options(math_lib 
          PRIVATE -Wall -Wextra
      )
      ```

13. **`set_target_properties(<TARGET> PROPERTIES <key> <value>)`**  
    - **作用**：设置目标属性  
    - **示例**：  
      ```cmake
      set_target_properties(my_app PROPERTIES
          CXX_STANDARD 17        # 强制C++17
          OUTPUT_NAME "myapp"    # 输出文件名
      )
      ```

---

#### **五、安装与打包**
14. **`install(TARGETS <target>)`**  
    - **作用**：定义安装规则  
    - **示例**：  
      ```cmake
      install(TARGETS my_app
          RUNTIME DESTINATION bin   # 可执行文件
          LIBRARY DESTINATION lib   # 动态库
          ARCHIVE DESTINATION lib   # 静态库
      )
      install(FILES include/utils.h DESTINATION include)
      ```

15. **`add_subdirectory(<dir>)`**  
    - **作用**：添加子目录构建  
    - **示例**：`add_subdirectory(src)`

---

### **关键注意事项**
1. **作用域规则**  
   - `add_subdirectory()` 会创建新作用域，父作用域变量需用 `PARENT_SCOPE` 传递  
   ```cmake
   set(MY_VAR "value" PARENT_SCOPE)  # 传递到父目录
   ```

2. **现代 CMake 最佳实践**  
   - **禁止**使用全局指令：  
     ```cmake
     include_directories()  # ✗ 旧式
     link_directories()     # ✗ 旧式
     ```
   - **推荐**使用目标属性：  
     ```cmake
     target_include_directories()  # ✓
     target_link_libraries()       # ✓
     ```

3. **生成器表达式**  
   - 动态生成构建系统内容（条件编译、路径转换等）：  
     ```cmake
     target_compile_definitions(app PRIVATE
         $<$<CONFIG:Release>:NDEBUG=1>  # Release模式定义NDEBUG
     )
     ```

4. **跨平台路径处理**  
   - 使用 `${CMAKE_CURRENT_SOURCE_DIR}` 代替相对路径  
   - 避免硬编码路径分隔符（`/` 或 `\`）

5. **构建类型控制**  
   - 设置默认构建类型：  
     ```cmake
     if(NOT CMAKE_BUILD_TYPE)
         set(CMAKE_BUILD_TYPE Release)  # 未指定时设为Release
     endif()
     ```

---

### **完整示例**
```cmake
cmake_minimum_required(VERSION 3.10)
project(MyApp VERSION 1.0 LANGUAGES CXX)

# 添加可执行文件
add_executable(my_app main.cpp)

# 添加静态库
add_library(math STATIC math.cpp)
target_include_directories(math PUBLIC include)

# 链接库和依赖
find_package(Threads REQUIRED)
target_link_libraries(my_app 
    PRIVATE 
        math 
        Threads::Threads
)

# 设置C++标准
set_target_properties(my_app PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# 安装规则
install(TARGETS my_app DESTINATION bin)
```

> 通过此指南，您可快速掌握 CMake 核心用法。始终遵循 **目标为中心（Target-centric）** 的现代 CMake 理念，可显著提升项目的可维护性和跨平台兼容性。