#ifndef _SERVE_STATIC_H_
#define _SERVE_STATIC_H_

#include "csapp.h"
#include "req_handler.h"

void serve_static(int fd, char *filename, int filesize);

#endif
