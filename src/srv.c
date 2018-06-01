#include <unistd.h>
#include <stdlib.h>
#include <srv.h>
#include <list.h>
#include <api.h>
#include <sys/epoll.h>

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

int crpc_srv_run(int count)
{
    int epfd =  epoll_create(MAX_EP_SIZE);
    struct epoll_event evs[MAX_EP_SIZE];
    int i, num, ret;
    evs[0].events = EPOLLIN;
    evs[0].data.fd = g_srv.fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, g_srv.fd, &evs[0]);
    while (1) {
        num = epoll_wait(epfd,evs,MAX_EP_SIZE,-1);
        for (i = 0; i < num; i++) {
            ret = read(evs[i].data.fd, g_srv.iobuf, sizeof(g_srv.iobuf));
            if (ret <=0) {
                continue;
            }
            // pkt decode, call api, pkt encode, reply
        }
        if (count != -1 && --count <= 0) {
            break;
        }
    }
    return 1;
}

int srv_add_api(api_t *api)
{
    api_t *new = malloc(sizeof(*new));
    if(new) {
        memcpy(new, api, sizeof(*new));
        list_add(&api->lst, &g_srv.apilst);
    }
    return !!new;
}
