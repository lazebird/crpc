#include <srv.h>

srv_t g_srv;
int crpc_srv_init(char *path)
{
    if (g_srv.fd > 0) {
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
    return g_srv.fd;
}