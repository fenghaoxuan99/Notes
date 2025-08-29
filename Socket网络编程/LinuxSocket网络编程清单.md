# Linux Socket 网络编程全面指南

## 1. 核心数据结构详解

### 1.1 地址结构体家族

#### `struct sockaddr` (通用套接字地址结构)
```c
struct sockaddr {
    sa_family_t sa_family;    // 地址族(AF_INET/AF_INET6/AF_UNIX)
    char        sa_data[14];  // 协议地址
};
```
sockaddr_in。sin

#### `struct sockaddr_in` (IPv4专用结构)
```c
struct sockaddr_in {
    sa_family_t    sin_family; // 地址族(AF_INET)
    in_port_t      sin_port;   // 16位端口号(网络字节序)
    struct in_addr sin_addr;   // 32位IP地址
    unsigned char  sin_zero[8];// 填充字节(必须置0)
};
```

#### `struct sockaddr_in6` (IPv6专用结构)
```c
struct sockaddr_in6 {
    sa_family_t     sin6_family;   // AF_INET6
    in_port_t       sin6_port;     // 端口号(网络字节序)
    uint32_t        sin6_flowinfo; // IPv6流信息
    struct in6_addr sin6_addr;     // IPv6地址
    uint32_t        sin6_scope_id; // 接口范围ID
};
```

#### `struct sockaddr_un` (UNIX域套接字)
```c
struct sockaddr_un {
    sa_family_t sun_family;   // AF_UNIX
    char        sun_path[108];// 路径名
};
```

**使用示例：IPv4地址初始化**
```c
struct sockaddr_in server_addr;
memset(&server_addr, 0, sizeof(server_addr));
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(8080);  // 主机字节序转网络字节序
inet_pton(AF_INET, "192.168.1.100", &server_addr.sin_addr);
```

## 2. 核心API函数详解与示例

### 2.1 基础操作函数

#### `socket()` - 创建套接字
```c
int socket(int domain, int type, int protocol);
```
- domain: `AF_INET`(IPv4), `AF_INET6`(IPv6), `AF_UNIX`(本地通信)
- type: `SOCK_STREAM`(TCP), `SOCK_DGRAM`(UDP)
- protocol: 通常设为0

**示例：创建TCP套接字**
```c
int sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
}
```

#### `bind()` - 绑定地址
```c
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

**示例：绑定到本地所有接口的8080端口**
```c
struct sockaddr_in addr = {0};
addr.sin_family = AF_INET;
addr.sin_port = htons(8080);
addr.sin_addr.s_addr = INADDR_ANY;  // 监听所有网络接口

if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    perror("bind failed");
    close(sockfd);
    exit(EXIT_FAILURE);
}
```

#### `listen()` - 设置监听队列
```c
int listen(int sockfd, int backlog);
```
- backlog: 已完成连接队列的最大长度(SOMAXCONN)

#### `accept()` - 接受连接
```c
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

**完整服务端示例**
```c
int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 10);
    
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
        printf("Accepted connection from %s:%d\n", client_ip, ntohs(client_addr.sin_port));
        
        // 处理客户端请求
        char buffer[1024] = {0};
        recv(client_fd, buffer, sizeof(buffer), 0);
        printf("Received: %s\n", buffer);
        
        const char *response = "HTTP/1.1 200 OK\r\n\r\nHello from server!";
        send(client_fd, response, strlen(response), 0);
        
        close(client_fd);
    }
    
    close(server_fd);
    return 0;
}
```

### 2.2 数据传输函数

#### `send()`/`recv()` - TCP数据传输
```c
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```

**常用flags:**
- `MSG_NOSIGNAL`: 连接断开时不发送SIGPIPE信号
- `MSG_DONTWAIT`: 非阻塞操作
- `MSG_WAITALL`: 等待所有数据到达

#### `sendto()`/`recvfrom()` - UDP数据传输
```c
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen);
```

**UDP服务端示例**
```c
int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    
    while (1) {
        char buffer[1024] = {0};
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        ssize_t recv_len = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                                   (struct sockaddr*)&client_addr, &client_len);
        
        printf("Received %zd bytes from %s:%d\n", recv_len,
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        
        sendto(sockfd, buffer, recv_len, 0,
               (struct sockaddr*)&client_addr, client_len);
    }
    
    close(sockfd);
    return 0;
}
```

