#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
// #include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <list.h>
#include "api.h"

#define MAX_BUF_LEN 2048
#define MAX_EP_SIZE 4

typedef struct {
    int fd;
    struct sockaddr_un caddr;
    struct sockaddr_un saddr;
    char iobuf[MAX_BUF_LEN];
    callback_t cb;
} client_t;

void *client_req_api(char *name, void *arg, int argsize, callback_t func);

#endif