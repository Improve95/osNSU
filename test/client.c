#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

int main() {
    const int port = 8081;
    const int maxConnections = 10;

    struct sockaddr_in clt_sockaddr;
    socklen_t clt_sock_len = sizeof(clt_sockaddr);
    int client_fd;
    int status;

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("open socket");
        exit(EXIT_FAILURE);
    }

    status = inet_pton(AF_INET, "127.0.0.1", &clt_sockaddr.sin_addr);
    if (status == -1) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }
    clt_sockaddr.sin_port= htons(port);
    clt_sockaddr.sin_family = AF_INET;

    status = connect(client_fd, (struct sockaddr_in*)&clt_sockaddr, clt_sock_len);
    if (status == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    while (1) {
        sleep(1);
    }

    return 0;
}