#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include "cache.h"
#include "serverConnectionList.h"
#include "httpService.h"

#define DEAD_CLIENT_EXCEPTION -2
#define RECV_CLIENT_EXCEPTION -3
#define NOT_GET_EXCEPTION -5
#define URL_EXCEPTION -6
#define RESOLVING_SOCKET_FROM_URL_EXCEPTION -7

#define SEND_TO_CLIENT_EXCEPTION -22
#define WRITER_CACHE_INVALID_EXCEPTION -33
#define CACHE_INVALID_EXCEPTION -44
#define SUCCESS_WITH_END 11

enum client_state {
    WAITING_REQUEST,
    SENDING_FROM_CACHE
} typedef ClientState;

struct client_connection {
    int clientSocket;
    int id;
    int numChunksWritten;
    int cacheIndex;
    NodeCacheData **curData;
    ClientState state;
    struct pollfd *fd;

    int (*sendFromCache)(struct client_connection *self, CacheEntry *cache,int *localConnections);
    int (*handleGetRequest)(struct client_connection *self, char *buffer, int bufferSize,
                            CacheEntry *cache,
                            const int maxCacheSize,
                            int *localConnectionsCount,
                            int threadId, NodeServerConnection **listServerConnections);

} typedef ClientConnection;

ClientConnection *initClientConnection(int clientSocket);

int handleGetRequest(ClientConnection *self, char *buffer, int bufferSize,
                     CacheEntry *cache,
                     const int maxCacheSize, int*localConnectionsCount,
                     int threadId, NodeServerConnection **listServerConnections);


int closeClientConnection(ClientConnection *self);

#endif //CLIENTCONNECTION_H
