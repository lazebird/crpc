#include <stdlib.h>
#include "api.h"
#include "dbg.h"
#include "srv.h"
#include "client.h"

static int add_api(api_t *api)
{
    api_t *newapi = malloc(sizeof(*newapi));
    if (newapi) {
        memcpy(newapi, api, sizeof(*newapi));
        list_add(&newapi->lst, &g_srv.apilst);
    }
    return !!newapi;
}

static api_t *find_api(char *name)
{
    api_t *p;
    list_for_each_entry(p, &g_srv.apilst, lst)
    {
        if (!strcmp(name, p->name)) {
            return p;
        }
    }
    return NULL;
}

int crpc_api_reg(char *name, func_t func, int retsize)
{
    api_t tmp;
    LOG_DBG("name %s addr %p ret size %d", name, func, retsize);
    memset(&tmp, 0, sizeof(tmp));
    snprintf(tmp.name, sizeof(tmp.name), "%s", name);
    tmp.func = func;
    tmp.retsize = retsize;
    add_api(&tmp);
    return 1;
}

void *crpc_sync_call(char *name, void *arg, int argsize)
{
    LOG_DBG("name %s arg %p arg size %d", name, arg, argsize);
    return client_req_api(name, arg, argsize, NULL);
}

int crpc_async_call(char *name, void *arg, int argsize, callback_t func)
{
    LOG_DBG("name %s arg %p arg size %d callback %p", name, arg, argsize, func);
    client_req_api(name, arg, argsize, func);
    return 1;
}

void *api_exec_by_name(char *name, void *arg, int *retsize)
{
    api_t *p = find_api(name);
    if (!p) {
        return NULL;
    }
    *retsize = p->retsize;
    return p->func(arg);
}
