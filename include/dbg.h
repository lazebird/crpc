#ifndef DBG_H
#define DBG_H
#include <stdio.h>

#define LOG_DBG(fmt, args...) printf("[%s :%d] "fmt"\n", __func__, __LINE__, ##args)
#define LOG_ERR(fmt, args...) printf("[%s :%d] "fmt"\n", __func__, __LINE__, ##args)

#endif