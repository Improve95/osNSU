#include <stdbool.h>
#include "../headers/pthreadService.h"
#include "../headers/cache.h"

int searchUrlInCacheConcurrent(char *url, CacheEntry *cache, int cacheSize) {
    for (int j = 0; j < cacheSize; j++) {
//        printf("searchUrlInCacheConcurrent lock...\n");
        pthread_mutex_lock(&cache[j].mutex);

        if (cache[j].url != NULL && strcmp(cache[j].url, url) == 0) {
            if (cache[j].status == VALID || cache[j].status == DOWNLOADING) {
                cache[j].readers++;
                pthread_mutex_unlock(&cache[j].mutex);
//                printf("searchUrlInCacheEND\n");
                return j;
            }

            pthread_mutex_unlock(&cache[j].mutex);
            // infoPrintf("searchUrlInCacheEND");
        } else {
            pthread_mutex_unlock(&cache[j].mutex);
            // infoPrintf("searchUrlInCacheEND ");
        }
    }
    return -1;
}

int searchFreeCacheConcurrent(char *url, CacheEntry *cache, int cacheSize, int threadId) {
    for (int j = 0; j < cacheSize; j++) {
//        printf("searchFreeCacheConcurrent lock...\n");
        pthread_mutex_lock(&cache[j].mutex);
        if (cache[j].url == NULL) {
//            printf("(%d)SEARCH_CACHE: found free cache id=%d\n", threadId, j);
            cache[j].readers = 1;
            cache[j].status = DOWNLOADING;
            cache[j].data = initDataCacheList();
            cache[j].numChunks = 0;
            cache[j].allSize = 0;
            cache[j].recvSize = 0;
            cache[j].url = (char *) malloc(sizeof(char) * strlen(url) + 1);
            memcpy(cache[j].url, url, sizeof(char) * strlen(url) + 1);

            pthread_mutex_unlock(&cache[j].mutex);
//            printf("searchFreeCacheAndSetDownloadingStateEND\n");
            return j;
        } else {
            pthread_mutex_unlock(&cache[j].mutex);
//            printf("searchFreeCacheAndSetDownloadingStateEND\n");
        }
    }
    return -1;
}

int searchNotUsingCacheConcurrent(char *url, CacheEntry *cache, int cacheSize, int threadId) {
    for (int j = 0; j < cacheSize; j++) {
//        printf("searchNotUsingCacheConcurrent...\n");
        pthread_mutex_lock(&cache[j].mutex);

        if (/*cache[j].readers == 0 || */cache[j].status == INVALID) {
            perror("thread (%d) search_cache: found not using cache id=%d");
//            printf("thread (%d) search_cache: found not using cache id=%d", threadId, j);
            cache[j].readers = 1;
            cache[j].status = DOWNLOADING;
            cache[j].data = initDataCacheList();
            cache[j].numChunks = 0;
            cache[j].allSize = 0;
            cache[j].recvSize = 0;

            freeList(cache[j].data);
            free(cache[j].url);
            cache[j].url = (char *) malloc(sizeof(char) * sizeof(url));
            memcpy(cache[j].url, url, sizeof(char) * sizeof(url));

            pthread_mutex_unlock(&cache[j].mutex);
//            printf("searchNotUsingCacheConcurrent\n");
            return j;
        } else {
            pthread_mutex_unlock(&cache[j].mutex);
//            printf("searchNotUsingCacheEND\n");
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

        cache[i].readers = 0;
        cache[i].data = initDataCacheList();
        cache[i].numChunks = 0;
        erCVC = initCondVariable(&cache[i].chunksCondVar);
        erMC = initMutex(&cache[i].chunksMutex);

        cache[i].url = NULL;
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
//    printf("destroy cache\n");
}

int putDataToCache(CacheEntry *cacheChunk, char *newData, int lengthNewData, int threadId) {
    pushDataCacheBack(cacheChunk->data, newData, lengthNewData);
    cacheChunk->recvSize += lengthNewData;
//    printf("thread %d: putDataToCache: chunksMutex...\n", threadId);
    pthread_mutex_lock(&cacheChunk->chunksMutex);
    cacheChunk->numChunks++;
    pthread_mutex_unlock(&cacheChunk->chunksMutex);
//    printf("putDataToCache: chunksMutex\n");
    return 0;
}

void addReader(CacheEntry *cacheInfo) {

}

void removeReader(CacheEntry *cacheInfo) {

}

void setCacheStatus(CacheEntry *cacheInfo, CacheStatus status) {
//    printf("setCacheStatus...\n");
    pthread_mutex_lock(&cacheInfo->mutex);
    cacheInfo->status = status;
    pthread_mutex_unlock(&cacheInfo->mutex);
//    printf("setCacheStatusEND\n");
}

CacheStatus getCacheStatus(CacheEntry *cacheInfo) {
//    printf("getCacheStatus\n");
    CacheStatus returnStatus;
    pthread_mutex_lock(&cacheInfo->mutex);
    returnStatus = cacheInfo->status;
    pthread_mutex_unlock(&cacheInfo->mutex);
//    printf("getCacheStatusEND\n");
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
