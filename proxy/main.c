#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include "headers/clientConnectionList.h"
#include "headers/serverConnectionList.h"
#include "headers/queueService.h"
#include "headers/threadPool.h"
#include "headers/cache.h"
#include "headers/serverSocketService.h"
#include <time.h>

#define MAX_CONNECTIONS 100
#define MAX_CACHE_SIZE 5
#define BUFFER_SIZE 16 * 1024
#define MAX_NUM_TRANSLATION_CONNECTIONS 100
#define GARBAGE_COLLECTOR_CHECK_PERIOD 5
#define CACHE_INFO_TTL 10

Queue *socketsQueue;
int poolSize;
int isRun = 1;

CacheEntry cache[MAX_CACHE_SIZE];
int proxySocket;
bool sigCaptured = false;

void checkArgs(int argc, const char *argv[]) {
    poolSize = atoi(argv[1]);
    if (argc != 3) {
        perror("Wrong count of arguments");
        exit(EXIT_FAILURE);
    }

    int proxySocketPort = atoi(argv[2]);
    if (proxySocketPort <= 0) {
        fprintf(stderr, "Incorrect port\n");
        exit(EXIT_FAILURE);
    }
}

void signalHandler(int sig) {
    if (sig == SIGTERM) {
        write(0, "SIGTERM\n", 8);
    }
    if (sig == SIGINT) {
        write(0, "SIGINT\n", 7);
    }
    isRun = 0;
    sigCaptured = true;
    pthread_cond_broadcast(&socketsQueue->condVar);
}

void removeClientWrapper(const char *reason, int *localConnectCount, NodeClientConnection **list,
                         ClientConnection *clientConnection, int threadId) {
    printf("Thread: %d, Connection %d, %s\n", threadId, clientConnection->id, reason);
    deleteClientConnectionById(list, clientConnection->id);
    (*localConnectCount)--;
}

void removeServerWrapper(const char *reason, int *localConnectCount, NodeServerConnection **list,
                         ServerConnection *serverConnection, int threadId) {

    printf("Thread: %d, Connection %d, %s\n", threadId, serverConnection->id, reason);
    deleteServerConnectionById(list, serverConnection->id);
    (*localConnectCount)--;
}

void handleCachingException(int result, NodeServerConnection **listServers, ServerConnection *serverConnection,
                            int threadId, int *localConnects) {

    if (result == RECV_FROM_SERVER_EXCEPTION) {
        printf("(%d) (%d)| READ_FROM_SERVER_WRITE_CLIENT:recv fron server err\n", threadId, serverConnection->id);
        makeCacheInvalid(&cache[serverConnection->cacheIndex]);
        removeServerWrapper("READ_FROM_SERVER_WRITE_CLIENT:recv fron server err", localConnects, listServers,
                            serverConnection, threadId);
    } else if (result == SERVER_CLOSED_EXCEPTION) {
        if (serverConnection->cacheIndex != -1) {
            printf("(%d) (%d)| READ_FROM_SERVER_WRITE_CLIENT:INVALID CACHE\n", threadId, serverConnection->id);
            makeCacheInvalid(&cache[serverConnection->cacheIndex]);
        }
        removeServerWrapper("READ_FROM_SERVER_WRITE_CLIENT:server closed", localConnects, listServers, serverConnection,
                            threadId);

    } else if (result == NOT_FREE_CACHE_EXCEPTION) {
        printf("READ_FROM_SERVER_WRITE_CLIENT:i dont have cache");
    } else if (result == STATUS_OR_CONTENT_LENGTH_EXCEPTION) {
        makeCacheInvalid(&cache[serverConnection->cacheIndex]);
        removeServerWrapper("DO NOT NEED TO BE CACHED", localConnects, listServers, serverConnection, threadId);
    } else if (result == END_READING_PROCCESS) {
        removeServerWrapper("READ_FROM_SERVER_WRITE_CLIENT:SUCCESS", localConnects, listServers, serverConnection,
                            threadId);
    } else if (result == PUT_CACHE_DATA_EXCEPTION) {
        removeServerWrapper("PUT_CACHE_DATA_EXCEPTION", localConnects, listServers, serverConnection, threadId);
    } else {
        printf("(default)");
    }

}

