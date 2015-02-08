/*
 * time.c - a minimal CGI program that generates time
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../csapp.h"

void func(int fd, const char *buf) {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char content[MAXLINE + 1];
    snprintf(content, MAXLINE, "Welcome to time.com: ");
    snprintf(content, MAXLINE, "%sTHE Internet time portal.\r\n<p>", content);
    snprintf(content, MAXLINE, "%sCurrent time is: %s\r\n<p>", content,
             asctime(timeinfo));
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
