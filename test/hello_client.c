#include <stdio.h>
#include <string.h>
#include <crpc.h>
#include "hello.h"

int main(int argc, char *argv[])
{
    msg_req_t msg;
    msg_reply_t *rep;
    int i;
    memset(&msg, 0, sizeof(msg));
    crpc_debug(0x3);
    crpc_client_init(HELLO_CLIENT_PATH, HELLO_SERVER_PATH);
    for (i = 1; i < 10; i++) {
        msg.id = i;
        snprintf(msg.info, sizeof(msg.info), "%s", "world");
        rep = CRPC_API_CALL(hello, &msg);
        if (rep) {
            printf("%s -> [%d] %s\n", rep->server, rep->id, rep->info);
        }
    }
    crpc_client_destroy();
    return 0;
}
