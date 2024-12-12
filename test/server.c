#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <netinet/in.h>

int main() {
    const int port = 8080;
    const int maxConnections = 10;

    struct sockaddr_in listenAddress;

    listenAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    listenAddress.sin_family = AF_INET;
    listenAddress.sin_port = htons(port);

    int proxySocket = socket(AF_INET, SOCK_STREAM, 0);

    if (proxySocket < 0) {
        perror("Cannot create proxySocket");
        return -1;
    }
    int reuse = 1;
    setsockopt(proxySocket, SOL_SOCKET, SO_REUSEPORT, (const char *) &reuse, sizeof(reuse));
    if (bind(proxySocket, (struct sockaddr *) &listenAddress, sizeof(listenAddress))) {
        perror("Cannot bind proxySocket");
        return -1;
    }

    if (listen(proxySocket, maxConnections)) {
        perror("listen error");
        return -1;
    }

    struct pollfd fds;
    fds.fd = proxySocket;
    fds.events = POLLOUT;

    while (1) {
        int polled = poll(&fds, 1, -1);
        if (fds.revents & POLLERR || fds.revents & POLLHUP || fds.revents & POLLNVAL) {
            printf("error\n");
        }
        if (fds.revents & POLLOUT) {
            printf("POLLOUT\n");
        }
        if (fds.revents & POLLIN) {
            printf("POLLIN\n");
        }
    }

    return 0;
}