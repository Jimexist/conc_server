/*
 * wrapper for all staticly linked cgi programs
 */
#include <stdlib.h>

extern void func(char *);

int main(void) {
    func(getenv("QUERY_STRING"));
}
