
### **完整 epoll 服务器示例 (C语言)**
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_EVENTS 1024
#define PORT 8080
#define BUF_SIZE 4096

// 设置为1启用边沿触发(ET)，0为水平触发(LT)
#define USE_EDGE_TRIGGER 1

// 连接上下文结构体
struct connection {
    int fd;
    char buf[BUF_SIZE];
    ssize_t bytes_read;
};

// 设置非阻塞IO
static void set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

// 创建监听socket
static int create_listen_socket() {
    int listen_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (listen_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 允许端口复用
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr = { htonl(INADDR_ANY) }
    };

    if (bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(listen_fd, SOMAXCONN) == -1) {
        perror("listen");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    return listen_fd;
}

// 处理客户端数据（Echo逻辑）
static void handle_client_data(struct connection *conn) {
    ssize_t n = write(conn->fd, conn->buf, conn->bytes_read);
    if (n == -1) {
        if (errno != EAGAIN) {
            perror("write");
            close(conn->fd);
            free(conn);
        }
        return;
    }
    printf("Echoed %zd bytes to fd %d\n", n, conn->fd);
}

int main() {
    int listen_fd = create_listen_socket();
    int epoll_fd = epoll_create1(EPOLL_CLOEXEC);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    // 添加监听socket到epoll
    struct epoll_event ev;
    ev.events = EPOLLIN;
    struct connection *listen_conn = malloc(sizeof(struct connection));
    listen_conn->fd = listen_fd;
    ev.data.ptr = listen_conn;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev) == -1) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }

    struct epoll_event events[MAX_EVENTS];
    printf("Server started on port %d (ET mode: %d)\n", PORT, USE_EDGE_TRIGGER);

    while (1) {
        int nready = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nready == -1) {
            if (errno == EINTR) continue; // 被信号中断
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < nready; ++i) {
            struct connection *conn = events[i].data.ptr;

            // 处理错误事件
            if (events[i].events & EPOLLERR) {
                fprintf(stderr, "EPOLLERR on fd %d\n", conn->fd);
                close(conn->fd);
                free(conn);
                continue;
            }

            // 处理新连接
            if (conn->fd == listen_fd) {
                struct sockaddr_in client_addr;
                socklen_t addrlen = sizeof(client_addr);
                int client_fd = accept4(listen_fd, (struct sockaddr*)&client_addr, 
                                      &addrlen, SOCK_NONBLOCK);
                if (client_fd == -1) {
                    if (errno != EAGAIN) perror("accept");
                    continue;
                }

                printf("New connection fd %d\n", client_fd);

                struct connection *client_conn = malloc(sizeof(struct connection));
                client_conn->fd = client_fd;
                client_conn->bytes_read = 0;

                ev.events = EPOLLIN;
                if (USE_EDGE_TRIGGER) ev.events |= EPOLLET;
                ev.data.ptr = client_conn;

                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
                    perror("epoll_ctl client");
                    close(client_fd);
                    free(client_conn);
                }
                continue;
            }

            // 处理客户端数据
            if (events[i].events & EPOLLIN) {
                conn->bytes_read = 0;
                
                // ET模式必须循环读取直到EAGAIN
                do {
                    ssize_t n = read(conn->fd, conn->buf + conn->bytes_read, 
                                   BUF_SIZE - conn->bytes_read);
                    if (n == -1) {
                        if (errno != EAGAIN) {
                            perror("read");
                            close(conn->fd);
                            free(conn);
                        }
                        break;
                    } else if (n == 0) { // 客户端关闭连接
                        printf("Client fd %d disconnected\n", conn->fd);
                        close(conn->fd);
                        free(conn);
                        break;
                    }

                    conn->bytes_read += n;
                    printf("Received %zd bytes from fd %d\n", n, conn->fd);

                    // LT模式不需要循环读取（但这里演示完整处理）
                    if (!USE_EDGE_TRIGGER) break;
                } while (conn->bytes_read < BUF_SIZE);

                if (conn->bytes_read > 0) {
                    handle_client_data(conn);
                }
            }
        }
    }

    close(listen_fd);
    close(epoll_fd);
    return 0;
}
```

---

### **关键流程解析**
1. **初始化阶段**：
   ```mermaid
   sequenceDiagram
       participant Server
       participant Kernel
       Server->>Kernel: epoll_create1() 创建实例
       Server->>Kernel: socket()+bind()+listen() 创建监听套接字
       Server->>Kernel: epoll_ctl(EPOLL_CTL_ADD) 添加监听socket
   ```

2. **事件循环**：
   ```c
   while (1) {
       epoll_wait()  // 阻塞等待事件
       for 每个就绪事件 {
           if 是监听socket事件:
               accept() 新连接
               epoll_ctl(EPOLL_CTL_ADD) 添加新fd
           else:
               read() 数据 -> 处理 -> write() 回显
               ET模式需循环read到EAGAIN
       }
   }
   ```

3. **两种触发模式对比**：
   - **LT模式**（默认）：
     - 只要socket可读，每次`epoll_wait`都会返回
     - 可以不用一次性读完数据
   - **ET模式**：
     ```c
     ev.events = EPOLLIN | EPOLLET;  // 必须显式设置
     // 必须使用非阻塞IO + 循环读取直到EAGAIN
     while ((n = read(fd, buf, size)) > 0) {
         // 处理数据
     }
     if (n == -1 && errno != EAGAIN) {
         // 错误处理
     }
     ```

---

### **测试方法**
1. 编译运行：
   ```bash
   gcc -o epoll_server epoll_server.c
   ./epoll_server
   ```

2. 使用 `telnet` 或 `nc` 测试：
   ```bash
   # 终端1
   $ nc localhost 8080
   Hello  # 输入内容
   Hello  # 服务器回显

   # 终端2 (观察服务器输出)
   New connection fd 5
   Received 6 bytes from fd 5
   Echoed 6 bytes to fd 5
   ```