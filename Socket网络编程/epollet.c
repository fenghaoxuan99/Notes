#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <stdbool.h>

#define MAX_EVENTS 1024
#define BUFFER_SIZE 1024

// 设置文件描述符为非阻塞模式
static void set_nonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
    {
        perror("fcntl F_GETFL");
        exit(EXIT_FAILURE);
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        perror("fcntl F_SETFL");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);

    // 1. 创建监听socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 设置SO_REUSEADDR选项
    int opt = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        perror("setsockopt");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    // 2. 绑定地址
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    // 3. 开始监听
    if (listen(listen_fd, SOMAXCONN) == -1)
    {
        perror("listen");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", port);

    // 4. 创建epoll实例
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        perror("epoll_create1");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    // 5. 添加监听socket到epoll，使用ET模式
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET; // 边缘触发模式
    event.data.fd = listen_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event) == -1)
    {
        perror("epoll_ctl EPOLL_CTL_ADD");
        close(listen_fd);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }

    // 事件循环
    struct epoll_event events[MAX_EVENTS];
    while (true)
    {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1)
        {
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < nfds; i++)
        {
            // 6. 处理新连接
            if (events[i].data.fd == listen_fd)
            {
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int conn_fd;

                // 必须循环accept直到EAGAIN，因为ET模式只通知一次
                while ((conn_fd = accept(listen_fd,
                                         (struct sockaddr *)&client_addr,
                                         &client_len)) > 0)
                {
                    printf("New connection from %s:%d\n",
                           inet_ntoa(client_addr.sin_addr),
                           ntohs(client_addr.sin_port));

                    // 设置新连接为非阻塞模式
                    set_nonblocking(conn_fd);

                    // 添加新连接到epoll，使用ET模式
                    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
                    event.data.fd = conn_fd;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd, &event) == -1)
                    {
                        perror("epoll_ctl EPOLL_CTL_ADD");
                        close(conn_fd);
                        continue;
                    }
                }

                if (conn_fd == -1)
                {
                    if (errno != EAGAIN && errno != EWOULDBLOCK)
                    {
                        perror("accept");
                    }
                    // 已经accept完所有连接
                }
            }
            // 7. 处理客户端数据
            else
            {
                // 7.1 处理连接关闭事件
                if (events[i].events & (EPOLLRDHUP | EPOLLHUP))
                {
                    printf("Client disconnected\n");
                    close(events[i].data.fd);
                    continue;
                }

                // 7.2 处理可读事件
                if (events[i].events & EPOLLIN)
                {
                    int fd = events[i].data.fd;
                    char buffer[BUFFER_SIZE];
                    ssize_t total_read = 0;

                    // ET模式必须循环读取直到EAGAIN
                    while (true)
                    {
                        ssize_t n = read(fd, buffer + total_read, sizeof(buffer) - total_read);
                        if (n > 0)
                        {
                            total_read += n;
                            // 检查是否已经读满缓冲区
                            if (total_read == sizeof(buffer))
                            {
                                break;
                            }
                        }
                        else if (n == 0)
                        {
                            // 对端关闭连接
                            printf("Client closed connection\n");
                            close(fd);
                            break;
                        }
                        else
                        {
                            if (errno == EAGAIN || errno == EWOULDBLOCK)
                            {
                                // 数据已读完，可以处理接收到的数据
                                if (total_read > 0)
                                {
                                    printf("Received %zd bytes: %.*s\n",
                                           total_read, (int)total_read, buffer);

                                    // Echo回数据
                                    ssize_t total_sent = 0;
                                    while (total_sent < total_read)
                                    {
                                        ssize_t sent = write(fd, buffer + total_sent,
                                                             total_read - total_sent);
                                        if (sent == -1)
                                        {
                                            if (errno == EAGAIN || errno == EWOULDBLOCK)
                                            {
                                                // 输出缓冲区已满，需要等待可写事件
                                                event.events = EPOLLOUT | EPOLLET | EPOLLRDHUP;
                                                event.data.fd = fd;
                                                if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event) == -1)
                                                {
                                                    perror("epoll_ctl EPOLL_CTL_MOD");
                                                    close(fd);
                                                }
                                                break;
                                            }
                                            else
                                            {
                                                perror("write");
                                                close(fd);
                                                break;
                                            }
                                        }
                                        total_sent += sent;
                                    }
                                }
                            }
                            else
                            {
                                perror("read");
                                close(fd);
                            }
                            break;
                        }
                    }
                }
                // 7.3 处理可写事件
                else if (events[i].events & EPOLLOUT)
                {
                    int fd = events[i].data.fd;
                    // 获取之前未发送完的数据（需通过上下文或自定义结构体传递）
                    YourBuffer *pending_data = get_pending_data(fd);

                    // 继续发送剩余数据
                    ssize_t sent = write(fd, pending_data->buf + pending_data->sent_len,
                                         pending_data->total_len - pending_data->sent_len);
                    if (sent > 0)
                    {
                        pending_data->sent_len += sent;
                        if (pending_data->sent_len == pending_data->total_len)
                        {
                            // 全部发送完成，恢复监听读事件
                            free_pending_data(fd);
                            event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
                            epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event);
                        }
                    }
                    else if (sent == -1 && errno != EAGAIN)
                    {
                        // 真实错误
                        close(fd);
                        free_pending_data(fd);
                    }
                    // 若 sent == -1 && errno == EAGAIN，保持监听 EPOLLOUT
                }
            }
        }
    }

    close(listen_fd);
    close(epoll_fd);
    return 0;
}
