#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

#define MAX_CLIENTS 10
#define PORT 8080
#define BUFFER_SIZE 1024

typedef struct
{
    int fd;
    pthread_t thread_id;
} client_info;

client_info clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *arg)
{
    int client_fd = *(int *)arg;
    char buffer[BUFFER_SIZE];

    while (1)
    {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(client_fd, &read_fds);

        struct timeval tv = {1, 0}; // 1秒超时

        int ret = select(client_fd + 1, &read_fds, NULL, NULL, &tv);
        if (ret == -1)
        {
            perror("select in thread");
            break;
        }
        else if (ret == 0)
        {
            // 超时，可以做一些其他处理
            continue;
        }

        if (FD_ISSET(client_fd, &read_fds))
        {
            memset(buffer, 0, BUFFER_SIZE);
            ssize_t bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
            if (bytes_read <= 0)
            {
                // 客户端断开连接
                break;
            }
            printf("Received from client %d: %s\n", client_fd, buffer);

            // 简单回显
            write(client_fd, buffer, bytes_read);
        }
    }

    // 清理客户端
    close(client_fd);
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].fd == client_fd)
        {
            clients[i].fd = -1;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    return NULL;
}

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // 初始化客户端数组
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        clients[i].fd = -1;
    }

    // 创建服务器socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 设置socket选项
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 绑定socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 监听
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server started on port %d\n", PORT);

    while (1)
    {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(server_fd, &read_fds);
        int max_fd = server_fd;

        // 添加现有客户端到select集合
        pthread_mutex_lock(&clients_mutex);
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (clients[i].fd != -1)
            {
                FD_SET(clients[i].fd, &read_fds);
                if (clients[i].fd > max_fd)
                {
                    max_fd = clients[i].fd;
                }
            }
        }
        pthread_mutex_unlock(&clients_mutex);

        // 设置5秒超时
        struct timeval tv = {5, 0};
        int activity = select(max_fd + 1, &read_fds, NULL, NULL, &tv);

        if ((activity < 0) && (errno != EINTR))
        {
            perror("select error");
        }
        else if (activity == 0)
        {
            printf("Select timeout, no activity in 5 seconds\n");
            continue;
        }

        // 检查新连接
        if (FD_ISSET(server_fd, &read_fds))
        {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
            {
                perror("accept");
                continue;
            }

            printf("New connection, socket fd: %d\n", new_socket);

            // 添加到客户端数组
            pthread_mutex_lock(&clients_mutex);
            int added = 0;
            for (int i = 0; i < MAX_CLIENTS; i++)
            {
                if (clients[i].fd == -1)
                {
                    clients[i].fd = new_socket;
                    if (pthread_create(&clients[i].thread_id, NULL, handle_client, &clients[i].fd) != 0)
                    {
                        perror("pthread_create");
                        close(new_socket);
                        clients[i].fd = -1;
                    }
                    else
                    {
                        added = 1;
                    }
                    break;
                }
            }
            pthread_mutex_unlock(&clients_mutex);

            if (!added)
            {
                printf("Max clients reached, rejecting connection\n");
                close(new_socket);
            }
        }
    }

    return 0;
}