### 2.3 高级控制函数

#### `setsockopt()` - 设置套接字选项
```c
int setsockopt(int sockfd, int level, int optname,
               const void *optval, socklen_t optlen);
```

**常用选项:**
- `SO_REUSEADDR`: 允许重用本地地址
- `TCP_NODELAY`: 禁用Nagle算法
- `SO_RCVBUF`/`SO_SNDBUF`: 调整接收/发送缓冲区大小

**示例：设置地址重用**
```c
int optval = 1;
setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
```

#### `fcntl()` - 设置非阻塞模式
```c
int flags = fcntl(sockfd, F_GETFL, 0);
fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
```

## 3. 多路复用IO模型详解

### 3.1 select模型
```c
int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout);
```

**示例：使用select处理多个客户端**
```c
fd_set read_fds;
FD_ZERO(&read_fds);
FD_SET(server_fd, &read_fds);
int max_fd = server_fd;

while (1) {
    fd_set tmp_fds = read_fds;
    if (select(max_fd + 1, &tmp_fds, NULL, NULL, NULL) < 0) {
        perror("select error");
        continue;
    }
    
    for (int fd = 0; fd <= max_fd; fd++) {
        if (FD_ISSET(fd, &tmp_fds)) {
            if (fd == server_fd) {
                // 新连接
                int client_fd = accept(server_fd, NULL, NULL);
                FD_SET(client_fd, &read_fds);
                max_fd = client_fd > max_fd ? client_fd : max_fd;
            } else {
                // 客户端数据
                char buffer[1024];
                ssize_t n = recv(fd, buffer, sizeof(buffer), 0);
                if (n <= 0) {
                    close(fd);
                    FD_CLR(fd, &read_fds);
                } else {
                    send(fd, buffer, n, 0);
                }
            }
        }
    }
}
```

### 3.2 poll模型
```c
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```

**示例：使用poll处理多个客户端**
```c
struct pollfd fds[1024];
fds[0].fd = server_fd;
fds[0].events = POLLIN;
int nfds = 1;

while (1) {
    int ret = poll(fds, nfds, -1);
    if (ret < 0) {
        perror("poll error");
        continue;
    }
    
    for (int i = 0; i < nfds; i++) {
        if (fds[i].revents & POLLIN) {
            if (fds[i].fd == server_fd) {
                int client_fd = accept(server_fd, NULL, NULL);
                fds[nfds].fd = client_fd;
                fds[nfds].events = POLLIN;
                nfds++;
            } else {
                char buffer[1024];
                ssize_t n = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                if (n <= 0) {
                    close(fds[i].fd);
                    fds[i] = fds[nfds - 1];
                    nfds--;
                    i--;
                } else {
                    send(fds[i].fd, buffer, n, 0);
                }
            }
        }
    }
}
```

### 3.3 epoll模型
```c
int epoll_create1(int flags);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event *events,
               int maxevents, int timeout);
```

**示例：使用epoll实现高性能服务器**
```c
int epoll_fd = epoll_create1(0);
struct epoll_event event;
event.events = EPOLLIN;
event.data.fd = server_fd;
epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event);

struct epoll_event events[1024];

while (1) {
    int n = epoll_wait(epoll_fd, events, 1024, -1);
    for (int i = 0; i < n; i++) {
        if (events[i].data.fd == server_fd) {
            int client_fd = accept(server_fd, NULL, NULL);
            event.events = EPOLLIN | EPOLLET;  // 边缘触发模式
            event.data.fd = client_fd;
            epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);
        } else {
            char buffer[1024];
            ssize_t len;
            while ((len = recv(events[i].data.fd, buffer, sizeof(buffer), 0)) > 0) {
                send(events[i].data.fd, buffer, len, 0);
            }
            if (len == 0 || (len < 0 && errno != EAGAIN)) {
                close(events[i].data.fd);
            }
        }
    }
}
```

## 4. 关键网络概念深入解析

