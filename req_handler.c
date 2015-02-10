#include "req_handler.h"

static Cache *code_cache = NULL;
const static size_t cache_size = 4;
static pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void server_init() {
    assert(!code_cache && "init can only happen once");
    code_cache = new_cache(cache_size);
}

typedef struct {
    int fd;
    Cache *code_cache;
    pthread_rwlock_t *rwlock;
} Params;

/*
 * clienterror - returns an error message to the client
 */
void clienterror(int fd, char *cause, char *errnum, char *shortmsg,
                 char *longmsg) {
    char buf[MAXLINE], body[MAXBUF];

    /* Build the HTTP response body */
    sprintf(body, "<html><title>Client Error</title>");
    sprintf(body, "%s<body bgcolor=ffffff>\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Concurrent Web server</em>\r\n", body);
    sprintf(body, "%s</body></html>\r\n", body);

    /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    if (-1 == rio_writen(fd, buf, strlen(buf))) {
        return;
    }
    sprintf(buf, "Content-type: text/html\r\n");
    if (-1 == rio_writen(fd, buf, strlen(buf))) {
        return;
    }

    sprintf(buf, "Content-length: %d\r\n\r\n", (int) strlen(body));
    if (-1 == rio_writen(fd, buf, strlen(buf))) {
        return;
    }

    if (-1 == rio_writen(fd, body, strlen(body))) {
        return;
    }
}

/*
 * get_filetype - derive file type from file name
 */
void get_filetype(char *filename, char *filetype) {
    if (strstr(filename, ".html")) {
        strcpy(filetype, "text/html");
    } else if (strstr(filename, ".gif")) {
        strcpy(filetype, "image/gif");
    } else if (strstr(filename, ".jpg")) {
        strcpy(filetype, "image/jpeg");
    } else {
        strcpy(filetype, "text/plain");
    }
}

/*
 * parse_uri - parse URI into filename and CGI args
 *             return 0 if dynamic content, 1 if static
 */
int parse_uri(char *uri, char *filename, char *cgiargs) {
    char *ptr;

    if (!strstr(uri, "cgi-bin")) {
        /* Static content */
        strcpy(cgiargs, "");
        strcpy(filename, ".");
        strcat(filename, uri);
        if (uri[strlen(uri) - 1] == '/') {
            strcat(filename, "home.html");
        }
        return 1;
    } else { /* Dynamic content */
        ptr = index(uri, '?');
        if (ptr) {
            strcpy(cgiargs, ptr + 1);
            *ptr = '\0';
        } else {
            strcpy(cgiargs, "");
        }
        strcpy(filename, ".");
        strcat(filename, uri);
        return 0;
    }
}

/*
 * read_requesthdrs - read and parse HTTP request headers
 */
void read_requesthdrs(rio_t *rp) {
    char buf[MAXLINE];

    Rio_readlineb(rp, buf, MAXLINE);
    while (strcmp(buf, "\r\n")) {
        Rio_readlineb(rp, buf, MAXLINE);
    }
}

/*
 * on_request - handle one HTTP request
 */
void on_request(int clientfd) {
    pthread_t thread;
    /* note this will be freed in the client thread */
    Params *param = (Params *) Malloc(sizeof(Params));
    param->fd = clientfd;
    param->code_cache = code_cache;
    param->rwlock = &rwlock;
    Pthread_create(&thread, NULL, &handle_request, param);
}

/*
 * handle_request - handle request in its own thread
 */
void *handle_request(void *ptr) {
    /* detach itself, and passing argument for the file descriptor */
    Pthread_detach(Pthread_self());
    Params *p = (Params *) ptr;
    const int fd = p->fd;
    Cache *code_cache = p->code_cache;
    pthread_rwlock_t *rwlock = p->rwlock;
    Free(ptr);
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;

    /* Read request line and headers */
    Rio_readinitb(&rio, fd);
    Rio_readlineb(&rio, buf, MAXLINE);
    sscanf(buf, "%s %s %s", method, uri, version);

    if (strcasecmp(method, "GET")) {
        clienterror(fd, method, "501", "Not Implemented",
                    "Tiny does not implement this method");
    } else {
        read_requesthdrs(&rio);
        /* Parse URI from GET request */
        const int is_static = parse_uri(uri, filename, cgiargs);

        if (stat(filename, &sbuf) < 0) {
            clienterror(fd, filename, "404", "Not found",
                        "Tiny couldn't find this file");
        } else {
            if (is_static) { /* Serve static content */
                if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) {
                    clienterror(fd, filename, "403", "Forbidden",
                                "Tiny couldn't read the file");
                } else {
                    serve_static(fd, filename, sbuf.st_size);
                }
            } else { /* Serve dynamic content */
                if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) {
                    clienterror(fd, filename, "403", "Forbidden",
                                "Tiny couldn't run the CGI program");
                } else {
                    serve_dynamic(fd, filename, cgiargs, code_cache, rwlock);
                }
            }
        }
    }
    Close(fd);
    return NULL;
}
