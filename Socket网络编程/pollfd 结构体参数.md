# C++高级I/O多路复用：poll系统调用深度解析

## 1. struct pollfd 结构体详解

### 1.1 基本结构
```c
struct pollfd {
    int   fd;         // 文件描述符
    short events;     // 请求监视的事件
    short revents;    // 实际发生的事件
};
```

### 1.2 文件描述符(fd)
**核心要点：**
- 支持的文件描述符类型：
  - 标准I/O (0/1/2)
  - 套接字(socket)
  - 管道(pipe)
  - 设备文件
- 有效性检查：
  ```c
  if (fcntl(fd, F_GETFL) == -1 && errno == EBADF) {
      // 无效的文件描述符
  }
  ```
- 非阻塞模式设置：
  ```c
  int flags = fcntl(fd, F_GETFL, 0);
  fcntl(fd, F_SETFL, flags | O_NONBLOCK);
  ```

### 1.3 事件标志(events/revents)

#### 1.3.1 输入事件(events)

| 标志位      | 值    | 描述                                                                 |
|-------------|-------|----------------------------------------------------------------------|
| POLLIN      | 0x001 | 普通或优先级带数据可读                                              |
| POLLPRI     | 0x002 | 高优先级数据可读(如TCP带外数据)                                     |
| POLLOUT     | 0x004 | 写操作不会阻塞                                                      |
| POLLRDHUP   | 0x200 | 对端关闭连接(Linux特有)                                             |

**组合使用示例：**
```c
// 监视可读事件和连接关闭事件
fds.events = POLLIN | POLLRDHUP;

// 监视可读、可写和错误事件
fds.events = POLLIN | POLLOUT | POLLERR;
```

#### 1.3.2 输出事件(revents)

| 标志位      | 描述                                                                 |
|-------------|----------------------------------------------------------------------|
| POLLERR     | 设备错误(总是报告，无需设置)                                        |
| POLLHUP     | 设备挂起(总是报告)                                                  |
| POLLNVAL    | 无效请求(fd未打开)                                                  |

**事件处理优先级：**
1. POLLNVAL (最高优先级)
2. POLLERR
3. POLLHUP
4. 其他事件

## 2. poll系统调用实战

### 2.1 基础用法
```c
#include <poll.h>

int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```

**参数说明：**
- `fds`: pollfd结构数组
- `nfds`: 监视的描述符数量
- `timeout`: 超时时间(毫秒)，-1表示阻塞等待

### 2.2 完整示例：TCP服务器

```c++
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <vector>
#include <algorithm>

const int MAX_CLIENTS = 10;
const int BUFFER_SIZE = 1024;

class PollServer {
public:
    PollServer(int port) : port_(port) {
        server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd_ < 0) {
            throw std::runtime_error("socket creation failed");
        }

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port_);

        if (bind(server_fd_, (sockaddr*)&addr, sizeof(addr)) < 0) {
            throw std::runtime_error("bind failed");
        }

        if (listen(server_fd_, 5) < 0) {
            throw std::runtime_error("listen failed");
        }

        // 设置服务器socket为非阻塞
        fcntl(server_fd_, F_SETFL, O_NONBLOCK);
    }

    void run() {
        std::vector<pollfd> fds;
        fds.push_back({server_fd_, POLLIN, 0});

        while (true) {
            int ready = poll(fds.data(), fds.size(), -1);
            if (ready < 0) {
                perror("poll error");
                continue;
            }

            for (size_t i = 0; i < fds.size(); ++i) {
                if (fds[i].revents == 0) continue;

                // 处理错误事件
                if (fds[i].revents & (POLLERR | POLLHUP | POLLNVAL)) {
                    handle_error(fds[i].fd);
                    fds.erase(fds.begin() + i);
                    --i;
                    continue;
                }

                // 新连接到达
                if (fds[i].fd == server_fd_) {
                    accept_new_connection(fds);
                } 
                // 客户端数据到达
                else {
                    handle_client_data(fds[i].fd);
                }
            }
        }
    }

private:
    void accept_new_connection(std::vector<pollfd>& fds) {
        sockaddr_in client_addr{};
        socklen_t addr_len = sizeof(client_addr);
        int client_fd = accept(server_fd_, (sockaddr*)&client_addr, &addr_len);
        
        if (client_fd < 0) {
            perror("accept error");
            return;
        }

        // 设置客户端socket为非阻塞
        fcntl(client_fd, F_SETFL, O_NONBLOCK);
        
        // 添加到监视列表
        fds.push_back({client_fd, POLLIN | POLLRDHUP, 0});
        printf("New client connected: %d\n", client_fd);
    }

    void handle_client_data(int fd) {
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read = read(fd, buffer, BUFFER_SIZE - 1);
        
        if (bytes_read <= 0) {
            if (bytes_read == 0) {
                printf("Client %d disconnected\n", fd);
            } else {
                perror("read error");
            }
            close(fd);
            return;
        }

        buffer[bytes_read] = '\0';
        printf("Received from %d: %s\n", fd, buffer);
        
        // 回显数据
        write(fd, buffer, bytes_read);
    }

    void handle_error(int fd) {
        printf("Error occurred on fd %d\n", fd);
        close(fd);
    }

    int server_fd_;
    int port_;
};

int main() {
    PollServer server(8080);
    server.run();
    return 0;
}
```

