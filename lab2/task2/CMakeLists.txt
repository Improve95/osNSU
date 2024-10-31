cmake_minimum_required(VERSION 3.5.1)
project(syncronized C)

set(SRC queue.c t2a.c)
# set(SRC queue.c t2a.c)
# set(SRC queue.c t2a.c)
# set(SRC queue.c t2a.c)
add_executable(${PROJECT_NAME} ${SRC})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")