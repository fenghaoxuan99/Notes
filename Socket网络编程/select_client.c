#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

void *handle_clinet(void *arg)
{
    int client_fd = *(int *)arg;
    char buffer[BUFFER_SIZE];

    while (1)
    {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(client_fd, &read_fds);
        struct timeval tv = {1, 0}; // 1秒超时

        int ret;
        do
        {
            int ret = select(client_fd + 1, &read_fds, NULL, NULL, &tv);
        } while (ret == -1 && errno == EINTR);

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

            buffer[bytes_read] = '\0'; // 确保字符串结束

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

    close(client_fd);
    pthread_exit(NULL);
    return NULL;
}
