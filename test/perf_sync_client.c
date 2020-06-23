#include "hello.h"
#include <crpc.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define TEST_COUNT 10000

int main(int argc, char *argv[])
{
    msg_req_t msg;
    msg_reply_t *rep;
    int i, count = 0;
    struct timeval tv_start, tv_end;
    memset(&msg, 0, sizeof(msg));
    // crpc_debug(0x3);
    crpc_client_init(HELLO_CLIENT_PATH, HELLO_SERVER_PATH);
    gettimeofday(&tv_start, NULL);
    for (i = 0; i < TEST_COUNT; i++) {
        msg.id = i;
        snprintf(msg.info, sizeof(msg.info), "%s", "world");
        rep = CRPC_API_CALL(hello, &msg);
        if (rep) {
            count++;
        }
    }
    gettimeofday(&tv_end, NULL);
    crpc_client_destroy();
    printf("Total spent %ldms for %d times, success %d times.\n", ((tv_end.tv_sec - tv_start.tv_sec) * 1000000 + (tv_end.tv_usec - tv_start.tv_usec)) / 1000, TEST_COUNT, count);
    return 0;
}
