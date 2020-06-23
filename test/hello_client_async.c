#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <crpc.h>
#include "hello.h"

static void hell_cb(msg_reply_t *rep)
{
    if (rep) LOG_INFO("%s -> [%d] %s", rep->server, rep->id, rep->info);
}

int main(int argc, char *argv[])
{
    msg_req_t msg;
    int i;
    memset(&msg, 0, sizeof(msg));
    crpc_debug(0x3);
    int fd = crpc_client_init(HELLO_CLIENT_PATH, HELLO_SERVER_PATH);
    for (i = 1; i <= LOOP_COUNT; i++) {
        msg.id = i;
        snprintf(msg.info, sizeof(msg.info), "%s", "world");
        CRPC_API_CALL_ASYNC(hello, &msg, hell_cb);
        crpc_client_run(fd, 1000);
    }
    while (crpc_client_run(fd, 1000) > 0) continue;
    crpc_client_destroy();
    return 0;
}
