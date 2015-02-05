#include <dlfcn.h>
#include "serve_dynamic.h"

typedef void (*fp)(int fd, const char *);

void serve_dynamic(int fd, char *filename, char *cgiargs) {
    char libname[MAXLINE + 1];
    if (!strstr(filename, "./cgi-bin/")) {
        return;
    }

    snprintf(libname, MAXLINE, "./cgi-bin/lib%s.so", filename + 10);
    void *handle = dlopen(libname, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "failed to load %s, error number is %d\n", libname,
                errno);
    } else {
        printf("loaded %s\n", libname);
    }

    dlerror();
    fp func = (fp) dlsym(handle, "func");

    char *error = dlerror();
    if (error != NULL) {
        fprintf(stderr, "dynamic link error %s\n", error);
        return;
    }

    char buf[MAXLINE + 1];
    snprintf(buf, MAXLINE, "HTTP/1.0 200 OK\r\n");
    if (-1 == rio_writen(fd, buf, strlen(buf))) {
        return;
    }

    snprintf(buf, MAXLINE, "Server: Optimized Concurrent Web Server\r\n");
    if (-1 == rio_writen(fd, buf, strlen(buf))) {
        return;
    }

    func(fd, cgiargs);
    dlclose(handle);
    printf("unloaded %s\n", libname);
}
