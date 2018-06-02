#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>
#include <client.h>
#include <pkt.h>
#include <dbg.h>

client_t g_client;
int crpc_client_init(char *cpath, char *spath)
{
    if(g_client.fd > 0) {
        return g_client.fd;
    }
    g_client.caddr.sun_family = AF_UNIX;
    snprintf(g_client.caddr.sun_path, sizeof(g_client.caddr.sun_path), "%s", cpath);
    g_client.saddr.sun_family = AF_UNIX;
    snprintf(g_client.saddr.sun_path, sizeof(g_client.saddr.sun_path), "%s", spath);
    g_client.fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(g_client.fd < 0) {
        return -1;
    }
    unlink(cpath);
    if(bind(g_client.fd, (struct sockaddr *)&g_client.caddr, sizeof(g_client.caddr)) < 0) {
        close(g_client.fd);
        g_client.fd = -1;
        return -2;
    }
    LOG_DBG("fd %d", g_client.fd);
    return g_client.fd;
}

int crpc_client_destroy(void)
{
    if(g_client.fd > 0) {
        close(g_client.fd);
        g_client.fd = -1;
    }
    return g_client.fd;
}

void *crpc_client_proc(int fd) // callback can be set by request and matched with pktid
{
    static pktmsg_t msg;
    int ret;
    struct sockaddr_un src_addr;
    socklen_t addrlen = sizeof(src_addr);
    memset(&msg, 0, sizeof(msg));
    ret = recvfrom(fd, g_client.iobuf, sizeof(g_client.iobuf), 0, (struct sockaddr *)&src_addr, &addrlen);
    LOG_DBG("recv %d bytes from fd %d", ret, fd);
    if(ret <=0) {
        LOG_ERR("recv error, ret %d, errno %d, errstr %s", ret, errno, strerror(errno));
        return 0;
    }
    // pkt decode, call api, pkt encode, reply
    pkt_proc(g_client.iobuf, ret, &msg);
    if(msg.type == PKT_TYPE_REP && g_client.cb) {
        g_client.cb(msg.val);
        return NULL;
    }
    return msg.val;
}

static void *crpc_client_run(int sec)
{
    int epfd =  epoll_create(1);
    struct epoll_event evt;
    void *res = NULL;
    if(epfd < 0) {
        return NULL;
    }
    evt.events = EPOLLIN;// | EPOLLET;
    evt.data.fd = g_client.fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, g_client.fd, &evt);
    if(epoll_wait(epfd,&evt,1,sec) > 0) {
        res = crpc_client_proc(evt.data.fd);
    }
    close(epfd);
    return res;
}

void *client_req_api(char *name, void *arg, int argsize, callback_t func)
{
    pktmsg_t msg;
    memset(&msg, 0, sizeof(msg));
    msg.type = PKT_TYPE_REQ;
    snprintf(msg.name, sizeof(msg.name), "%s", name);
    memcpy(msg.val, arg, argsize);
    msg.vallen = argsize;
    pkt_send(&msg, g_client.fd, g_client.iobuf, sizeof(g_client.iobuf), (struct sockaddr *)&g_client.saddr, (socklen_t)sizeof(g_client.saddr));
    if(func) {
        g_client.cb = func;
        return NULL;
    } else {
        g_client.cb = NULL;
    }
    return crpc_client_run(3000);
}

