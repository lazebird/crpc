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
static void hell_cb(msg_reply_t *rep)
{
    if (rep) {
        printf("%s -> [%d] %s\n", rep->server, rep->id, rep->info);
    }
}

static int client_daemon(int count)
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
            crpc_client_proc(evs[i].data.fd);
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
    msg_req_t msg;
    int i;
    memset(&msg, 0, sizeof(msg));
    crpc_debug(0x3);
    g_fd = crpc_client_init(HELLO_CLIENT_PATH, HELLO_SERVER_PATH);
    for (i = 1; i < 10; i++) {
        msg.id = i;
        snprintf(msg.info, sizeof(msg.info), "%s", "world");
        CRPC_API_CALL_ASYNC(hello, &msg, hell_cb);
    }
    client_daemon(8);
    crpc_client_destroy();
    return 0;
}
