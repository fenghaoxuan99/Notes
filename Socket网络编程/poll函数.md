# C++高级I/O多路复用：poll函数深度解析与实践指南

## 一、poll函数全面解析

### 1. 函数原型与参数详解

```c
#include <poll.h>
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```

#### 参数深度解析

1. **fds参数**（核心结构体） 
```c
struct pollfd {
    int fd;         // 文件描述符
    short events;   // 监视的事件集合
    short revents;  // 实际发生的事件集合
};
```

- **事件标志位**：
  - `POLLIN`：普通数据可读
  - `POLLPRI`：紧急/带外数据可读
  - `POLLOUT`：可写（不阻塞）
  - `POLLERR`：错误条件（自动设置）
  - `POLLHUP`：连接断开（自动设置）
  - `POLLNVAL`：无效fd（自动设置）

2. **nfds参数**
   - 实际应用技巧：
     ```c
     // 动态计算数组有效长度
     nfds_t active_fds = 0;
     for (int i = 0; i < max_fds; i++) {
         if (fds[i].fd >= 0) active_fds++;
     }
     poll(fds, active_fds, timeout);
     ```

3. **timeout参数**
   - 实际精度测试代码：
     ```c
     struct timespec start, end;
     clock_gettime(CLOCK_MONOTONIC, &start);
     poll(NULL, 0, 100); // 100ms超时
     clock_gettime(CLOCK_MONOTONIC, &end);
     printf("Actual timeout: %ld ms\n", 
         (end.tv_sec - start.tv_sec)*1000 + 
         (end.tv_nsec - start.tv_nsec)/1000000);
     ```

### 2. 内核处理流程详解

1. **完整内核调用链**：
   ```
   sys_poll() → do_sys_poll() → do_poll() → 
   poll_get_entry() → vfs_poll() → 驱动实现的poll方法
   ```

2. **性能关键点**：
   - 每次调用需要将整个数组从用户空间拷贝到内核空间
   - 内核需要遍历所有描述符，时间复杂度O(n)
   - 唤醒后需要再次遍历所有描述符检查状态

3. **等待队列机制**：
   ```c
   // 内核中的大致实现
   add_wait_queue(&dev->wq, &wait);
   while (!need_resched()) {
       set_current_state(TASK_INTERRUPTIBLE);
       if (事件就绪) break;
       schedule_timeout(timeout);
   }
   remove_wait_queue(&dev->wq, &wait);
   ```

## 二、高级应用与最佳实践

### 1. 生产级代码模板

```c
#include <vector>
#include <algorithm>

class PollHandler {
    std::vector<pollfd> fds_;
    int timeout_;
    
public:
    void add_fd(int fd, short events) {
        fds_.push_back({fd, events, 0});
    }
    
    void remove_fd(int fd) {
        fds_.erase(std::remove_if(fds_.begin(), fds_.end(), 
            [fd](const pollfd& p) { return p.fd == fd; }), 
            fds_.end());
    }
    
    void run() {
        while (!should_stop) {
            int ready = poll(fds_.data(), fds_.size(), timeout_);
            if (ready == -1) {
                if (errno == EINTR) continue;
                throw std::runtime_error("poll error");
            }
            
            for (auto& pfd : fds_) {
                if (pfd.revents & POLLERR) {
                    handle_error(pfd.fd);
                    continue;
                }
                
                if (pfd.revents & POLLIN) {
                    handle_input(pfd.fd);
                }
                
                if (pfd.revents & POLLOUT) {
                    handle_output(pfd.fd);
                }
            }
        }
    }
};
```

### 2. 性能优化技巧

1. **动态事件管理**：
```c
// 只在需要时才监视写事件
void enable_write(int fd, bool enable) {
    for (auto& pfd : fds_) {
        if (pfd.fd == fd) {
            if (enable) pfd.events |= POLLOUT;
            else pfd.events &= ~POLLOUT;
            break;
        }
    }
}
```

2. **批量处理优化**：
```c
// 使用事件就绪列表避免全量扫描
std::vector<int> get_ready_fds() {
    std::vector<int> ready;
    for (const auto& pfd : fds_) {
        if (pfd.revents) {
            ready.push_back(pfd.fd);
            if (ready.size() >= max_batch) break;
        }
    }
    return ready;
}
```

### 3. 多线程安全实现

```c
class ThreadSafePoll {
    std::mutex mtx_;
    std::vector<pollfd> fds_;
    
public:
    void add_fd(int fd, short events) {
        std::lock_guard<std::mutex> lock(mtx_);
        fds_.push_back({fd, events, 0});
    }
    
    int wait(int timeout) {
        std::vector<pollfd> local_fds;
        {
            std::lock_guard<std::mutex> lock(mtx_);
            local_fds = fds_;
        }
        
        int ret = poll(local_fds.data(), local_fds.size(), timeout);
        if (ret > 0) {
            std::lock_guard<std::mutex> lock(mtx_);
            for (size_t i = 0; i < fds_.size(); ++i) {
                fds_[i].revents = local_fds[i].revents;
            }
        }
        return ret;
    }
};
```

## 三、典型应用场景实现

### 1. 高性能TCP服务器

