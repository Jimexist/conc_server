/*
 * baseline.c - A simple, iterative HTTP/1.0 Web server that uses the
 *     GET method to serve static and dynamic content.
 * this is adapted from tiny.c
 */
#include "baseline.h"

int main(int argc, char **argv) {
    /* ignore the sigpipe */
    Signal(SIGPIPE, SIG_IGN);

    /* Check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    const int port = atoi(argv[1]);
    const int listenfd = Open_listenfd(port);
    struct sockaddr_in clientaddr;

    while (1) {
        socklen_t clientlen = sizeof(clientaddr);
        int clientfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
        on_request(clientfd);
    }
}

/*
 * serve_static - copy a file back to the client
 */
void serve_static(int fd, char *filename, int filesize) {
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];

    /* Send response headers to client */
    get_filetype(filename, filetype);
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    sprintf(buf, "%sServer: Concurrent Web Server\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
    if (-1 == rio_writen(fd, buf, strlen(buf))) {
        return;
    }

    /* Send response body to client */
    srcfd = Open(filename, O_RDONLY, 0);
    srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    Close(srcfd);
    if (-1 == rio_writen(fd, srcp, filesize)) {
        /* broken pipe */
    }
    Munmap(srcp, filesize);
}

/*
 * serve_dynamic - run a CGI program on behalf of the client
 */
void serve_dynamic(int fd, char *filename, char *cgiargs) {
    char buf[MAXLINE], *emptylist[] = {NULL};

    /* Return first part of HTTP response */
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    if (-1 == rio_writen(fd, buf, strlen(buf))) {
        return;
    }
    sprintf(buf, "Server: Concurrent Web Server\r\n");
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
