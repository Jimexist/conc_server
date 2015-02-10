#include <dlfcn.h>
#include <pthread.h>
#include "code_cache.h"
#include "serve_dynamic.h"

typedef void (*fp)(int fd, const char *);

static int lock_read(pthread_rwlock_t *rwlock) {
    int retval = pthread_rwlock_rdlock(rwlock);
#ifdef DEBUG
    printf("[%p] pthread_rwlock_rdlock/%d\n", pthread_self(), retval);
#endif
    if (retval) {
        fprintf(stderr, "failed to pthread_rwlock_rdlock, errno %d\n", retval);
    }
    return retval;
}

static int lock_write(pthread_rwlock_t *rwlock) {
    int retval = pthread_rwlock_wrlock(rwlock);
#ifdef DEBUG
    printf("[%p] pthread_rwlock_wrlock/%d\n", pthread_self(), retval);
#endif
    if (retval) {
        fprintf(stderr, "failed to pthread_rwlock_wrlock, errno %d\n", retval);
    }
    return retval;
}

static int unlock(pthread_rwlock_t *rwlock) {
    int retval = pthread_rwlock_unlock(rwlock);
#ifdef DEBUG
    printf("[%p] pthread_rwlock_unlock/%d\n", pthread_self(), retval);
#endif
    if (retval) {
        fprintf(stderr, "failed to pthread_rwlock_unlock, errno %d\n", retval);
    }
    return retval;
}

void serve_dynamic(int fd, char *filename, char *cgiargs, Cache *code_cache,
                   pthread_rwlock_t *rwlock) {
    char libname[MAXLINE + 1];
    if (!strstr(filename, "./cgi-bin/")) {
        return;
    }
    snprintf(libname, MAXLINE, "./cgi-bin/lib%s.so",
             filename + strlen("./cgi-bin/"));

    if (0 == lock_read(rwlock)) {
        void *handle = get(code_cache, libname);
        if (NULL == handle) {
            if (0 == unlock(rwlock) && 0 == lock_write(rwlock)) {
                handle = get(code_cache, libname);
                if (NULL == handle) {
                    handle = dlopen(libname, RTLD_LAZY);
                    char *errstr = dlerror();
                    if (errstr != NULL) {
                        fprintf(stderr, "dlopen error %s\n", errstr);
                    }
                    if (NULL != handle) {
                        void *old_handle = put(code_cache, libname, handle);
                        if (NULL != old_handle) {
                            dlclose(old_handle);
                            errstr = dlerror();
                            if (errstr != NULL) {
                                fprintf(stderr, "dlclose error %s\n", errstr);
                            }
                        }
                    }
                }
            }
        }
        if (NULL != handle) {
            /* clear errors */
            dlerror();
            const fp func = (fp) dlsym(handle, "func");
            char *errstr = dlerror();
            if (errstr != NULL) {
                fprintf(stderr, "dynamic link error %s\n", errstr);
            } else {
                char buf[MAXLINE + 1];
                snprintf(buf, MAXLINE, "HTTP/1.0 200 OK\r\n");
                if (-1 != rio_writen(fd, buf, strlen(buf))) {
                    snprintf(buf, MAXLINE,
                             "Server: Optimized Concurrent Web Server\r\n");
                    if (-1 != rio_writen(fd, buf, strlen(buf))) {
                        func(fd, cgiargs);
                    }
                }
            }
            unlock(rwlock);
        }
    }
}
