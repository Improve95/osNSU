#define _GNU_SOURCE
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *block_thread(void *arg) {
    sigset_t signal_set;
    sigfillset(&signal_set);
    pthread_sigmask(SIG_BLOCK, &signal_set, NULL);

    printf("block_thread [%d %d %d %ld]: blocked all signals\n", 
        getpid(), getppid(), gettid(), pthread_self());
}

void sigint_handler_v2(int signo) {
    printf("sigint_handler_v2 [%d %d %d]: receive sigint\n", 
        getpid(), getppid(), gettid());
}

void sigint_handler(int signo) {
    printf("sigint_handler_v1 [%d %d %d]: receive sigint\n", 
        getpid(), getppid(), gettid());
}

void *sigint_thread_v2(void *arg) {
    struct sigaction sa;
    sa.sa_handler = sigint_handler_v2;

    sigset_t set; 
    sigemptyset(&set);                                                             
    sigaddset(&set, SIGINT);

    sa.sa_mask = set; 
    sigaction(SIGINT, &sa, NULL);
    
    printf("sigint_handler_v2 [%d %d %d %ld]: Set up sigint handler\n", 
        getpid(), getppid(), gettid(), pthread_self());

    sleep(10);
}

void *sigint_thread(void *arg) {
    sleep(2);
    struct sigaction sa;
    sa.sa_handler = sigint_handler;

    sigset_t set; 
    sigemptyset(&set);                                                             
    sigaddset(&set, SIGINT);
    sa.sa_mask = set; 

    sigaction(SIGINT, &sa, NULL);
    
    printf("sigint_handler_v1 [%d %d %d %ld]: Set up sigint handler\n", 
        getpid(), getppid(), gettid(), pthread_self());

    sleep(10);
}

void *wait_sigquit_thread_v2(void *arg) {
    int sig;

    sigset_t signal_set;
    sigemptyset(&signal_set);
    sigaddset(&signal_set, SIGQUIT);

    printf("wait_sigquit_thread_v2 [%d %d %d %ld]: wait for sigquit\n", 
        getpid(), getppid(), gettid(), pthread_self());

    sigwait(&signal_set, &sig);

    printf("wait_sigquit_thread_v2 [%d %d %d %ld]: receive sigquit\n", 
        getpid(), getppid(), gettid(), pthread_self());
}

void *wait_sigquit_thread(void *arg) {
    int sig;

    sigset_t signal_set;
    sigemptyset(&signal_set);
    sigaddset(&signal_set, SIGQUIT);

    printf("wait_sigquit_thread_v1 [%d %d %d %ld]: wait for sigquit\n", 
        getpid(), getppid(), gettid(), pthread_self());

    sigwait(&signal_set, &sig);

    printf("wait_sigquit_thread_v1 [%d %d %d %ld]: receive sigquit\n", 
        getpid(), getppid(), gettid(), pthread_self());
}

int main() {
    pthread_t tid1, tid2, tid3, tid4, tid5;

    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    
    pthread_create(&tid1, NULL, block_thread, NULL);
    printf("main [%d %d %d]: create thread %ld\n", getpid(), getppid(), gettid(), tid1);

    pthread_create(&tid2, NULL, sigint_thread, NULL);
    printf("main [%d %d %d]: create thread %ld\n", getpid(), getppid(), gettid(), tid2);

    pthread_create(&tid3, NULL, sigint_thread_v2, NULL);
    printf("main [%d %d %d]: create thread %ld\n", getpid(), getppid(), gettid(), tid3);

    /* pthread_create(&tid4, NULL, wait_sigquit_thread, NULL);
    printf("main [%d %d %d]: create thread %ld\n", getpid(), getppid(), gettid(), tid4);

    pthread_create(&tid5, NULL, wait_sigquit_thread_v2, NULL);
    printf("main [%d %d %d]: create thread %ld\n", getpid(), getppid(), gettid(), tid5); */

    /* здесь в /proc/pid/status будет висеть три заблокированных сигнала */
    pthread_kill(tid1, SIGINT);
    pthread_kill(tid1, SIGSEGV);
    pthread_kill(tid1, SIGQUIT);

    /* здесь переписывается структура в которой хранится функция обрабатывающая сигнал
        и вызовется та, которая записана последней 
        (там приколы с act и old act, по исходникам посмотреть надо)*/
    sleep(3);
    pthread_kill(tid2, SIGINT);
    pthread_kill(tid3, SIGINT);

    /* здесь обработка сигналов произойдет в случайно порядке, 
        каждый поток может обработать свой сигнал раньше/позже предыдущего */
    /* sleep(1);
    pthread_kill(tid4, SIGQUIT);
    pthread_kill(tid5, SIGQUIT); */

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL); 
    // pthread_join(tid4, NULL);
    // pthread_join(tid5, NULL); 

    sleep(1);

    return 0;
}