### 4.1 字节序转换
```c
uint32_t htonl(uint32_t hostlong);  // 主机到网络(32位)
uint16_t htons(uint16_t hostshort); // 主机到网络(16位)
uint32_t ntohl(uint32_t netlong);   // 网络到主机(32位)
uint16_t ntohs(uint16_t netshort);  // 网络到主机(16位)
```

### 4.2 TCP状态机与TIME_WAIT
- `TIME_WAIT`状态持续2MSL(最大报文段生存时间)
- 作用：
  - 确保最后一个ACK能到达对端
  - 让网络中旧的重复报文失效

**避免TIME_WAIT过多：**
```c
struct linger so_linger;
so_linger.l_onoff = 1;
so_linger.l_linger = 0;
setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));
```

### 4.3 滑动窗口与Nagle算法
- 滑动窗口：TCP流量控制机制
- Nagle算法：合并小数据包减少网络负载
- 禁用Nagle算法：
```c
int flag = 1;
setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
```

## 5. 调试与性能优化工具

### 5.1 网络诊断工具
```bash
# 查看网络连接状态
netstat -tulnp
ss -tulnp

# 抓包分析
tcpdump -i eth0 port 80 -w capture.pcap

# 跟踪系统调用
strace -f -e trace=network ./server
```

### 5.2 性能调优参数
```bash
# 增大本地端口范围
echo "1024 65535" > /proc/sys/net/ipv4/ip_local_port_range

# 启用TCP快速回收TIME_WAIT套接字
echo 1 > /proc/sys/net/ipv4/tcp_tw_reuse

# 调整TCP接收缓冲区大小
sysctl -w net.core.rmem_max=16777216
sysctl -w net.ipv4.tcp_rmem="4096 87380 16777216"
```

## 6. 高级应用示例

### 6.1 多线程服务器
```c
void *handle_client(void *arg) {
    int client_fd = *(int *)arg;
    free(arg);
    
    char buffer[1024];
    while (1) {
        ssize_t n = recv(client_fd, buffer, sizeof(buffer), 0);
        if (n <= 0) break;
        send(client_fd, buffer, n, 0);
    }
    
    close(client_fd);
    return NULL;
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    // ... bind和listen代码...
    
    while (1) {
        int *client_fd = malloc(sizeof(int));
        *client_fd = accept(server_fd, NULL, NULL);
        
        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, client_fd);
        pthread_detach(thread);
    }
    
    close(server_fd);
    return 0;
}
```

### 6.2 实现简单的HTTP服务器
```c
void send_response(int fd, const char *status, const char *content_type, const char *body) {
    char response[1024];
    int len = snprintf(response, sizeof(response),
        "HTTP/1.1 %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s", status, content_type, strlen(body), body);
    
    send(fd, response, len, 0);
}

int main() {
    // ... socket, bind, listen代码...
    
    while (1) {
        int client_fd = accept(server_fd, NULL, NULL);
        char buffer[1024] = {0};
        recv(client_fd, buffer, sizeof(buffer), 0);
        
        if (strstr(buffer, "GET / ")) {
            send_response(client_fd, "200 OK", "text/html", 
                "<html><body><h1>Welcome</h1></body></html>");
        } else {
            send_response(client_fd, "404 Not Found", "text/html", 
                "<html><body><h1>404 Not Found</h1></body></html>");
        }
        
        close(client_fd);
    }
    
    return 0;
}
```

## 7. 常见问题解决方案

### 7.1 处理EAGAIN/EWOULDBLOCK错误
```c
ssize_t safe_recv(int fd, void *buf, size_t len) {
    ssize_t n;
    do {
        n = recv(fd, buf, len, 0);
    } while (n == -1 && errno == EINTR);  // 处理信号中断
    
    if (n == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        return 0;  // 非阻塞模式下无数据可读
    }
    
    return n;
}
```

### 7.2 处理TCP粘包问题

TCP是流式协议，没有消息边界概念，需要应用层自己处理消息边界。

**解决方案：**
1. 固定长度消息
2. 特殊分隔符（如`\r\n\r\n`）
3. 消息头+消息体（包含长度信息）

