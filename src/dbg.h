#ifndef DBG_H
#define DBG_H
#include <stdio.h>

extern int g_crpc_dbg;
#define LOG_ERR(fmt, args...)                                                                                                                                                      \
    if (g_crpc_dbg >= 1) printf("[%s :%d] " fmt "\n", __func__, __LINE__, ##args)
#define LOG_WARN(fmt, args...)                                                                                                                                                     \
    if (g_crpc_dbg >= 2) printf("[%s :%d] " fmt "\n", __func__, __LINE__, ##args)
#define LOG_INFO(fmt, args...)                                                                                                                                                     \
    if (g_crpc_dbg >= 3) printf("[%s :%d] " fmt "\n", __func__, __LINE__, ##args)
#define LOG_DBG(fmt, args...)                                                                                                                                                      \
    if (g_crpc_dbg >= 4) printf("[%s :%d] " fmt "\n", __func__, __LINE__, ##args)

#endif