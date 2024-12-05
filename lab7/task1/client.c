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

#define PORT 65525
#define BUFFER_SIZE 1024

int main() {
    struct sockaddr_in srv_sockaddr;
    socklen_t clt_sock_len = sizeof(srv_sockaddr);
    int client_fd;
    int status;

    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_fd == -1) {
        perror("open socket");
        exit(EXIT_FAILURE);
    }

    status = inet_pton(AF_INET, "127.0.0.1", &srv_sockaddr.sin_addr);
    if (status == -1) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }
    srv_sockaddr.sin_port= htons(PORT);
    srv_sockaddr.sin_family = AF_INET;  

    char send_buffer[BUFFER_SIZE], receive_buffer[BUFFER_SIZE];
    for (int i = 0; i < 5; i++) {
        snprintf(send_buffer, BUFFER_SIZE, "hello_%d", i);
        sendto(client_fd, send_buffer, BUFFER_SIZE, 0, (struct sockaddr*)&srv_sockaddr, clt_sock_len);

        recvfrom(client_fd, receive_buffer, BUFFER_SIZE, 0, (struct sockaddr*)&srv_sockaddr, &clt_sock_len);
        printf("%s\n", receive_buffer);

        sleep(2);
    }

    close(client_fd);

    return EXIT_SUCCESS;
}
