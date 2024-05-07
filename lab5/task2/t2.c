#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int global_var = 5;
int wait_time = 3;

int main() {
    int local_var = 10;

    printf("global: %d, local: %d\n", global_var, local_var);
    printf("parent pid: %d\n", getpid());

    sleep(wait_time);
    printf("\n");

    const int parent_id = getpid();

    int fork_pid = fork();

    if (fork_pid == 0) {
        printf("Parent pid from child proc: %d\n", parent_id);
        printf("Child pid from child proc: %d\n", getpid());
    } else {
        printf("Parent pid from parent proc: %d\n", getpid());
    }

    sleep(wait_time);
    printf("\n");

    /*if (fork_pid == 0) {
        printf("global_var from child: %d\n", global_var);
        printf("local_var from child: %d\n", local_var);
    } else {
        printf("global_var from parent: %d\n", global_var);
        printf("local_var from parent: %d\n", local_var);
    }
    sleep(wait_time);
    printf("\n");*/

    // if (fork_pid == 0) {
    //     global_var = 50;
    //     local_var = 100;
    //     printf("global_var from child: %d\n", global_var);
    //     printf("local_var from child: %d\n", local_var);
    // } else {
    //     sleep(5);
    //     printf("global_var from parent: %d\n", global_var);
    //     printf("local_var from parent: %d\n", local_var);
    // }

    /*if (fork_pid == 0) {
        sleep(5);
        printf("global_var from child: %d\n", global_var);
        printf("local_var from child: %d\n", local_var);
    } else {
        global_var = 50;
        local_var = 100;
        printf("global_var from parent: %d\n", global_var);
        printf("local_var from parent: %d\n", local_var);
    }*/
    
    sleep(10);
    printf("\n");

    if (fork_pid == 0) {
        sleep(10);
        printf("child process terminated\n");
        exit(0);
    } else {
        printf("parent exit");
        exit(0);
    }
    
    return 0;
}
