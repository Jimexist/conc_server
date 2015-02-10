#include <dlfcn.h>
#include <pthread.h>
#include "code_cache.h"
#include "serve_dynamic.h"

typedef void (*fp)(int fd, const char *);

void serve_dynamic(int fd, char *filename, char *cgiargs, Cache *code_cache,
                   pthread_rwlock_t *rwlock) {
    char libname[MAXLINE + 1];
    if (!strstr(filename, "./cgi-bin/")) {
        return;
    }
    snprintf(libname, MAXLINE, "./cgi-bin/lib%s.so", filename + 10);

    if (0 == pthread_rwlock_rdlock(rwlock)) {
        void *handle = get(code_cache, libname);
        if (NULL == handle) {
            if (0 == pthread_rwlock_wrlock(rwlock)) {
                handle = dlopen(libname, RTLD_LAZY);
                if (NULL != handle) {
                    void *old_handle = put(code_cache, libname, handle);
                    if (NULL != old_handle) {
                        if (0 != dlclose(old_handle)) {
                            fprintf(stderr, "failed to dlclose: %s\n",
                                    dlerror());
                        }
                    }
                }
                pthread_rwlock_unlock(rwlock);
            }
        }
        if (NULL == handle) {
            /* clear errors */
            dlerror();
            fp func = (fp) dlsym(handle, "func");
            char *error = dlerror();
            if (error != NULL) {
                fprintf(stderr, "dynamic link error %s\n", error);
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
        }
        pthread_rwlock_unlock(rwlock);
    }
}
