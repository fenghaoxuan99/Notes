




## target_include_directories
`target_include_directories` 用于指定目标（可执行文件或库）的包含路径。这个命令为特定的目标添加头文件搜索路径，这些路径会在编译时被编译器使用。
`target_include_directories` 命令的基本用法：
### 语法
```cmake
target_include_directories(<target>
    <INTERFACE|PUBLIC|PRIVATE> [items1...]
    [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```

- `<target>`：    指定目标名称，可以是可执行文件或库。
- `<INTERFACE|PUBLIC|PRIVATE>`：指定包含路径的作用域：
  - `INTERFACE`：当其他目标链接到这个目标时，这些包含路径会被传递给依赖目标。
  - `PUBLIC`：这些包含路径会被添加到目标本身以及依赖于该目标的其他目标。
  - `PRIVATE`：这些包含路径仅会被添加到目标本身，不会传递给依赖目标。
- `[items...]`：包含路径或变量，可以是相对路径或绝对路径，也可以是之前定义的变量。

### 示例

1. **为可执行文件添加私有包含路径**：
   ```cmake
   add_executable(MyExecutable main.cpp)
   target_include_directories(MyExecutable PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
   ```
   这里，`MyExecutable` 可执行文件被添加了一个私有包含路径，指向当前源目录下的 `include` 子目录。

2. **为库添加公共和接口包含路径**：
   ```cmake
   add_library(MyLibrary STATIC src/lib.cpp)
   target_include_directories(MyLibrary PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
   target_include_directories(MyLibrary INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/api")
   ```
   在这个例子中，`MyLibrary` 库有两个包含路径，一个是公共的（`PUBLIC`），另一个是接口的（`INTERFACE`）。公共路径将对依赖 `MyLibrary` 的目标可见，而接口路径仅当其他目标链接到 `MyLibrary` 时才会传递。

3. **使用变量**：
   ```cmake
   set(MY_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
   add_executable(MyExecutable main.cpp)
   target_include_directories(MyExecutable PRIVATE ${MY_INCLUDE_DIR})
   ```
   这里，使用了一个变量 `MY_INCLUDE_DIR` 来存储包含路径，并将其传递给 `target_include_directories`。

使用 `target_include_directories` 可以更精确地控制包含路径的作用域，从而避免不必要的依赖传递和潜在的编译问题。

## 输出设置
set (LIBRARY_OUTPUT_PATH    ${PROJECT_SOURCE_DIR}/lib)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_compile_options(-std=c++11 -Wall) 

## set_target_properties
`set_target_properties` 用来设置或修改指定目标（可执行文件、库或缓存）的属性。属性可以控制目标的各种方面，例如输出名称、链接库、编译选项等。

### 语法
```cmake
set_target_properties(<target>...
                       PROPERTIES <property> <value>...
                                 [<property> <value>...])
```
- `<target>`：目标名称或目标列表，可以是可执行文件、库或接口库。
- `PROPERTIES`：指定要设置的属性列表。
- `<property>`：属性名称。
- `<value>`：属性值。

### 常用属性

- `OUTPUT_NAME`：                设置目标的输出文件名（不包含目录路径和扩展名）。
- `ARCHIVE_OUTPUT_DIRECTORY`：   设置静态库的输出目录。
- `LIBRARY_OUTPUT_DIRECTORY`：   设置共享库的输出目录。
- `RUNTIME_OUTPUT_DIRECTORY`：   设置可执行文件的输出目录。
- `INCLUDE_DIRECTORIES`：        设置目标的包含目录。
- `COMPILE_DEFINITIONS`：        设置目标的预处理器定义。
- `COMPILE_OPTIONS`：            设置目标的编译选项。
- `LINK_LIBRARIES`：             设置目标的链接库。
- `POSITION_INDEPENDENT_CODE`：  对于共享库，设置为 `TRUE` 以启用位置无关代码（PIC）。

### 示例

假设您有一个名为 `MyTarget` 的目标，并希望设置以下属性：

- 输出名称为 `MyApp`。
- 静态库和共享库分别放在 `lib/static` 和 `lib/shared` 目录。
- 可执行文件放在 `bin` 目录。
- 添加预处理器定义 `MY_DEFINE`。
- 添加编译选项 `-Wall`。

您可以使用以下命令：

```cmake
set_target_properties(MyTarget PROPERTIES
                     OUTPUT_NAME "MyApp"
                     ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/static"
                     LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/shared"
                     RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                     COMPILE_DEFINITIONS "MY_DEFINE"
                     COMPILE_OPTIONS "-Wall")
```

这将为 `MyTarget` 设置上述属性，影响其构建过程。

请注意，`set_target_properties` 可以多次调用以修改不同的属性，并且可以在同一个调用中设置多个属性。此外，对于接口库，您可以使用 `INTERFACE` 属性前缀来设置接口属性，这些属性将被传递给依赖该接口库的目标。


## add_subdirectory(math)
添加子目录，子目录下也有一个CmakeLists.txt文件，cmake会自动执行子目录下的CmakeLists.txt文件

## 添加链接库
target_link_libraries(Demo MathFunctions)

option (USE_MYMATH
	   "Use provided math implementation" ON)




set (EXTRA_LIBS ${EXTRA_LIBS} MathFunctions)

您提供的代码片段使用了 CMake 的 `configure_file` 命令，这个命令用于将一个源文件复制到另一个位置，并在这个过程中对源文件中的变量进行替换。以下是对 `configure_file` 命令的详细解释：


