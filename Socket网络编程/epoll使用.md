# C++高级工程师知识体系：epoll深度解析与实战指南

## 1. epoll核心概念

### 1.1 epoll是什么

epoll是Linux内核实现的一种高效的I/O多路复用机制，全称为eventpoll。它解决了传统select/poll在大量并发连接时性能下降的问题，是构建高性能网络服务器的关键技术。

**核心优势对比：**
- **数据结构**：select/poll使用线性表，epoll使用红黑树
- **效率**：select/poll O(n)复杂度，epoll O(1)复杂度
- **内存**：select/poll需要用户/内核空间拷贝，epoll使用共享内存
- **就绪通知**：select/poll需要轮询判断，epoll直接返回就绪列表

### 1.2 适用场景

epoll特别适合以下场景：
- 高并发连接（C10K及以上问题）
- 长连接服务（如即时通讯、游戏服务器）
- 需要处理大量网络事件的系统

## 2. epoll API详解

### 2.1 核心三函数

#### 2.1.1 epoll_create

```c
int epoll_create(int size);
```
- 功能：创建epoll实例，返回文件描述符
- 参数：size（内核2.6.8后忽略，但必须>0）
- 返回值：成功返回epoll fd，失败返回-1

#### 2.1.2 epoll_ctl

```c
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
```

**关键数据结构：**
```c
typedef union epoll_data {
    void    *ptr;
    int      fd;
    uint32_t u32;
    uint64_t u64;
} epoll_data_t;

struct epoll_event {
    uint32_t     events;  // 监听的事件类型
    epoll_data_t data;    // 用户数据
};
```

**操作类型：**
- EPOLL_CTL_ADD：添加监控
- EPOLL_CTL_MOD：修改监控
- EPOLL_CTL_DEL：删除监控

**事件类型：**
- EPOLLIN：可读事件
- EPOLLOUT：可写事件
- EPOLLERR：错误事件
- EPOLLET：边沿触发模式

#### 2.1.3 epoll_wait

```c
int epoll_wait(int epfd, struct epoll_event *events,
               int maxevents, int timeout);
```
- 功能：等待事件发生
- 参数：
  - events：输出参数，存储就绪事件
  - maxevents：events数组大小
  - timeout：超时时间（ms），-1表示阻塞
- 返回值：就绪事件数量

## 3. epoll实战应用

### 3.1 基础服务器实现

```c
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

int main() {
    // 1. 创建监听socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    // 2. 绑定地址
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(8080),
        .sin_addr.s_addr = INADDR_ANY
    };
    bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    // 3. 监听
    listen(listen_fd, SOMAXCONN);
    
    // 4. 创建epoll实例
    int epoll_fd = epoll_create1(0);
    
    // 5. 添加监听socket到epoll
    struct epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN;
    ev.data.fd = listen_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev);
    
    while(1) {
        // 6. 等待事件
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        
        for(int i = 0; i < nfds; i++) {
            if(events[i].data.fd == listen_fd) {
                // 7. 处理新连接
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int client_fd = accept(listen_fd, 
                    (struct sockaddr*)&client_addr, &client_len);
                
                // 8. 添加新连接到epoll
                ev.events = EPOLLIN;
                ev.data.fd = client_fd;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
            } else {
                // 9. 处理客户端数据
                char buffer[BUFFER_SIZE];
                int n = read(events[i].data.fd, buffer, BUFFER_SIZE);
                if(n <= 0) {
                    // 连接关闭或出错
                    close(events[i].data.fd);
                } else {
                    // 处理数据
                    write(events[i].data.fd, buffer, n);
                }
            }
        }
    }
    
    close(epoll_fd);
    close(listen_fd);
    return 0;
}
```

### 3.2 高级特性实现

#### 3.2.1 边沿触发(ET)模式

```c
// 设置ET模式
ev.events = EPOLLIN | EPOLLET;

// 必须将socket设为非阻塞
int flags = fcntl(fd, F_GETFL, 0);
fcntl(fd, F_SETFL, flags | O_NONBLOCK);

// ET模式下的读处理
while(1) {
    ssize_t count = read(fd, buf, sizeof(buf));
    if (count == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // 数据已读完
            break;
        }
        // 处理错误
        break;
    } else if (count == 0) {
        // 连接关闭
        break;
    }
    // 处理数据
}
```

#### 3.2.2 高性能服务器架构

```c
// 线程池+epoll的服务器架构
void* worker_thread(void* arg) {
    int epoll_fd = *(int*)arg;
    struct epoll_event events[MAX_EVENTS_PER_THREAD];
    
    while(1) {
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS_PER_THREAD, -1);
        for(int i = 0; i < n; i++) {
            // 处理事件
            if(events[i].events & EPOLLIN) {
                handle_read(events[i].data.fd);
            }
            if(events[i].events & EPOLLOUT) {
                handle_write(events[i].data.fd);
            }
        }
    }
    return NULL;
}
```

