#include "serve_dynamic.h"

/*
 * serve_dynamic - run a CGI program on behalf of the client
 */
void serve_dynamic(int fd, char *filename, char *cgiargs, Cache *cache,
                   pthread_rwlock_t *rwlock) {
    char buf[MAXLINE], *emptylist[] = {NULL};

    /* Return first part of HTTP response */
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    if (-1 == rio_writen(fd, buf, strlen(buf))) {
        return;
    }
    sprintf(buf, "Server: Baseline Concurrent Web Server\r\n");
    if (-1 == rio_writen(fd, buf, strlen(buf))) {
        return;
    }

    if (Fork() == 0) { /* child */
        /* Real server would set all CGI vars here */
        setenv("QUERY_STRING", cgiargs, 1);
        Dup2(fd, STDOUT_FILENO);
        /* Redirect stdout to client */
        Execve(filename, emptylist, environ);
        /* Run CGI program */
    }
    Wait(NULL);
    /* Parent waits for and reaps child */
}
