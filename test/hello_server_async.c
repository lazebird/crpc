#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <crpc.h>
#include "hello.h"

int g_fd;
msg_reply_t *hello(msg_req_t *msg)
{
    static msg_reply_t res;
    printf("req [%d] %s\n", msg->id, msg->info);
    memset(&res, 0, sizeof(res));
    res.id = msg->id;
    snprintf(res.info, sizeof(res.info), "%s %s", "hello", msg->info);
    snprintf(res.server, sizeof(res.server), "%s", HELLO_SERVER_PATH);
    return &res;
}

static int srv_daemon(int count)
{
    int epfd =  epoll_create(MAX_EP_SIZE);
    struct epoll_event evs[MAX_EP_SIZE];
    int i, num;
    if (epfd < 0) {
        return 0;
    }
    evs[0].events = EPOLLIN;
    evs[0].data.fd = g_fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, g_fd, &evs[0]);
    while(1) {
        memset(&evs, 0, sizeof(evs));
        num = epoll_wait(epfd,evs,MAX_EP_SIZE,-1);
        if (num <= 0) {
            LOG_ERR("epoll_wait error, ret %d, errno %d, errstr %s", num, errno, strerror(errno));
            sleep(1);
            continue;
        }
        for(i = 0; i < num; i++) {
            crpc_srv_proc(evs[i].data.fd);
        }
        if(!--count) {
            break;
        }
    }
    close(epfd);
    return 1;
}

int main(int argc, char *argv[])
{
    crpc_debug(0x3);
    g_fd = crpc_srv_init(HELLO_SERVER_PATH);
    CRPC_API_REG(hello);
    srv_daemon(8);
    crpc_srv_destroy();
    return 0;
}
