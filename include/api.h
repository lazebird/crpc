#ifndef API_H
#define API_H

#define MAX_NAME_LEN 64

typedef void *(*func_t)(void *);
typedef void (*callback_t)(void *);

typedef struct {
    struct list_head lst;
    char name[MAX_NAME_LEN];
    func_t api;
    int argsize;
    int retsize;
} api_t;

#endif