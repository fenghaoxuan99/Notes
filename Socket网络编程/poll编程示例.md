Linux 下使用 `poll` 进行 socket 编程的示例：

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

// 时间复杂度说明：
// poll() - O(n) n为监控的fd数量
// 整体程序时间复杂度 - O(n) 事件驱动模型

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // 创建监听socket (O(1))
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 设置socket选项 (O(1))
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 绑定socket (O(1))
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 开始监听 (O(1))
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // 初始化pollfd结构体数组
    struct pollfd fds[MAX_CLIENTS + 1]; // +1 for server_fd
    nfds_t nfds = 1; // 当前监控的fd数量

    // 添加server_fd到监控列表
    fds[0].fd = server_fd;
    fds[0].events = POLLIN; // 监控可读事件

    // 初始化客户端fd为-1（表示未使用）
    for (int i = 1; i < MAX_CLIENTS + 1; i++) {
        fds[i].fd = -1;
    }

    while (1) {
        // 调用poll等待事件 (阻塞调用)
        int ret = poll(fds, nfds, -1); // -1表示无限等待
        if (ret < 0) {
            perror("poll error");
            break;
        }

        // 检查所有监控的fd
        for (int i = 0; i < nfds; i++) {
            if (fds[i].revents == 0) {
                continue; // 无事件发生
            }

            // 处理错误事件
            if (fds[i].revents & (POLLERR | POLLHUP | POLLNVAL)) {
                printf("Error on fd %d\n", fds[i].fd);
                close(fds[i].fd);
                fds[i].fd = -1;
                continue;
            }

            // 处理服务器socket事件（新连接）
            if (fds[i].fd == server_fd && (fds[i].revents & POLLIN)) {
                if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                    perror("accept");
                    continue;
                }

                printf("New connection from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

                // 将新socket添加到poll监控列表
                int j;
                for (j = 1; j < MAX_CLIENTS + 1; j++) {
                    if (fds[j].fd == -1) {
                        fds[j].fd = new_socket;
                        fds[j].events = POLLIN;
                        if (j + 1 > nfds) {
                            nfds = j + 1;
                        }
                        break;
                    }
                }

                if (j == MAX_CLIENTS + 1) {
                    printf("Too many clients, rejecting\n");
                    close(new_socket);
                }
            }
            // 处理客户端socket事件
            else if (fds[i].revents & POLLIN) {
                int valread = read(fds[i].fd, buffer, BUFFER_SIZE);
                if (valread <= 0) {
                    // 客户端断开连接
                    printf("Client disconnected\n");
                    close(fds[i].fd);
                    fds[i].fd = -1;
                } else {
                    printf("Received: %s\n", buffer);
                    // 回显数据
                    send(fds[i].fd, buffer, valread, 0);
                    memset(buffer, 0, BUFFER_SIZE);
                }
            }
        }
    }

    // 关闭所有socket
    for (int i = 0; i < nfds; i++) {
        if (fds[i].fd != -1) {
            close(fds[i].fd);
        }
    }

    return 0;
}
```

### 关键点解析：
1. **poll vs select**：
   - poll没有最大文件描述符限制（select通常限制为1024）
   - poll使用链表结构，select使用位图
   - 时间复杂度相同（O(n)），但poll在大规模连接时更高效

2. **事件类型说明**：
   - `POLLIN`：数据可读
   - `POLLOUT`：数据可写
   - `POLLERR`：错误发生
   - `POLLHUP`：连接挂起

3. **性能优化建议**：
   - 对于超大规模连接（C10K问题），考虑改用epoll（时间复杂度O(1)）
   - 可以使用非阻塞IO配合poll提高吞吐量

4. **常见错误处理**：
   - 检查poll返回值（-1表示错误，0表示超时）
   - 正确处理`POLLERR`等错误事件
   - 注意fd重用时的初始化
