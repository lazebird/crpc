#include <client.h>

client_t g_client;
int crpc_client_init(char *path)
{
    if (g_client.fd > 0) {
        return g_client.fd;
    }
    g_client.saddr.sun_family = AF_UNIX;
    snprintf(g_client.saddr.sun_path, sizeof(g_client.saddr.sun_path), "%s", path);
    g_client.fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(g_client.fd < 0) {
        return -1;
    }
    return g_client.fd;
}