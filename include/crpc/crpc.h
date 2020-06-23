// for external only
#ifndef CRPC_H
#define CRPC_H

typedef void *(*func_t)(void *);
typedef void (*callback_t)(void *);

int crpc_debug(int level);
int crpc_srv_init(char *path);
int crpc_srv_destroy(void);
int crpc_client_init(char *cpath, char *spath);
int crpc_client_destroy(void);

int crpc_srv_run(int fd, int msec); // this will hungup for new pkt, proc and return if a pkt arrives
int crpc_srv_proc(int fd);          // this will read a pkt and proc it; use this api if you want to wait a pkt yourself

void *crpc_client_run(int fd, int msec); // this will hungup for new pkt, proc and return if a pkt arrives
void *crpc_client_proc(int fd);          // this will read a pkt and proc it; use this api if you want to wait a pkt yourself

#define CRPC_API_REG(f) crpc_api_reg(#f, (func_t)f, sizeof(typeof(*f(NULL))))
int crpc_api_reg(char *name, func_t func, int retsize);

#define CRPC_API_CALL(f, arg) crpc_sync_call(#f, (void *)arg, sizeof(typeof(*arg)))
void *crpc_sync_call(char *name, void *arg, int argsize);

#define CRPC_API_CALL_ASYNC(f, arg, cb) crpc_async_call(#f, (void *)arg, sizeof(typeof(*arg)), (callback_t)cb)
int crpc_async_call(char *name, void *arg, int argsize, callback_t func);

#endif
