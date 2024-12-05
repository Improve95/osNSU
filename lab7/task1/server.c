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
#define BUFFER_SIZE 1024

int main() {
    struct sockaddr_in srv_sockaddr, clt_sockaddr;
    socklen_t srv_sock_len = sizeof(srv_sockaddr);
    int client_fd;

    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_fd == -1) {
        perror("open socket");
        exit(EXIT_FAILURE);
    }

    srv_sockaddr.sin_family = AF_INET;
    srv_sockaddr.sin_addr.s_addr = INADDR_ANY;
    srv_sockaddr.sin_port = htons(PORT);

    if (bind(client_fd, (struct sockaddr*)&srv_sockaddr, sizeof(srv_sockaddr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    char receive_buffer[BUFFER_SIZE];
    char send_buffer[BUFFER_SIZE + 30];
    int read_bytes;
    while (true) {
        read_bytes = recvfrom(client_fd, receive_buffer, BUFFER_SIZE, 0, (struct sockaddr*)&clt_sockaddr, &srv_sock_len);
        if (read_bytes == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        printf("%s\n", receive_buffer);
        snprintf(send_buffer, BUFFER_SIZE + 30, "return from server: %s", receive_buffer);
        sendto(client_fd, send_buffer, BUFFER_SIZE, 0, (struct sockaddr*)&clt_sockaddr, srv_sock_len);
    }
    
    close(client_fd);
    close(client_fd);

    return EXIT_SUCCESS;
}