## configure_file：中用于复制和替换变量的命令。

### 命令解释
- `configure_file`：CMake 中用于复制和替换变量的命令。
- `${PROJECT_SOURCE_DIR}/config.h.in`：源文件的路径，这里 `${PROJECT_SOURCE_DIR}` 是 CMake 预定义的变量，表示项目的顶层源目录。`config.h.in` 是源文件的名字，通常这个文件包含了一些变量占位符。
- `${PROJECT_BINARY_DIR}/config.h`：目标文件的路径，`${PROJECT_BINARY_DIR}` 是 CMake 预定义的变量，表示当前 CMake 项目的二进制（构建）目录。`config.h` 是复制后的文件名。

### 命令作用

1. **复制文件**：将 `${PROJECT_SOURCE_DIR}/config.h.in` 复制到 `${PROJECT_BINARY_DIR}` 目录下。
2. **替换变量**：在复制过程中，CMake 会查找文件中的变量占位符（如 `@VAR@`），并将它们替换为在 CMake 配置时定义的变量值。例如，如果有一个变量 `MY_VERSION` 在 CMakeLists.txt 中被定义为 `"1.0"`，那么文件中的 `@MY_VERSION@` 将被替换为 `"1.0"`。
3. **生成新文件**：生成的新文件名为 `config.h`，位于构建目录中指定的位置。

### 示例

假设您有一个 `config.h.in` 文件，内容如下：

```c
// config.h.in
#define MyApp_VERSION @MY_APP_VERSION@
```

在 `CMakeLists.txt` 中，您定义了一个变量并使用 `configure_file`：

```cmake
# 定义一个变量
set(MY_APP_VERSION "1.0")

# 使用 configure_file 命令
configure_file(
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)
```

执行 CMake 配置后，`config.h.in` 文件将被复制到构建目录，并重命名为 `config.h`，内容变为：

```c
// config.h
#define MyApp_VERSION 1.0
```

这样，`config.h` 文件就可以在编译时被包含，而其中的变量已经被适当替换。

### 注意事项

- `configure_file` 命令在每次运行 CMake 时都会执行，如果源文件或变量值发生变化，目标文件也会相应更新。
- 如果需要在不同构建类型或配置之间使用不同的配置文件，可以使用条件语句来设置不同的变量值。
- 从 CMake 3.10 开始，推荐使用 `configure_package_config_file` 或 `file(GENERATE)` 命令来替代 `configure_file`，因为它们提供了更多的灵活性和功能。





`find_package` 是 CMake 中一个非常强大的命令，用于在构建系统中定位外部库和软件包。这个命令会搜索指定的软件包，并根据找到的软件包设置一系列变量，这些变量可以用来指定如何编译和链接到这些软件包。

### 基本语法

```cmake
find_package(<package> [version] [EXACT] [QUIET] [MODULE]
            [REQUIRED] [[COMPONENTS] [components...]]
            [OPTIONAL_COMPONENTS components...]
            [NO_POLICY_SCOPE])
```

- `<package>`：要查找的软件包的名称。
- `version`：指定软件包的版本要求。可以是最小版本号，或者使用 `EXACT` 选项指定精确版本。
- `EXACT`：如果指定，要求找到的软件包版本必须与指定的版本完全匹配。
- `QUIET`：指定在查找过程中不输出任何诊断信息。
- `MODULE`：指定使用模块模式的查找方式，这通常意味着查找 `Find<package>.cmake` 文件。
- `REQUIRED`：如果指定，找不到软件包时，CMake 将报错并停止构建过程。
- `COMPONENTS`：指定需要查找的软件包的组件。
- `OPTIONAL_COMPONENTS`：指定可选的组件，如果这些组件找不到，不会报错。
- `NO_POLICY_SCOPE`：防止在 find_package 调用中设置的任何 CMake 策略影响全局状态。

### 示例

假设您需要在项目中使用 Qt5，您可以在 `CMakeLists.txt` 文件中使用 `find_package` 来查找 Qt5：

```cmake
cmake_minimum_required(VERSION 3.5)
project(MyQtApp)

# 查找 Qt5 核心模块，要求版本至少为 5.12
find_package(Qt5Core 5.12 REQUIRED)

# 包含 Qt5 的头文件目录
include_directories(${Qt5Core_INCLUDE_DIRS})

# 添加可执行文件
add_executable(MyQtApp main.cpp)

# 链接 Qt5 核心模块库
target_link_libraries(MyQtApp Qt5::Core)
```

在这个例子中，`find_package` 命令查找 Qt5 核心模块，并设置了一系列变量，如 `Qt5Core_INCLUDE_DIRS` 和 `Qt5::Core`。然后，您可以使用这些变量来包含头文件和链接库。

### 注意事项

- `find_package` 命令通常在项目的顶层 `CMakeLists.txt` 文件中调用，以确保找到的包对整个项目可用。
- 如果使用 `REQUIRED` 选项，当找不到指定的软件包时，CMake 将报错并停止构建过程。
- 使用 `find_package` 时，CMake 会在默认的路径和 `CMAKE_PREFIX_PATH` 指定的路径中查找软件包。可以通过设置 `CMAKE_PREFIX_PATH` 来指定额外的查找路径。
- 一些软件包可能提供了多个组件，您可以使用 `COMPONENTS` 选项来指定需要查找的组件。

`find_package` 是 CMake 构建外部依赖关系的关键工具，它使得集成第三方库变得简单而直接。
