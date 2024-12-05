#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include "logging.h"
#include "cache.h"
#include "httpService.h"

#define ALLOCATE_ERROR_EXCEPTION -5;


#define RECV_FROM_SERVER_EXCEPTION -2
#define SERVER_CLOSED_EXCEPTION -3
#define STATUS_OR_CONTENT_LENGTH_EXCEPTION -4
#define NOT_FREE_CACHE_EXCEPTION -6
#define PUT_CACHE_DATA_EXCEPTION -7

#define END_READING_PROCCESS 1
enum ServerState{
    REQUEST_SENDING,
    CACHING
} typedef ServerState;

struct ServerConnection {

    int serverSocket;
    int cacheIndex;
    int id;
    ServerState state;
    struct pollfd *fd;

    int (*sendRequest)(struct ServerConnection *self, char *data, int dataSize);

    int (*caching)(struct ServerConnection *self, CacheEntry *cache, void *buf, size_t bufferSize);

} typedef ServerConnection;

ServerConnection *initServerConnection(int serverSocket, int cacheIndex);

int closeServerConnection(struct ServerConnection *self);

#endif //SERVERCONNECTION_H
