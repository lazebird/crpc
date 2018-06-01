#include <api.h>
#include <dbg.h>

int crpc_api_reg(char *name, func_t func, int retsize)
{
    LOG_DBG("name %s addr %p ret size %d", name, func, retsize);
    return 1;
}

void *crpc_sync_call(char *name, void *arg, int argsize)
{
    LOG_DBG("name %s arg %p arg size %d", name, arg, argsize);
    return NULL;
}

int crpc_async_call(char *name, void *arg, int argsize, callback_t func)
{
    LOG_DBG("name %s arg %p arg size %d callback %p", name, arg, argsize, func);
    return 1;
}
