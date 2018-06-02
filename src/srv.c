#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/epoll.h>
#include <srv.h>
#include <list.h>
#include <api.h>
#include <pkt.h>
#include <dbg.h>

srv_t g_srv;
int crpc_srv_init(char *path)
{
    if(g_srv.fd > 0) {
        return g_srv.fd;
    }
    g_srv.saddr.sun_family = AF_UNIX;
    snprintf(g_srv.saddr.sun_path, sizeof(g_srv.saddr.sun_path), "%s", path);
    unlink(path);
    g_srv.fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(g_srv.fd < 0) {
        return -1;
    }
    if(bind(g_srv.fd, (struct sockaddr *)&g_srv.saddr, sizeof(g_srv.saddr)) < 0) {
        close(g_srv.fd);
        g_srv.fd = -1;
        return -2;
    }
    INIT_LIST_HEAD(&g_srv.apilst);
    LOG_DBG("fd %d", g_srv.fd);
    return g_srv.fd;
}

int crpc_srv_destroy(void)
{
    if(g_srv.fd > 0) {
        close(g_srv.fd);
        g_srv.fd = -1;
    }
    return g_srv.fd;
}

int crpc_srv_proc(int fd)
{
    pktmsg_t msg;
    int ret;
    struct sockaddr_un src_addr;
    socklen_t addrlen = sizeof(src_addr);
    memset(&msg, 0, sizeof(msg));
    ret = recvfrom(fd, g_srv.iobuf, sizeof(g_srv.iobuf), 0, (struct sockaddr *)&src_addr, &addrlen);
    LOG_DBG("recv %d bytes from fd %d", ret, fd);
    if(ret <=0) {
        LOG_ERR("recv error, ret %d, errno %d, errstr %s", ret, errno, strerror(errno));
        return ret;
    }
    // pkt decode, call api, pkt encode, reply
    pkt_proc(g_srv.iobuf, ret, &msg);
    LOG_DBG("recv req api: %s", msg.name);
    ret = pkt_send(&msg, fd, g_srv.iobuf, sizeof(g_srv.iobuf), (struct sockaddr *)&src_addr, addrlen);
    return ret;
}

int crpc_srv_run(int count)
{
    int epfd =  epoll_create(1);
    struct epoll_event evt;
    int ret;
    if(epfd < 0) {
        return 0;
    }
    evt.events = EPOLLIN | EPOLLET;
    evt.data.fd = g_srv.fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, g_srv.fd, &evt);
    while(count < 0 || --count > 0) {
        if((ret = epoll_wait(epfd,&evt,1,-1)) > 0) {
            crpc_srv_proc(evt.data.fd);
        } else {
            LOG_ERR("epoll_wait error, ret %d, errno %d, errstr %s", ret, errno, strerror(errno));
            sleep(1);
        }
    }
    close(epfd);
    LOG_INFO("#### Count arrives, server exits ####");
    return 1;
}
