#ifndef HELLO_H
#define HELLO_H

#define HELLO_SERVER_PATH "/tmp/.hello_server"
#define HELLO_CLIENT_PATH "/tmp/.hello_client"

#define MAX_EP_SIZE 4
#define LOG_ERR(fmt, args...) printf("[%s :%d] "fmt"\n", __func__, __LINE__, ##args)

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
