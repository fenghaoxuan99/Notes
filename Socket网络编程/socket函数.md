# Socket 编程全面指南：从基础到高级应用

## 1. Socket 基础概念

### 1.1 什么是 Socket？

Socket（套接字）是网络通信的端点，是操作系统提供的用于网络编程的接口。它允许不同主机或同一主机上的不同进程之间进行数据交换。

### 1.2 Socket 通信模型

Socket 通信遵循客户端-服务器模型：
- **服务器端**：创建Socket → 绑定地址 → 监听连接 → 接受连接 → 数据交换
- **客户端**：创建Socket → 连接服务器 → 数据交换

## 2. Socket API 详解

### 2.1 socket() 函数

```c
#include <sys/types.h>
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
```

#### 参数深度解析

1. **domain (协议族)**
   - `AF_INET` (IPv4)：最常用的互联网协议
   - `AF_INET6` (IPv6)：下一代互联网协议
   - `AF_UNIX`/`AF_LOCAL`：本地进程间通信
   - `AF_PACKET`：底层数据包接口

2. **type (套接字类型)**
   - `SOCK_STREAM` (TCP)：可靠、面向连接的字节流
   - `SOCK_DGRAM` (UDP)：无连接、不可靠的数据报
   - `SOCK_RAW`：原始网络协议访问
   - `SOCK_SEQPACKET`：可靠的有序分组服务

3. **protocol (协议)**
   - `IPPROTO_TCP` (6)：TCP传输协议
   - `IPPROTO_UDP` (17)：UDP传输协议
   - `IPPROTO_ICMP` (1)：ICMP协议

### 2.2 相关系统调用

```c
// 绑定地址
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

// 监听连接
int listen(int sockfd, int backlog);

// 接受连接
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

// 建立连接
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

// 发送数据
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
              const struct sockaddr *dest_addr, socklen_t addrlen);

// 接收数据
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                struct sockaddr *src_addr, socklen_t *addrlen);

// 关闭套接字
int close(int fd);
```

## 3. 完整示例代码

### 3.1 TCP 服务器示例

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char *response = "Hello from server";

    // 创建socket文件描述符
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 设置socket选项
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 绑定socket到端口
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 开始监听
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // 接受连接
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // 读取客户端数据
    read(new_socket, buffer, BUFFER_SIZE);
    printf("Message from client: %s\n", buffer);

    // 发送响应
    send(new_socket, response, strlen(response), 0);
    printf("Response sent\n");

    // 关闭连接
    close(new_socket);
    close(server_fd);
    return 0;
}
```

### 3.2 TCP 客户端示例

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};

    // 创建socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 将IP地址从文本转换为二进制形式
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // 连接服务器
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // 发送数据
    send(sock, hello, strlen(hello), 0);
    printf("Hello message sent\n");

    // 接收响应
    read(sock, buffer, 1024);
    printf("Server response: %s\n", buffer);

    // 关闭连接
    close(sock);
    return 0;
}
```

## 4. 高级主题

### 4.1 非阻塞I/O和多路复用

```c
#include <sys/select.h>

int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout);

// 示例：使用select处理多个连接
fd_set readfds;
int max_sd, activity;

while(1) {
    FD_ZERO(&readfds);
    FD_SET(master_socket, &readfds);
    max_sd = master_socket;

    // 添加所有客户端socket到集合
    for (int i = 0; i < max_clients; i++) {
        int sd = client_socket[i];
        if (sd > 0) FD_SET(sd, &readfds);
        if (sd > max_sd) max_sd = sd;
    }

    // 等待活动
    activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

    if ((activity < 0) && (errno != EINTR)) {
        printf("select error");
    }

    // 处理活动socket...
}
```

### 4.2 原始套接字编程

```c
// 创建原始套接字接收ICMP包
int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
if (sock < 0) {
    perror("socket");
    return -1;
}

// 接收数据包
char buffer[1024];
struct sockaddr_in from;
socklen_t fromlen = sizeof(from);
int bytes = recvfrom(sock, buffer, sizeof(buffer), 0, 
                    (struct sockaddr*)&from, &fromlen);

if (bytes < 0) {
    perror("recvfrom");
    close(sock);
    return -1;
}

// 处理ICMP包...
```

## 5. 最佳实践和性能优化

1. **错误处理**
   - 检查所有系统调用的返回值
   - 使用perror或strerror(errno)输出有意义的错误信息

2. **资源管理**
   - 确保在所有路径上关闭文件描述符
   - 使用setsockopt设置SO_REUSEADDR避免地址占用问题

3. **性能考虑**
   - 对于高并发服务器，考虑使用epoll或kqueue
   - 使用sendfile()实现零拷贝文件传输
   - 调整TCP_NODELAY和SO_SNDBUF/SO_RCVBUF参数

4. **安全考虑**
   - 验证所有输入数据
   - 使用SSL/TLS加密敏感数据
   - 限制连接速率防止DoS攻击

## 6. 跨平台注意事项

1. **Windows vs Unix**
   - Windows使用Winsock API (winsock2.h)
   - 初始化需要调用WSAStartup()
   - 关闭使用closesocket()而非close()

2. **字节序问题**
   - 使用htons(), htonl(), ntohs(), ntohl()处理网络字节序

3. **头文件差异**
   - Unix: sys/socket.h, netinet/in.h, arpa/inet.h
   - Windows: winsock2.h, ws2tcpip.h

## 7. 调试技巧

1. **网络工具**
   - tcpdump/Wireshark抓包分析
   - netstat查看连接状态
   - telnet/nc测试服务

2. **日志记录**
   - 记录关键操作和错误
   - 记录客户端IP和端口

3. **压力测试**
   - 使用ab/wrk进行性能测试
   - 模拟高并发场景

## 8. 常见问题解决方案

**问题1：Address already in use**
解决方案：
```c
int yes = 1;
setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
```

**问题2：Connection reset by peer**
解决方案：
- 检查对端是否意外关闭
- 添加适当的错误处理代码

**问题3：Resource temporarily unavailable**
解决方案：
- 对于非阻塞socket是正常现象
- 可重试操作或使用select/epoll等待

**问题4：Broken pipe**
解决方案：
- 对端关闭连接后继续写入导致
- 检查send/recv返回值并处理错误
