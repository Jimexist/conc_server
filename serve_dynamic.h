#ifndef _SERVE_DYNAMIC_H_
#define _SERVE_DYNAMIC_H_

#include <pthread.h>
#include "csapp.h"
#include "assert.h"
#include "code_cache.h"

void serve_dynamic(int fd, char *filename, char *cgiargs, Cache *code_cache,
                   pthread_rwlock_t *rwlock);

#endif
