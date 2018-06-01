#ifndef HELLO_H
#define HELLO_H

#define HELLO_SERVER_PATH "/tmp/.hello_server"
#define HELLO_CLIENT_PATH "/tmp/.hello_client"

typedef struct {
    int id;
    char info[40];
} msg_req_t;

typedef struct {
    int id;
    char info[40];
    char server[40];
} msg_reply_t;

#endif
