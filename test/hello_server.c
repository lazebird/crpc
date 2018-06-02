#include <stdio.h>
#include <string.h>
#include <crpc.h>
#include "hello.h"

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

int main(int argc, char *argv[])
{
    crpc_debug(0x3);
    crpc_srv_init(HELLO_SERVER_PATH);
    CRPC_API_REG(hello);
    crpc_srv_run(8);
    crpc_srv_destroy();
    return 0;
}
