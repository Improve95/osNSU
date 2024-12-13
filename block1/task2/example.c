#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <dirent.h> 
#include <sys/stat.h>
#include <unistd.h>

/* я не понял прикола про стэк, а именно про то, что у нас стэк окружен зоной, где нет никаких прав
и что структура потока лежит внизу стэка, в какой то особенной зоне, короч хз */

typedef void *(*start_routine_t) (void*);

typedef struct {
    int             mythread_id;
    start_routine   start_routine_t;
    void            *arg;
    int             pid;

    volatile ine finished;
    volatile int joined;

    ucontext_t before_start_routine; /* 
    для возвращения к состоянию до вызова функции start_routine*/

    /*
    * закидываем регистры процессора, чтобы можно было бы переместиться к месту
    * перед началом функции
    */
} mypthread;

void *thread_startup(void *arg) {
    //init

    // вызов функции пользователя
    // нужно проверять, были ли мы уже здесь или нет
    start_routine()

    //finish

    wait_until_join();

    //очистка стека

}

void thread_cancel() {
    //проблема в остановке функции которую передал пользователь(бля, логично)
}

void create_stack() {
    //todo спиздить с лекции с записи
}

void my_thread(my_thread *thread, void *(*start_routine), (void *) void *arg) {
    mypthread my_thread = new mythread();

    create_stack();

    clone(start_routine, arg, );
    thread = my_thread;
}

int main() {

    return 0;
}
