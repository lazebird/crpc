#include "dbg.h"

int g_crpc_dbg;

int crpc_debug(int level)
{
    g_crpc_dbg = level;
    return 1;
}