## 4. 性能优化技巧

### 4.1 内存管理优化

```c
// 使用内存池管理连接数据
typedef struct {
    int fd;
    char* read_buf;
    char* write_buf;
    // 其他连接状态
} connection_t;

// 预分配连接对象池
connection_t* conn_pool = malloc(MAX_CONNECTIONS * sizeof(connection_t));
```

### 4.2 事件处理优化

```c
// 批量处理就绪事件
int batch_size = 16;
struct epoll_event* batch_events = malloc(batch_size * sizeof(struct epoll_event));

while(1) {
    int n = epoll_wait(epfd, batch_events, batch_size, 0);
    if(n > 0) {
        process_batch(batch_events, n);
    } else if(n == 0) {
        // 没有事件，可以做其他工作
        do_background_tasks();
    }
}
```

### 4.3 多核扩展

```c
// 每个CPU核心一个epoll实例
int num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
pthread_t threads[num_cpus];
int epoll_fds[num_cpus];

for(int i = 0; i < num_cpus; i++) {
    epoll_fds[i] = epoll_create1(0);
    pthread_create(&threads[i], NULL, worker_thread, &epoll_fds[i]);
}

// 使用SO_REUSEPORT实现负载均衡
setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(int));
```

## 5. 常见问题解决方案

### 5.1 惊群问题

```c
// 使用EPOLLEXCLUSIVE标志
ev.events = EPOLLIN | EPOLLEXCLUSIVE;
epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev);
```

### 5.2 连接泄漏检测

```c
// 定期检查空闲连接
void check_idle_connections() {
    time_t now = time(NULL);
    for(int i = 0; i < MAX_CONNECTIONS; i++) {
        if(connections[i].last_active + IDLE_TIMEOUT < now) {
            close(connections[i].fd);
            epoll_ctl(epfd, EPOLL_CTL_DEL, connections[i].fd, NULL);
        }
    }
}
```

### 5.3 性能监控

```c
// 通过/proc文件系统获取epoll统计
void monitor_epoll_stats(int epfd) {
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/fdinfo/%d", getpid(), epfd);
    
    FILE* fp = fopen(path, "r");
    if(fp) {
        char line[256];
        while(fgets(line, sizeof(line), fp)) {
            if(strstr(line, "tfd")) {
                printf("Epoll fd info: %s", line);
            }
        }
        fclose(fp);
    }
}
```

## 6. 进阶应用场景

### 6.1 实现异步DNS解析

```c
// 使用epoll管理DNS查询socket
int dns_sock = socket(AF_INET, SOCK_DGRAM, 0);

struct epoll_event ev;
ev.events = EPOLLIN | EPOLLET;
ev.data.fd = dns_sock;
epoll_ctl(epfd, EPOLL_CTL_ADD, dns_sock, &ev);

// 发送DNS查询后通过epoll等待响应
```

### 6.2 实现WebSocket服务器

```c
// WebSocket握手完成后
void upgrade_to_websocket(int client_fd) {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
    ev.data.fd = client_fd;
    epoll_ctl(epfd, EPOLL_CTL_MOD, client_fd, &ev);
    
    // 设置websocket处理回调
    set_websocket_handler(client_fd, ws_message_handler);
}
```

### 6.3 实现HTTP/2服务器

```c
// 多路复用的HTTP/2连接
void handle_http2_connection(int fd) {
    // 每个HTTP/2流作为一个逻辑连接
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = http2_stream;  // 使用ptr存储流对象
    
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}
```

## 7. 性能测试与调优

### 7.1 基准测试方法

```bash
# 使用wrk进行压力测试
wrk -t12 -c4000 -d30s http://localhost:8080/

# 监控epoll性能
perf stat -e 'syscalls:sys_enter_epoll*' ./server
```

### 7.2 关键性能指标

1. **事件处理延迟**：从事件发生到处理完成的时间
2. **吞吐量**：每秒处理的请求数
3. **并发连接数**：系统能维持的最大活跃连接
4. **CPU利用率**：epoll处理线程的CPU使用率

### 7.3 调优参数

```c
// 调整内核参数
echo 100000 > /proc/sys/fs/epoll/max_user_watches
echo 65535 > /proc/sys/net/core/somaxconn

// 调整TCP参数
int enable = 1;
setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &enable, sizeof(enable));
setsockopt(fd, SOL_TCP, TCP_QUICKACK, &enable, sizeof(enable));
```
