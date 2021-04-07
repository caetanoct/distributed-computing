#ifndef LOGGER_H
#define LOGGER_H

#include <pthread.h>

#define GLOBAL_LOG_PERIOD 20

// Info required for loggger to do it's job
typedef struct logger_info {
    char * mem_address; // Server address space
    int server_id;
    int num_servers;
    int server_mem;
    pthread_mutex_t * mem_lock;
} logger_info;

void * logger(void * info);

#endif