void handleSendingFromCacheException(int result, NodeClientConnection **list, ClientConnection *clientConnection,
                                     int threadId, int *localConnectCount) {
    switch (result) {
        case WRITER_CACHE_INVALID_EXCEPTION: {
            removeClientWrapper("WRITER_CACHE_INVALID_EXCEPTION:smth happend with writer cache", localConnectCount,
                                list, clientConnection, threadId);
            break;
        }
        case SEND_TO_CLIENT_EXCEPTION: {
            removeClientWrapper("READ_FROM_CACHE_WRITE_CLIENT:client err", localConnectCount,
                                list, clientConnection, threadId);
            break;
        }
        case SUCCESS_WITH_END: {
            removeReader(&cache[clientConnection->cacheIndex]);
            removeClientWrapper("READ_FROM_CACHE_WRITE_CLIENT:SUCCESS", localConnectCount,
                                list, clientConnection, threadId);
            break;
        }
        case CACHE_INVALID_EXCEPTION: {
            removeClientWrapper("CACHE_INVALID_EXCEPTION:smth happend with writer cache", localConnectCount,
                                list, clientConnection, threadId);
            break;
        }
        default:
            printf("default");
    }
}

void handleGetException(int result, NodeClientConnection **list, ClientConnection *clientConnection,
                        int threadId, int *localConnectCount) {
    switch (result) {
        case DEAD_CLIENT_EXCEPTION: {
            removeClientWrapper("CLIENT_MESSAGE:dead client", localConnectCount, list, clientConnection, threadId);
            break;
        }
        case RECV_CLIENT_EXCEPTION : {
            removeClientWrapper("CLIENT_MESSAGE:recv err", localConnectCount, list, clientConnection, threadId);
            break;
        }
        case NOT_GET_EXCEPTION: {
            removeClientWrapper("CLIENT_MESSAGE:not GET", localConnectCount, list, clientConnection, threadId);
            break;
        }
        case URL_EXCEPTION: {
            removeClientWrapper("CLIENT_MESSAGE:not good url", localConnectCount, list, clientConnection, threadId);
            break;
        }
        case RESOLVING_SOCKET_FROM_URL_EXCEPTION : {
            removeClientWrapper("CLIENT_MESSAGE:get server err", localConnectCount, list, clientConnection, threadId);
            break;
        }
        default:
            printf("default----   %d\n", result);
    }
}

void *garbageCollectorRoutine(void *args) {
    while (isRun) {
        printf("\n");
        for (size_t i = 0; i < MAX_CACHE_SIZE; i++) {
            CacheEntry *cacheInfo = &cache[i];
            pthread_mutex_lock(&cacheInfo->mutex);
            time_t nowTime = time(NULL);
            printf("gb: readers: %ld, status %d\n", cacheInfo->readers, cacheInfo->status);
            if (cacheInfo->status == VALID &&
                cacheInfo->readers <= 0 && nowTime - cacheInfo->lastGetTime >= CACHE_INFO_TTL) {
                cacheInfo->status = INVALID;
                cacheInfo->readers = 0;
                free(cacheInfo->url);
                cacheInfo->url = NULL;
                printf("gb deleted cache\n");
            }
            pthread_mutex_unlock(&cacheInfo->mutex);
        }
        sleep(GARBAGE_COLLECTOR_CHECK_PERIOD);
    }
    return NULL;
}