### 2.3 性能优化技巧

1. **动态调整pollfd数组**
```c++
// 移除已关闭的连接
fds.erase(std::remove_if(fds.begin(), fds.end(), 
    [](const pollfd& pfd) { return pfd.fd == -1; }), 
    fds.end());
```

2. **批量处理事件**
```c++
const int MAX_EVENTS = 64;
int processed = 0;

while (processed < ready && fds.size() > 0) {
    if (fds[i].revents != 0) {
        // 处理事件
        processed++;
    }
    i = (i + 1) % fds.size();
}
```

3. **超时优化**
```c++
int timeout = 100; // 100ms超时
while (running) {
    int ready = poll(fds.data(), fds.size(), timeout);
    
    if (ready == 0) {
        // 超时处理
        handle_timeout();
        timeout = calculate_next_timeout(); // 动态调整超时
    }
    // ...处理事件
}
```

## 3. 高级主题与最佳实践

### 3.1 poll vs select vs epoll

| 特性          | poll             | select           | epoll            |
|---------------|------------------|------------------|------------------|
| 最大描述符数   | 无限制           | FD_SETSIZE(1024) | 无限制           |
| 效率          | O(n)             | O(n)             | O(1)             |
| 触发模式       | 水平触发         | 水平触发         | 水平/边缘触发    |
| 内存使用       | 用户空间数组     | 位图             | 内核维护         |

**选择建议：**
- 连接数少且跨平台：select
- 连接数中等：poll
- 高并发Linux系统：epoll

### 3.2 多线程环境下的poll

```c++
std::mutex poll_mutex;

void worker_thread(std::vector<pollfd>& fds) {
    while (true) {
        {
            std::lock_guard<std::mutex> lock(poll_mutex);
            int ready = poll(fds.data(), fds.size(), 100);
            if (ready > 0) {
                process_events(fds);
            }
        }
        // 其他处理
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
```

### 3.3 常见陷阱与解决方案

1. **文件描述符泄漏**
```c++
// 错误示例
if (poll(fds, n, timeout) > 0) {
    if (fds[0].revents & POLLHUP) {
        // 忘记close(fd)
    }
}

// 正确做法
if (fds[0].revents & POLLHUP) {
    close(fds[0].fd);
    fds[0].fd = -1; // 标记为无效
}
```

2. **事件风暴问题**
```c++
// POLLOUT会持续触发导致CPU满载
fds.events = POLLIN | POLLOUT;

// 解决方案：只在需要写时监视POLLOUT
void enable_write_notification(pollfd& pfd, bool enable) {
    if (enable) {
        pfd.events |= POLLOUT;
    } else {
        pfd.events &= ~POLLOUT;
    }
}
```

3. **跨平台兼容性**
```c++
// Windows的WSAPoll与Linux poll差异处理
#ifdef _WIN32
#define POLLRDHUP 0
// 其他兼容性处理
#endif
```

## 4. 性能与对比
### 4.1 结论
- poll在中等并发下表现良好
- 高并发场景考虑升级到epoll
- select应仅用于兼容性需求

## 5. 扩展阅读

1. **边缘触发与水平触发**
   - 水平触发(默认)：只要条件满足就会持续通知
   - 边缘触发：只在状态变化时通知一次

2. **与C++结合的现代封装**
```c++
class Poller {
public:
    void add_fd(int fd, short events) {
        pollfds_.push_back({fd, events, 0});
    }
    
    void remove_fd(int fd) {
        auto it = std::find_if(pollfds_.begin(), pollfds_.end(),
            [fd](const pollfd& pfd) { return pfd.fd == fd; });
        if (it != pollfds_.end()) {
            pollfds_.erase(it);
        }
    }
    
    std::vector<pollfd> wait(int timeout) {
        poll(pollfds_.data(), pollfds_.size(), timeout);
        return pollfds_;
    }

private:
    std::vector<pollfd> pollfds_;
};
```