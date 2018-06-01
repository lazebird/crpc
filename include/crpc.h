// for external only
#ifndef CRPC_H
#define CRPC_H

typedef void *(*func_t)(void *);
typedef void (*callback_t)(void *);

int crpc_srv_init(char *path);
int crpc_srv_destroy(void);
int crpc_client_init(char *cpath, char *spath);
int crpc_client_destroy(void);

int crpc_srv_run(int count);

#define CRPC_API_REG(f) crpc_api_reg(#f, (func_t)f, sizeof(typeof(*f(NULL))))
int crpc_api_reg(char *name, func_t func, int retsize);

#define CRPC_API_CALL(f, arg) crpc_sync_call(#f, (void *)arg, sizeof(typeof(*arg)))
void *crpc_sync_call(char *name, void *arg, int argsize);

#define CRPC_API_CALL_ASYNC(f, arg, cb) crpc_async_call(#f, (void *)arg, sizeof(typeof(*arg)), (callback_t)cb)
int crpc_async_call(char *name, void *arg, int argsize, callback_t func);

int crpc_debug(int level);

#endif