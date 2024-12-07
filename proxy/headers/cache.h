#ifndef CACHE_H
#define CACHE_H

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pthreadService.h"
#include "cacheList.h"

enum cache_status {
    DOWNLOADING,
    VALID,
    INVALID
} typedef CacheStatus;

struct cache_entry {
    size_t allSize;
    size_t recvSize;
    pthread_mutex_t mutex;
    size_t readers;

    ListCacheData *data;
    size_t numChunks;
    pthread_cond_t chunksCondVar;
    pthread_mutex_t chunksMutex;
    char *url;
    CacheStatus status;
} typedef CacheEntry;

void setCacheStatus(CacheEntry *cacheInfo, CacheStatus status);

CacheStatus getCacheStatus(CacheEntry *cacheInfo);

int getCacheRecvSize(CacheEntry *cacheInfo);

int getCacheAllSize(CacheEntry *cacheInfo);

void setCacheAllSize(CacheEntry *cacheInfo, int allSize);

int putDataToCache(CacheEntry *cacheChunk, char *newData, int lengthNewData);

void destroyCache(CacheEntry *cache, int maxCacheSize);

int initCache(CacheEntry *cache, int maxCacheSize);

void makeCacheInvalid(CacheEntry *cache);

/**
 * If not using cache exits return index cache or else return -1
 * */
int searchNotUsingCacheConcurrent(char *url, CacheEntry *cache, int cacheSize, int threadId);

/**
 * If free cache exits  return index cache or else return -1
 * */
int searchFreeCacheConcurrent(char *url, CacheEntry *cache, int cacheSize, int threadId);

/**
 * If url exits return index cache or else return -1
 * */
int searchUrlInCacheConcurrent(char *url, CacheEntry *cache, int cacheSize);

int broadcastWaitingCacheClients(CacheEntry *cacheChunk);

#endif //CACHE_H
