#ifndef PKT_H
#define PKT_H
#include <sys/socket.h>
#include "api.h"

typedef struct { // host byte order? for domain socket only, extend when use tcpip
    char ver;    // current v1
    char type;
    short id;     // pkt match
    short len;    // total len, from ver to data[end]
    char data[0]; // tlv follows
} __attribute__((packed)) hdr_t;

typedef enum { PKT_TYPE_INIT, PKT_TYPE_REQ, PKT_TYPE_REP, PKT_TYPE_MAX } pkt_type_t;

typedef struct { // host byte order? for domain socket only, extend when use tcpip
    char t;
    short l;
    char v[0];
} __attribute__((packed)) tlv_t;

typedef enum { TLV_TYPE_INIT, TLV_TYPE_NAME, TLV_TYPE_ARG, TLV_TYPE_MAX } tlv_type_t;

typedef struct {
    int type;
    int id;
    char name[MAX_NAME_LEN];
    char val[MAX_ARG_LEN];
    int vallen;
} pktmsg_t;

int pkt_proc(char *buf, int bufsize, pktmsg_t *msg);
int pkt_send(pktmsg_t *msg, int fd, char *buf, int bufsize, struct sockaddr *dest_addr, socklen_t addrlen);

#endif