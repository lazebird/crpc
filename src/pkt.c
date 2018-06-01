#include <dbg.h>
#include <pkt.h>
#include <string.h>
#include <errno.h>
#include <srv.h>

static int get_tlv(tlv_t *tlv, pktmsg_t *msg)
{
    int vallen = tlv->l - sizeof(*tlv);
    switch(tlv->t) {
    case TLV_TYPE_NAME:
        if(vallen < sizeof(msg->name)) {
            memcpy(msg->name, tlv->v, vallen);
            msg->name[vallen] = 0;
        }
        break;
    case TLV_TYPE_ARG:
        if(vallen < sizeof(msg->val)) {
            memcpy(msg->val, tlv->v, vallen);
            msg->vallen = vallen;
        }
        break;
    default:
        break;
    }
    return 1;
}

static int add_tlv(char *buf, int bufsize, int type, char *val, int vallen)
{
    tlv_t *tlv = (void *)buf;
    int len = vallen + sizeof(*tlv);
    if(bufsize < len) {
        return 0;
    }
    tlv->t = type;
    tlv->l = len;
    memcpy(tlv->v, val, vallen);
    return len;
}

int pkt2msg(char *buf, int buflen, pktmsg_t *msg)
{
    hdr_t *hdr = (void *)buf;
    tlv_t *tlv;
    int lenleft = buflen - sizeof(*hdr);
    if(hdr->type <= PKT_TYPE_INIT || hdr->type >= PKT_TYPE_MAX || hdr->len <= sizeof(*hdr) || hdr->len != buflen) {
        return 0;
    }
    msg->type = hdr->type;
    msg->id = hdr->id;
    for(tlv = (void *)(hdr->data); lenleft > 0 && tlv->l <= lenleft; lenleft -= tlv->l, tlv = (tlv_t *)((char *)tlv + tlv->l)) {
        get_tlv(tlv, msg);
    }
    return 1;
}

int msg2pkt(pktmsg_t *msg, char *buf, int bufsize)
{
    hdr_t *hdr = (void *)buf;
    hdr->type = msg->type;
    hdr->id = msg->id;
    if(hdr->type <= PKT_TYPE_INIT || hdr->type >= PKT_TYPE_MAX) {
        return 0;
    }
    hdr->ver = 1;
    hdr->len = sizeof(*hdr);
    hdr->len += add_tlv(buf + hdr->len, bufsize - hdr->len, TLV_TYPE_NAME, msg->name, strlen(msg->name));
    hdr->len += add_tlv(buf + hdr->len, bufsize - hdr->len, TLV_TYPE_ARG, msg->val, msg->vallen);
    return hdr->len;
}

int pkt_proc(char *buf, int bufsize, pktmsg_t *msg)
{
    void *res;
    int reslen;
    pkt2msg(buf, bufsize, msg);
    switch(msg->type) {
    case PKT_TYPE_REQ:
        if((res = api_exec_by_name(msg->name, msg->val, &reslen)) && reslen < sizeof(msg->val)) {
            memcpy(msg->val, res, reslen);
            msg->vallen = reslen;
            msg->type = PKT_TYPE_REP;
        }
        break;
    case PKT_TYPE_REP:
        break;
    default:
        break;
    }
    return 1;
}

int pkt_send(pktmsg_t *msg, int fd, char *buf, int bufsize, struct sockaddr *dest_addr, socklen_t addrlen)
{
    int ret, len;
    len = msg2pkt(msg, buf, bufsize);
    if(dest_addr) {
        ret = sendto(fd, buf, len, 0, dest_addr, addrlen);
    } else {
        ret = send(fd, buf, len, 0);
    }
    LOG_DBG("send fd %d pkt type %d len %d ret %d, errno %d errstr %s", fd, msg->type, len, ret, errno, strerror(errno));
    if (ret < 0) {
        struct sockaddr_un *un = (struct sockaddr_un *)dest_addr;
        LOG_DBG("addr family %d, path %s, len %d", un->sun_family, un->sun_path, addrlen);
    }
    return ret;
}
