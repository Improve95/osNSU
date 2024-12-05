#ifndef SERVER_SOCKET_SERVICE
#define SERVER_SOCKET_SERVICE

#include <stdio.h>
#include <poll.h>
#include <netinet/in.h>

#define BIND_EXCEPTION -1;
#define SOCKET_EXCEPTION -2;
#define LISTEN_EXCEPTION -3;

int getProxySocket(int port, int maxConnections);

int acceptPollWrapper(struct pollfd *fds, int listenSocket, int amountFds);

#endif
