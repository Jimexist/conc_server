#ifndef _BASELINE_H_
#define _BASELINE_H_

#include <pthread.h>
#include "csapp.h"
#include "req_handler.h"

void serve_static(int fd, char *filename, int filesize);
void serve_dynamic(int fd, char *filename, char *cgiargs);

#endif
