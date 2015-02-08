/*
 * rand.c - a minimal CGI program that generates random number
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../csapp.h"

void func(int fd, const char *buf) {
    char content[MAXLINE + 1];
    snprintf(content, MAXLINE, "Welcome to rand.com: ");
    snprintf(content, MAXLINE, "%sTHE Internet rand portal.\r\n<p>", content);
    snprintf(content, MAXLINE, "%sThe rand number is %d\r\n<p>", content,
             rand());
    snprintf(content, MAXLINE, "%sThanks for visiting!\r\n", content);

    char result[MAXLINE + 1];
    snprintf(result, MAXLINE,
             "Content-length: %ld\r\nContent-type: text/html\r\n\r\n%s",
             strlen(content), content);

    const size_t length = strlen(result);
    size_t written;
    if (length != (written = rio_writen(fd, result, length))) {
        fprintf(stderr, "expect to write %ld, written %ld\n", length, written);
    }
}
