/*
 * multiplier.c - a minimal CGI program that multiplies two numbers together
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../csapp.h"

static void parse(const char *buf, int *n1, int *n2) {
    char arg1[MAXLINE + 1], arg2[MAXLINE + 1];
    if (buf != NULL) {
        char *p = strchr(buf, '&');
        if (p) {
            *p = '\0';
            strncpy(arg1, buf, MAXLINE);
            strncpy(arg2, p + 1, MAXLINE);
            *n1 = atoi(arg1);
            *n2 = atoi(arg2);
            return;
        }
    }
    *n1 = *n2 = 0;
}

void func(int fd, const char *buf) {
    int n1, n2;
    parse(buf, &n1, &n2);

    char content[MAXLINE + 1];
    snprintf(content, MAXLINE, "Welcome to multiply.com: ");
    snprintf(content, MAXLINE, "%sTHE Internet multiplication portal.\r\n<p>",
             content);
    snprintf(content, MAXLINE, "%sThe answer is: %d * %d = %d\r\n<p>", content,
             n1, n2, n1 * n2);
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