int updatePoll(struct pollfd *fds, NodeClientConnection *clients, NodeServerConnection *servers) {
    int counter = 0;
    NodeClientConnection *iterClients = clients;
    NodeServerConnection *iterServers = servers;
    while (iterClients != NULL) {
        ClientConnection *clientConnection = iterClients->connection;
        fds[counter].fd = clientConnection->clientSocket;
        if (clientConnection->state == WAITING_REQUEST) {
            fds[counter].events = POLLIN;
        } else {
            fds[counter].events = POLLOUT;
        }
        clientConnection->fd = &fds[counter];
        iterClients = iterClients->next;
        counter++;
    }
    while (iterServers != NULL) {
        ServerConnection *serverConnection = iterServers->connection;
        if (serverConnection->state == REQUEST_SENDING) {
            fds[counter].events = POLLOUT;
        } else {
            fds[counter].events = POLLIN;
        }
        fds[counter].fd = serverConnection->serverSocket;
        iterServers = iterServers->next;
        counter++;
    }
    return counter;
}

int getNewClientSocket(int *localConnectionsCount) {
    int newClientSocket = -1;
    pthread_mutex_lock(&socketsQueue->queueMutex);
    if (!isEmpty(socketsQueue) && isRun == 1) {

        newClientSocket = getSocketFromQueue(socketsQueue);
        if (newClientSocket != -1) {
            (*localConnectionsCount)++;
        }
    }

    while (*localConnectionsCount == 0 && isEmpty(socketsQueue) && isRun == 1) {
        pthread_cond_wait(&socketsQueue->condVar, &socketsQueue->queueMutex);
        newClientSocket = getSocketFromQueue(socketsQueue);
        if (newClientSocket != -1) {
            (*localConnectionsCount)++;
        }

    }
    pthread_mutex_unlock(&socketsQueue->queueMutex);
    return newClientSocket;
}

void updateClients(NodeClientConnection **listClientsConnections, NodeServerConnection **listServerConnection,
                   int threadId, int *localConnectionsCount) {

    NodeClientConnection *iterClientConnectionNode = *listClientsConnections;
    char buf[BUFFER_SIZE];

    while (iterClientConnectionNode != NULL) {
        ClientConnection *clientConnection = iterClientConnectionNode->connection;
        if (clientConnection->fd->revents & POLLHUP) {
            iterClientConnectionNode = iterClientConnectionNode->next;
            handleGetException(DEAD_CLIENT_EXCEPTION, listClientsConnections, clientConnection, threadId, localConnectionsCount);
            continue;
        }
        if (clientConnection->state == WAITING_REQUEST) {
            int result = clientConnection->handleGetRequest(clientConnection, buf, BUFFER_SIZE, cache, MAX_CACHE_SIZE,
                                                            localConnectionsCount, threadId,
                                                            listServerConnection);
            clientConnection->state = SENDING_FROM_CACHE;
            if (result != 0) {
                iterClientConnectionNode = iterClientConnectionNode->next;
                handleGetException(result, listClientsConnections, clientConnection, threadId, localConnectionsCount);
                continue;
            }
        } else if (clientConnection->state == SENDING_FROM_CACHE) {
            int result = clientConnection->sendFromCache(clientConnection, cache, localConnectionsCount, threadId);
            if (result != 0) {
                printf("handleSendingFromCacheException\n");
                iterClientConnectionNode = iterClientConnectionNode->next;
                handleSendingFromCacheException(result, listClientsConnections, clientConnection, threadId,
                                                localConnectionsCount);
                continue;
            }
        }
        iterClientConnectionNode = iterClientConnectionNode->next;
    }
}

