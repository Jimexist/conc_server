#ifndef _REQ_HANDLER_H_
#define _REQ_HANDLER_H_

#include "csapp.h"
#include "serve_static.h"

void on_request(int clientfd);
void *handle_request(void *ptr);
void read_requesthdrs(rio_t *rp);
void get_filetype(char *filename, char *filetype);
int parse_uri(char *uri, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg,
                 char *longmsg);

#endif