```c++
class PollServer {
    int listen_fd_;
    std::vector<pollfd> clients_;
    
    void handle_new_connection() {
        sockaddr_in addr;
        socklen_t len = sizeof(addr);
        int conn_fd = accept(listen_fd_, (sockaddr*)&addr, &len);
        fcntl(conn_fd, F_SETFL, O_NONBLOCK);
        clients_.push_back({conn_fd, POLLIN, 0});
    }
    
    void handle_client_data(int fd) {
        char buf[4096];
        ssize_t n = read(fd, buf, sizeof(buf));
        if (n <= 0) {
            close(fd);
            clients_.erase(std::remove_if(clients_.begin(), clients_.end(),
                [fd](const pollfd& p) { return p.fd == fd; }), clients_.end());
            return;
        }
        // 处理业务逻辑...
    }
    
public:
    void run() {
        listen_fd_ = create_listen_socket(8080);
        pollfd listen_pfd = {listen_fd_, POLLIN, 0};
        clients_.push_back(listen_pfd);
        
        while (true) {
            int ready = poll(clients_.data(), clients_.size(), 1000);
            if (ready == -1) {
                if (errno == EINTR) continue;
                perror("poll");
                break;
            }
            
            for (size_t i = 0; i < clients_.size(); ++i) {
                if (!clients_[i].revents) continue;
                
                if (clients_[i].fd == listen_fd_) {
                    handle_new_connection();
                } else {
                    handle_client_data(clients_[i].fd);
                }
            }
        }
    }
};
```

### 2. 跨平台事件循环

```c++
class EventLoop {
    std::unordered_map<int, std::function<void(short)>> callbacks_;
    std::vector<pollfd> fds_;
    
public:
    void register_fd(int fd, short events, std::function<void(short)> cb) {
        fds_.push_back({fd, events, 0});
        callbacks_[fd] = cb;
    }
    
    void run() {
        while (!should_stop_) {
            int ready = poll(fds_.data(), fds_.size(), -1);
            if (ready <= 0) continue;
            
            for (auto& pfd : fds_) {
                if (pfd.revents && callbacks_.count(pfd.fd)) {
                    callbacks_[pfd.fd](pfd.revents);
                }
            }
        }
    }
};

// 使用示例
EventLoop loop;
loop.register_fd(socket_fd, POLLIN, [](short revents) {
    if (revents & POLLIN) {
        // 处理socket数据
    }
});
loop.run();
```

## 四、深入对比与进阶话题

### 1. poll vs select vs epoll

| 特性          | poll             | select           | epoll            |
|---------------|------------------|------------------|------------------|
| 时间复杂度    | O(n)             | O(n)             | O(1)             |
| 最大fd数      | 无硬限制         | FD_SETSIZE(1024) | 系统限制         |
| 内存使用      | 用户/内核各存一份| 仅内核存位图     | 仅内核存就绪列表 |
| 触发模式      | 水平触发         | 水平触发         | 支持边缘触发     |
| 适用场景      | 中小规模连接     | 兼容性要求       | 大规模高并发     |

### 2. 边缘触发(ET)模拟实现

虽然原生poll只支持水平触发，但可以模拟边缘触发行为：

```c++
struct ET_Poll {
    std::vector<pollfd> fds_;
    std::vector<bool> last_ready_;
    
    void add_fd(int fd, short events) {
        fds_.push_back({fd, events, 0});
        last_ready_.push_back(false);
    }
    
    int wait(int timeout) {
        int ready = poll(fds_.data(), fds_.size(), timeout);
        if (ready <= 0) return ready;
        
        int et_ready = 0;
        for (size_t i = 0; i < fds_.size(); ++i) {
            bool current = (fds_[i].revents != 0);
            if (current && !last_ready_[i]) {
                et_ready++;
            }
            last_ready_[i] = current;
        }
        return et_ready;
    }
};
```

### 3. 信号安全处理方案

```c++
void handle_signal(int sig) {
    // 设置全局标志位
    signaled = true;
}

int safe_poll(struct pollfd *fds, nfds_t nfds, int timeout) {
    sigset_t orig_mask, block_mask;
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGINT);
    sigaddset(&block_mask, SIGTERM);
    
    pthread_sigmask(SIG_BLOCK, &block_mask, &orig_mask);
    
    if (signaled) {
        pthread_sigmask(SIG_SETMASK, &orig_mask, NULL);
        errno = EINTR;
        return -1;
    }
    
    int ret = poll(fds, nfds, timeout);
    pthread_sigmask(SIG_SETMASK, &orig_mask, NULL);
    return ret;
}
```

## 五、调试与性能分析

### 1. 常见问题排查

1. **文件描述符泄漏**：
   ```bash
   # 监控poll调用的fd变化
   strace -e poll,close,dup,dup2 ./your_program
   ```

2. **性能瓶颈分析**：
   ```bash
   perf stat -e 'syscalls:sys_enter_poll' ./your_program
   perf top -p `pidof your_program`
   ```

3. **事件风暴诊断**：
   ```c
   // 添加调试日志
   printf("Poll call: nfds=%zu, timeout=%d\n", nfds, timeout);
   for (size_t i = 0; i < nfds; i++) {
       printf("  fd=%d, events=%04x\n", fds[i].fd, fds[i].events);
   }
   ```

### 2. 性能测试代码

```c++
#include <chrono>

void benchmark_poll(int max_fds) {
    std::vector<pollfd> fds(max_fds);
    for (int i = 0; i < max_fds; ++i) {
        fds[i].fd = open("/dev/null", O_RDONLY);
        fds[i].events = POLLIN;
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        poll(fds.data(), max_fds, 1);
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    std::cout << "Average poll time with " << max_fds 
              << " fds: " 
              << std::chrono::duration_cast<std::chrono::microseconds>(
                  end - start).count() / 1000.0 
              << " μs\n";
    
    for (auto& pfd : fds) close(pfd.fd);
}
```