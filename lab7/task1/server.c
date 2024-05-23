#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <stdbool.h>

#define PORT 65525

int main() {
    struct sockaddr_in srv_sockaddr;
    socklen_t srv_sock_len = sizeof(srv_sockaddr);
    int opt = 1;
    int server_fd;
    int client_socket;
    // int fork_pid;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("open socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    srv_sockaddr.sin_family = AF_INET;
    srv_sockaddr.sin_addr.s_addr = INADDR_ANY;
    srv_sockaddr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&srv_sockaddr, sizeof(srv_sockaddr));

    // while (true) {
        client_socket = accept(server_fd, (struct sockaddr*)&srv_sockaddr, &srv_sock_len);
        if (client_socket == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        /*fork_pid = fork();
        if (fork_pid == 0) {
            printf("here\n");
            break;
        }*/
    // }


    return EXIT_SUCCESS;
}