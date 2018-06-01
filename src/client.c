#include <client.h>
#include <unistd.h>
#include <sys/epoll.h>
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

static void *crpc_client_wait(pktmsg_t *msg, int sec)
{
    int epfd =  epoll_create(MAX_EP_SIZE);
    struct epoll_event evs[MAX_EP_SIZE];
    int i, num, ret;
    if (epfd < 0) {
        return 0;
    }
    evs[0].events = EPOLLIN | EPOLLET;
    evs[0].data.fd = g_client.fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, g_client.fd, &evs[0]);
    memset(evs, 0, sizeof(evs));
    num = epoll_wait(epfd,evs,MAX_EP_SIZE,sec);
    LOG_DBG("get %d events", num);
    for(i = 0; i < num; i++) {
        ret = recv(evs[i].data.fd, g_client.iobuf, sizeof(g_client.iobuf), 0);
        LOG_DBG("recv %d bytes from fd %d", ret, evs[i].data.fd);
        if(ret <=0) {
            continue;
        }
        // pkt decode, call api, pkt encode, reply
        pkt_proc(g_client.iobuf, ret, msg);
        //break; // should only one msg here
    }
    close(epfd);
    if(msg->type == PKT_TYPE_REP) {
        return msg->val;
    }
    return NULL;
}

void *client_req_api(char *name, void *arg, int argsize, callback_t func)
{
    static pktmsg_t msg;
    memset(&msg, 0, sizeof(msg));
    msg.type = PKT_TYPE_REQ;
    snprintf(msg.name, sizeof(msg.name), "%s", name);
    memcpy(msg.val, arg, argsize);
    msg.vallen = argsize;
    pkt_send(&msg, g_client.fd, g_client.iobuf, sizeof(g_client.iobuf), (struct sockaddr *)&g_client.saddr, (socklen_t)sizeof(g_client.saddr));
    if(func) {
        return NULL;
    }
    return crpc_client_wait(&msg, 3000);
}
