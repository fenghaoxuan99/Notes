# GDB 调试器全面指南

## 一、GDB 基础准备

### 1. 编译调试版本程序
```bash
# 必须添加 -g 选项生成调试信息
gcc -g program.c -o program

# 推荐添加额外选项
gcc -g -O0 -Wall program.c -o program
```
- `-g`：生成调试符号
- `-O0`：禁用优化（避免调试时代码行不对应）
- `-Wall`：开启所有警告

### 2. 启动与退出 GDB
```bash
# 启动GDB调试程序
gdb ./program

# 退出GDB
(gdb) quit   # 或简写 q
```

### 3. 命令行参数设置
```bash
# 设置程序启动参数
(gdb) set args arg1 arg2 ...

# 查看已设置参数
(gdb) show args
```

## 二、程序执行控制

### 1. 启动程序
```bash
# 运行程序直到第一个断点/结束
(gdb) run     # 简写 r

# 启动并停在main函数开始处
(gdb) start
```

### 2. 继续执行
```bash
# 继续运行直到下一个断点
(gdb) continue   # 简写 c

# 执行指定次数continue
(gdb) continue N
```

## 三、断点管理

### 1. 设置断点
```bash
# 当前文件行号断点
(gdb) break 10    # 简写 b 10

# 函数断点
(gdb) break main

# 指定文件断点
(gdb) break file.c:15

# 条件断点
(gdb) break 20 if i==5
```

### 2. 查看断点
```bash
(gdb) info breakpoints   # 简写 i b
```
输出示例：
```
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x000000000040054a in main at test.c:10
```

### 3. 断点操作
```bash
# 删除断点
(gdb) delete 2      # 删除2号断点
(gdb) delete        # 删除所有断点

# 禁用/启用断点
(gdb) disable 1     # 禁用1号断点
(gdb) enable 1      # 启用1号断点

# 临时断点（命中后自动删除）
(gdb) tbreak 15
```

## 四、单步调试

### 1. 基本单步命令
```bash
# 单步执行（进入函数）
(gdb) step      # 简写 s

# 单步执行（不进入函数）
(gdb) next      # 简写 n

# 执行完当前函数并返回
(gdb) finish

# 继续执行到指定行
(gdb) until 20
```

### 2. 高级单步控制
```bash
# 反向调试（需要记录执行历史）
(gdb) reverse-step
(gdb) reverse-next

# 跳过代码块执行
(gdb) jump 25    # 直接跳到25行
```

## 五、变量查看与修改

### 1. 查看变量值
```bash
# 打印变量值
(gdb) print var   # 简写 p var

# 格式化输出
(gdb) p/x var     # 十六进制
(gdb) p/d var     # 十进制
(gdb) p/t var     # 二进制
(gdb) p/c var     # 字符
```

### 2. 自动显示变量
```bash
# 设置自动显示
(gdb) display var
(gdb) display/i $pc   # 显示PC指针汇编

# 查看自动显示列表
(gdb) info display

# 取消自动显示
(gdb) undisplay 1
```

### 3. 修改变量值
```bash
(gdb) set var i=10
(gdb) set var ptr=0x8048000
```

## 六、堆栈与函数调用

### 1. 调用栈查看
```bash
# 查看调用栈
(gdb) backtrace   # 简写 bt

# 查看详细栈帧
(gdb) backtrace full
```

### 2. 栈帧操作
```bash
# 查看当前栈帧
(gdb) frame       # 简写 f

# 选择特定栈帧
(gdb) frame 2     # 切换到2号栈帧

# 查看栈帧参数
(gdb) info args
(gdb) info locals
```

## 七、内存与寄存器操作

### 1. 内存查看
```bash
# 查看内存内容
(gdb) x/10xw 0x8048000  # 16进制显示10个字
(gdb) x/20cb buf        # 显示20个字符

# 常用x命令格式
x/[数量][格式][单位] 地址
```

### 2. 寄存器操作
```bash
# 查看所有寄存器
(gdb) info registers

# 查看特定寄存器
(gdb) p $eax

# 修改寄存器值
(gdb) set $eax=0
```

## 八、高级调试技巧

### 1. 观察点（Watchpoint）
```bash
# 设置写观察点
(gdb) watch var

# 设置读观察点
(gdb) rwatch var

# 设置读写观察点
(gdb) awatch var
```

### 2. 捕获点（Catchpoint）
```bash
# 捕获系统调用
(gdb) catch syscall open

# 捕获信号
(gdb) catch signal SIGSEGV
```

### 3. 多线程调试
```bash
# 查看线程
(gdb) info threads

# 切换线程
(gdb) thread 2

# 所有线程执行命令
(gdb) thread apply all bt
```

## 九、GDB 实用配置

### 1. 配置文件 .gdbinit
```bash
# 常用配置示例
set history save on
set print pretty on
set disassembly-flavor intel
define hook-run
    break main
end
```

### 2. 常用用户定义命令
```bash
# 定义查看数组命令
define parray
    set $i = 0
    while $i < $arg0
        printf "array[%d] = %d\n", $i, $arg1[$i]
        set $i = $i + 1
    end
end

# 使用定义命令
(gdb) parray 10 array
```

## 十、图形界面与扩展

### 1. 文本界面模式
```bash
# 启动TUI模式
gdb -tui ./program
# 或运行时切换
(gdb) layout src
(gdb) layout asm
```

### 2. 增强工具
- **GDB Dashboard**：现代化GDB界面
- **PEDA**：渗透测试增强工具
- **GEF**：GDB增强功能