void updateServers(NodeServerConnection **listServerConnections, int threadId, int *localConnectCount) {
    NodeServerConnection *iterServerConnectionNode = *listServerConnections;

    char buf[BUFFER_SIZE];

    while (iterServerConnectionNode != NULL) {
        ServerConnection *serverConnection = iterServerConnectionNode->connection;
        if (serverConnection->state == CACHING) {
            int result = serverConnection->caching(serverConnection, &cache[serverConnection->cacheIndex], buf, BUFFER_SIZE, threadId);
            if (result != EXIT_SUCCESS) {
                iterServerConnectionNode = iterServerConnectionNode->next;
                if (result == END_READING_PROCCESS) {
                    setCacheStatus(&cache[serverConnection->cacheIndex], VALID);
                    removeServerWrapper("END_READING_PROCCESS SERVER", localConnectCount, listServerConnections,
                                        serverConnection, threadId);
                    continue;
                }
                setCacheStatus(&cache[serverConnection->cacheIndex], INVALID);
                handleCachingException(result, listServerConnections, serverConnection, threadId, localConnectCount);
                continue;
            }
        }
        iterServerConnectionNode = iterServerConnectionNode->next;
    }
}

void *work(void *args) {

    int threadId = *((int *) args);
    printf("START:id: %d\n", threadId);

    int localConnectionsCount = 0;

    struct pollfd fds[2 * MAX_CONNECTIONS];

    NodeClientConnection *listClientConnections = NULL;
    NodeServerConnection *listServerConnections = NULL;

    while (isRun == 1) {
        int newClientSocket = getNewClientSocket(&localConnectionsCount);
        if (newClientSocket != -1) {
            ClientConnection *clientConnection = initClientConnection(newClientSocket);
            pushClientConnectionBack(&listClientConnections, clientConnection);
            printf("New connection for thread=%d local=%d\n", threadId, localConnectionsCount);
        }
        localConnectionsCount = updatePoll(fds, listClientConnections, listServerConnections);
        int polled;
        if (isRun == 1) {
            polled = poll(fds, localConnectionsCount, -1);
        } else {
            break;
        }

        if (polled < 0) {
            perror("poll error");
        } else if (polled == 0) {
            continue;
        }
        updateServers(&listServerConnections, threadId, &localConnectionsCount);
        updateClients(&listClientConnections, &listServerConnections, threadId, &localConnectionsCount);
    }
    printf("before closing threadid-%d\n", threadId);
    printf("End thread-%d\n", threadId);
    pthread_exit(NULL);
}

int main(int argc, const char *argv[]) {
    checkArgs(argc, argv);
    int proxySocketPort = atoi(argv[2]);

    if (initCache(cache, MAX_CACHE_SIZE) == -1) {
        printf("ERROR in initCACHE");
        destroyCache(cache, MAX_CACHE_SIZE);
        pthread_exit(NULL);
    }

    int *threadsId = NULL;
    pthread_t *poolThreads = NULL;

    proxySocket = getProxySocket(proxySocketPort, MAX_NUM_TRANSLATION_CONNECTIONS);
    if (proxySocket < 0) { exit(0); }
    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);
    signal(SIGPIPE, SIG_IGN);

    struct pollfd proxyFds;
    proxyFds.fd = proxySocket;
    proxyFds.events = POLLIN;

    socketsQueue = createQueue();
    if (createThreadPool(poolSize, work, threadsId, &poolThreads) == -1) {
        pthread_exit(NULL);
    }

    pthread_t gbThread;
    if (pthread_create(&gbThread, NULL, garbageCollectorRoutine, NULL)) {
        pthread_exit(NULL);
    }

    while (true) {
        int newClientSocket = acceptPollWrapper(&proxyFds, proxySocket, 1);
        if (sigCaptured) {
            joinThreadPool(poolThreads, poolSize);
            pthread_join(gbThread, NULL);
            break;
        }

        if (newClientSocket != -1) {
            printf("ACCEPTED NEW CONNECTION\n");

            pthread_mutex_lock(&socketsQueue->queueMutex);
            putSocketInQueue(socketsQueue, newClientSocket);
            pthread_mutex_unlock(&socketsQueue->queueMutex);

            pthread_cond_signal(&socketsQueue->condVar);
        } else {
            isRun = 0;
            joinThreadPool(poolThreads, poolSize);
            break;
        }
    }

    destroyCache(cache,MAX_CACHE_SIZE);
    close(proxySocket);
    printf("Close proxy socket");
    pthread_exit(NULL);
}
