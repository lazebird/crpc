// for external only
#ifndef CRPC_H
#define CRPC_H

typedef void *(*func_t)(void *);
typedef void (*callback_t)(void *);

int crpc_srv_init(char *path);
int crpc_srv_destroy(void);
int crpc_client_init(char *cpath, char *spath);
int crpc_client_destroy(void);

// this will hungup for new pkt, proc it if a pkt arrives, then hungup again; if "count" pkts arrived, it will return; nagative "count" means indefinitely
int crpc_srv_run(int count);        
// this will read a pkt and proc it, after a pkt detected, call this with its fd
int crpc_srv_proc(int fd);      

#define CRPC_API_REG(f) crpc_api_reg(#f, (func_t)f, sizeof(typeof(*f(NULL))))
int crpc_api_reg(char *name, func_t func, int retsize);

#define CRPC_API_CALL(f, arg) crpc_sync_call(#f, (void *)arg, sizeof(typeof(*arg)))
void *crpc_sync_call(char *name, void *arg, int argsize);

#define CRPC_API_CALL_ASYNC(f, arg, cb) crpc_async_call(#f, (void *)arg, sizeof(typeof(*arg)), (callback_t)cb)
int crpc_async_call(char *name, void *arg, int argsize, callback_t func);

int crpc_client_proc(int fd); // callback can be set by request and matched with pktid

int crpc_debug(int level);

#endif
