/*
 * adder.c - a minimal CGI program that adds two numbers together
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE (8192)

void func(const char *buf) {
    char *p;
    char arg1[MAXLINE + 1], arg2[MAXLINE + 1], content[MAXLINE];
    int n1 = 0, n2 = 0;

    /* Extract the two arguments */
    if (buf != NULL) {
        p = strchr(buf, '&');
        if (p) {
            *p = '\0';
            strncpy(arg1, buf, MAXLINE);
            strncpy(arg2, p + 1, MAXLINE);
            n1 = atoi(arg1);
            n2 = atoi(arg2);
        }
    }

    /* Make the response body */
    sprintf(content, "Welcome to add.com: ");
    sprintf(content, "%sTHE Internet addition portal.\r\n<p>", content);
    sprintf(content, "%sThe answer is: %d + %d = %d\r\n<p>", content, n1, n2,
            n1 + n2);
    sprintf(content, "%sThanks for visiting!\r\n", content);

    /* Generate the HTTP response */
    printf("Content-length: %d\r\n", (int) strlen(content));
    printf("Content-type: text/html\r\n\r\n");
    printf("%s", content);
    fflush(stdout);
}
