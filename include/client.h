#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
// #include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <list.h>

#define MAX_BUF_LEN 2048

typedef struct {
    int fd;
    struct sockaddr_un saddr;
    char iobuf[MAX_BUF_LEN];
} client_t;

#endif