**示例：使用长度前缀处理粘包**
```c
// 发送带长度前缀的消息
void send_message(int fd, const void *buf, size_t len) {
    uint32_t net_len = htonl(len);
    send(fd, &net_len, sizeof(net_len), 0);  // 先发长度
    send(fd, buf, len, 0);                  // 再发内容
}

// 接收带长度前缀的消息
ssize_t recv_message(int fd, void *buf, size_t buf_size) {
    uint32_t net_len;
    ssize_t n = recv(fd, &net_len, sizeof(net_len), MSG_WAITALL);
    if (n <= 0) return n;
    
    uint32_t len = ntohl(net_len);
    if (len > buf_size) {
        errno = EMSGSIZE;
        return -1;
    }
    
    return recv(fd, buf, len, MSG_WAITALL);
}
```

### 7.3 处理大量TIME_WAIT状态

当服务器主动关闭大量连接时，会产生大量TIME_WAIT状态的套接字。

**解决方案：**
1. 让客户端主动关闭连接
2. 设置SO_LINGER选项快速关闭
3. 启用tcp_tw_reuse内核参数

**示例：优雅关闭连接**
```c
void graceful_close(int sockfd) {
    // 先关闭写方向
    shutdown(sockfd, SHUT_WR);
    
    // 读取剩余数据
    char buf[1024];
    while (recv(sockfd, buf, sizeof(buf), 0) > 0);
    
    // 完全关闭
    close(sockfd);
}
```

## 8. 安全编程实践

### 8.1 防御常见攻击

#### SYN Flood攻击防护
```c
// 启用SYN Cookie保护
echo 1 > /proc/sys/net/ipv4/tcp_syncookies

// 减少SYN_RECV状态超时时间
echo 30 > /proc/sys/net/ipv4/tcp_synack_retries
```

#### 端口扫描防护
```c
// 限制SYN包速率
iptables -A INPUT -p tcp --syn -m limit --limit 1/s -j ACCEPT
```

### 8.2 安全函数替代

**不安全的函数** | **安全替代方案**
---|---
`gets()` | `fgets()`或`getline()`
`strcpy()` | `strncpy()`或`snprintf()`
`strcat()` | `strncat()`
`sprintf()` | `snprintf()`

## 9. 性能优化技巧

### 9.1 零拷贝技术

**使用sendfile()传输文件**
```c
#include <sys/sendfile.h>

void send_file(int out_fd, int in_fd, off_t offset, size_t count) {
    off_t orig_offset = offset;
    while (count > 0) {
        ssize_t sent = sendfile(out_fd, in_fd, &offset, count);
        if (sent <= 0) break;
        count -= sent;
    }
}
```

### 9.2 批量写操作

**使用writev()进行向量写**
```c
struct iovec iov[2];
iov[0].iov_base = header;
iov[0].iov_len = sizeof(header);
iov[1].iov_base = body;
iov[1].iov_len = body_len;

ssize_t n = writev(fd, iov, 2);
```

## 10. 高级主题

### 10.1 原始套接字编程

**示例：构造自定义IP包**
```c
int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

char packet[1024];
// 填充IP头部
struct iphdr *ip_header = (struct iphdr *)packet;
ip_header->version = 4;
ip_header->ihl = 5;
ip_header->tot_len = htons(sizeof(packet));
// ...其他IP头部字段...

// 填充TCP/UDP数据
// ...

sendto(sock, packet, sizeof(packet), 0, 
       (struct sockaddr*)&dest_addr, sizeof(dest_addr));
```

### 10.2 多播编程

**示例：加入多播组**
```c
struct ip_mreq mreq;
mreq.imr_multiaddr.s_addr = inet_addr("224.0.0.1");
mreq.imr_interface.s_addr = htonl(INADDR_ANY);

setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
```

## 11. 现代替代方案

### 11.1 io_uring (Linux 5.1+)

**高性能异步IO接口示例**
```c
struct io_uring ring;
io_uring_queue_init(32, &ring, 0);

struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
io_uring_prep_read(sqe, fd, buf, len, 0);
io_uring_submit(&ring);

struct io_uring_cqe *cqe;
io_uring_wait_cqe(&ring, &cqe);
// 处理完成事件
io_uring_cqe_seen(&ring, cqe);
```

### 11.2 基于事件的库比较

库 | 特点
---|---
libevent | 跨平台，接口简单
libuv | Node.js使用，支持更多功能
boost.asio | C++风格，跨平台