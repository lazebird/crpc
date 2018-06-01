#ifndef SRV_H
#define SRV_H
#include <stdio.h>
// #include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <list.h>

#define MAX_BUF_LEN 2048
#define MAX_EP_SIZE 4

typedef struct {
    int fd;
    struct sockaddr_un saddr;
    struct list_head apilst;
    char iobuf[MAX_BUF_LEN];
} srv_t;

#endif