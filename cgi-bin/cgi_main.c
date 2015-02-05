/*
 * wrapper for all staticly linked cgi programs
 */
#include <stdlib.h>
#include <stdio.h>

extern void func(int fd, const char *);

int main(void) {
    func(fileno(stdout), getenv("QUERY_STRING"));
}
