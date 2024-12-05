cmake_minimum_required(VERSION 3.16)
project(proxyV3 C)

set(CMAKE_C_STANDARD 99)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=thread")

include_directories("services/")

add_executable(proxyV3
        main.c
        sources/queueService.c
        sources/pthreadService.c
        sources/threadPool.c
        sources/cache.c
        sources/httpService.c
        sources/serverSockerService.c
        sources/logger.c
        sources/clientConnection.c
        sources/serverConnection.c
        sources/cacheList.c
        sources/clientConnectionList.c
        sources/serverConnectionList.c)