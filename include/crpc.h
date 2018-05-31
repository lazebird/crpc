// for external only
#ifndef CRPC_H
#define CRPC_H

typedef void *(*func_t)(void *);
typedef void (*callback_t)(void *);

int crpc_srv_init(char *path);
int crpc_client_init(char *path);

#define CRPC_API_REG(f) crpc_api_reg(#f, f, sizeof(typeof(*f(NULL))))
int crpc_api_reg(char *name, func_t func, int retsize);

#define CRPC_API_CALL(f, arg) crpc_sync_call(#f, arg, sizeof(typeof(*arg)))
int crpc_sync_call(char *name, void *arg, int argsize);

#define CRPC_API_CALL_ASYNC(f, arg, cb) crpc_async_call(#f, f, sizeof(typeof(*arg)), cb)
int crpc_async_call(char *name, void *arg, int argsize, callback_t func);

#endif