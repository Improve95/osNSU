#include <stdbool.h>
#include "../headers/pthreadService.h"
#include "../headers/cache.h"

int searchUrlInCacheConcurrent(char *url, CacheEntry *cache, int cacheSize) {
    for (int j = 0; j < cacheSize; j++) {
        pthread_mutex_lock(&cache[j].mutex);
        if (cache[j].url != NULL && strcmp(cache[j].url, url) == 0) {
            if (cache[j].status == VALID || cache[j].status == DOWNLOADING) {
                cache[j].lastGetTime = time(NULL);
                cache[j].readers++;
                pthread_mutex_unlock(&cache[j].mutex);
                return j;
            }

            pthread_mutex_unlock(&cache[j].mutex);
        } else {
            pthread_mutex_unlock(&cache[j].mutex);
        }
    }
    return -1;
}

int searchFreeCacheConcurrent(char *url, CacheEntry *cache, int cacheSize, int threadId) {
    for (int j = 0; j < cacheSize; j++) {
        pthread_mutex_lock(&cache[j].mutex);
        if (cache[j].url == NULL) {
            cache[j].lastGetTime = time(NULL);
            cache[j].data = initDataCacheList();
            cache[j].status = DOWNLOADING;
            cache[j].readers = 1;
            cache[j].numChunks = 0;
            cache[j].allSize = 0;
            cache[j].recvSize = 0;
            cache[j].url = (char *) malloc(sizeof(char) * strlen(url) + 1);
            memcpy(cache[j].url, url, sizeof(char) * strlen(url) + 1);

//            printf("found free cache\n");
            pthread_mutex_unlock(&cache[j].mutex);
            return j;
        } else {
            pthread_mutex_unlock(&cache[j].mutex);
        }
    }
    return -1;
}

int searchNotUsingCacheConcurrent(char *url, CacheEntry *cache, int cacheSize, int threadId) {
    for (int j = 0; j < cacheSize; j++) {
        pthread_mutex_lock(&cache[j].mutex);

        if (cache[j].status == INVALID) {
            cache[j].lastGetTime = time(NULL);
            cache[j].data = initDataCacheList();
            cache[j].status = DOWNLOADING;
            cache[j].readers = 1;
            cache[j].numChunks = 0;
            cache[j].allSize = 0;
            cache[j].recvSize = 0;

            freeList(cache[j].data);
            free(cache[j].url);
            cache[j].url = (char *) malloc(sizeof(char) * sizeof(url));
            memcpy(cache[j].url, url, sizeof(char) * sizeof(url));

            printf("found not using cache\n");
            pthread_mutex_unlock(&cache[j].mutex);
            return j;
        } else {
            pthread_mutex_unlock(&cache[j].mutex);
        }
    }
    return -1;
}

void makeCacheInvalid(CacheEntry *cache) {
    setCacheStatus(cache, INVALID);
    pthread_cond_broadcast(&cache->chunksCondVar);
}

int initCache(CacheEntry *cache, const int maxCacheSize) {
    bool erMS, erCVC, erMC;
    for (int i = 0; i < maxCacheSize; i++) {
        cache[i].allSize = 0;
        cache[i].recvSize = 0;
        erMS = initMutex(&cache[i].mutex);

        cache[i].status = INVALID;
        cache[i].readers = 0;
        cache[i].data = initDataCacheList();
        cache[i].numChunks = 0;
        erCVC = initCondVariable(&cache[i].chunksCondVar);
        erMC = initMutex(&cache[i].chunksMutex);

        cache[i].url = NULL;

        cache[i].lastGetTime = 0;
    }
    return erMS && erCVC && erMC;
}

void destroyCache(CacheEntry *cache, const int maxCacheSize) {
    for (int i = 0; i < maxCacheSize; i++) {

        pthread_mutex_destroy(&cache[i].mutex);
        freeList(cache[i].data);
        pthread_cond_destroy(&cache[i].chunksCondVar);
        pthread_mutex_destroy(&cache[i].chunksMutex);

        free(cache[i].url);
    }
}

int putDataToCache(CacheEntry *cacheInfo, char *newData, int lengthNewData, int threadId) {
    pushDataCacheBack(cacheInfo->data, newData, lengthNewData);
    cacheInfo->recvSize += lengthNewData;
    pthread_mutex_lock(&cacheInfo->chunksMutex);
    cacheInfo->numChunks++;
    pthread_mutex_unlock(&cacheInfo->chunksMutex);
    return 0;
}

void removeReader(CacheEntry *cacheInfo) {
    pthread_mutex_lock(&cacheInfo->mutex);
//    printf("remove reader\n");
    cacheInfo->readers -= 1;
    pthread_mutex_unlock(&cacheInfo->mutex);
}

void setCacheStatus(CacheEntry *cacheInfo, CacheStatus status) {
    pthread_mutex_lock(&cacheInfo->mutex);
    cacheInfo->status = status;
    pthread_mutex_unlock(&cacheInfo->mutex);
}

CacheStatus getCacheStatus(CacheEntry *cacheInfo) {
    CacheStatus returnStatus;
    pthread_mutex_lock(&cacheInfo->mutex);
    returnStatus = cacheInfo->status;
    pthread_mutex_unlock(&cacheInfo->mutex);
    return returnStatus;
}

int getCacheRecvSize(CacheEntry *cacheInfo) {
    return cacheInfo->recvSize;
}

int getCacheAllSize(CacheEntry *cacheInfo) {
    CacheStatus returnAllSize;
    returnAllSize = cacheInfo->allSize;
    return returnAllSize;
}

void setCacheAllSize(CacheEntry *cacheInfo, int allSize) {
    cacheInfo->allSize = allSize;
}

int broadcastWaitingCacheClients(CacheEntry *cacheChunk) {
    pthread_cond_broadcast(&cacheChunk->chunksCondVar);
}
