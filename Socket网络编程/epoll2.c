
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

// server
int main(int argc, const char *argv[])
{
    int sever_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sever_fd == -1)
    {
        perror("socket error");
        exit(1);
    }
    int op = 1;
    setsockopt(sever_fd, SOL_SOCKET, SOL_REUSEADDR, &op, sizeof(op));

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9999);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int ret = bind(sever_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (ret == -1)
    {
        perror("bind error");
        exit(1);
    }

    ret = listen(sever_fd, 64);
    if (ret == -1)
    {
        perror("listen error");
        exit(1);
    }

    // 创建一个epoll实例
    int epfd = epoll_create(100);

    struct epoll_event ev;
    ev.events = EPOLLIN;   // 监听读事件
    ev.data.fd = sever_fd; // 监听的文件描述符

    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sever_fd, &ev);
    if (ret == -1)
    {
        perror("epoll_ctl");
        exit(0);
    }

    struct epoll_event evs[1024];
    int size = sizeof(evs) / sizeof(struct epoll_event);

    while (1)
    {
        // 调用一次, 检测一次
        int num = epoll_wait(epfd, evs, size, -1);
        for (int i = 0; i < num; ++i)
        {
            // 取出当前的文件描述符
            int curfd = evs[i].data.fd;
            // 判断这个文件描述符是不是用于监听的
            if (curfd == sever_fd)
            {
                // 建立新的连接
                int cfd = accept(curfd, NULL, NULL);
                // 新得到的文件描述符添加到epoll模型中, 下一轮循环的时候就可以被检测了
                ev.events = EPOLLIN; // 读缓冲区是否有数据
                ev.data.fd = cfd;
                ret = epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
                if (ret == -1)
                {
                    perror("epoll_ctl-accept");
                    exit(0);
                }
            }
            else
            {
                // 处理通信的文件描述符
                // 接收数据
                char buf[1024];
                memset(buf, 0, sizeof(buf));
                int len = recv(curfd, buf, sizeof(buf), 0);
                if (len == 0)
                {
                    printf("客户端已经断开了连接\n");
                    // 将这个文件描述符从epoll模型中删除
                    epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
                    close(curfd);
                }
                else if (len > 0)
                {
                    printf("客户端say: %s\n", buf);
                    send(curfd, buf, len, 0);
                }
                else
                {
                    perror("recv");
                    exit(0);
                }
                        }
        }
    }

    return 0;
}