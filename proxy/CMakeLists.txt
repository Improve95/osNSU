cmake_minimum_required(VERSION 3.16)
project(proxy C)

set(CMAKE_C_STANDARD 99)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=thread")

add_executable(proxy
        main.c
        sources/queueService.c
        sources/pthreadService.c
        sources/threadPool.c
        sources/cache.c
        sources/httpService.c
        sources/serverSocketService.c
        sources/clientConnection.c
        sources/serverConnection.c
        sources/cacheList.c
        sources/clientConnectionList.c
        sources/serverConnectionList.c)