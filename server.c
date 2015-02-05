#include "server.h"

int main(int argc, char **argv) {
    /* ignore the sigpipe */
    Signal(SIGPIPE, SIG_IGN);

    /* Check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    const int port = atoi(argv[1]);
    const int listenfd = Open_listenfd(port);
    struct sockaddr_in clientaddr;

    while (1) {
        socklen_t clientlen = sizeof(clientaddr);
        int clientfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
        on_request(clientfd);
    }
}
