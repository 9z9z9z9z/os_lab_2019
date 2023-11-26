#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int *mut1;
int *mut2;

void initialize_shared_memory() {
    key_t key = ftok("/tmp", 'z');
    int shmid = shmget(key, sizeof(int) * 2, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    mut1 = (int*)shmat(shmid, NULL, 0);
    mut2 = mut1 + 1;
}

void cleanup_shared_memory() {
    shmdt((void*)mut1);
}

void lock_mutex(int *mut) {
    asm volatile(
        "lock:\n"
        "    xorl %%eax, %%eax\n"
        "    incl %%eax\n"
        "    lock xchg %%eax, %0\n"
        "    test %%eax, %%eax\n"
        "    jnz lock"
        : "+m" (*mut)
        :
        : "%eax"
    );
}

void unlock_mutex(int *mut) {
    asm volatile(
        "movl $0, %0"
        : "+m" (*mut)
        :
    );
}

void *thread1() {
    printf("Thread 1: started\n");
    lock_mutex(mut1);
    
    sleep(2);
    
    lock_mutex(mut2);

    unlock_mutex(mut2);

    unlock_mutex(mut1);

    printf("Thread 1: ended\n");

    return NULL;
}

void *thread2() {
    printf("Thread 2: started\n");

    lock_mutex(mut2);
    sleep(2);
    
    lock_mutex(mut1);

    unlock_mutex(mut1);

    unlock_mutex(mut2);

    printf("Thread 2: ended\n");

    return NULL;
}

int main() {
    initialize_shared_memory();

    pid_t pid1, pid2;

    pid1 = fork();

    if (pid1 == 0) {
        // Child process
        thread1();
        return 0;
    } else if (pid1 < 0) {
        perror("fork");
        return 1;
    }

    pid2 = fork();

    if (pid2 == 0) {
        // Another child process
        thread2();
        return 0;
    } else if (pid2 < 0) {
        perror("fork");
        return 1;
    }

    // Wait for both child processes to complete
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    cleanup_shared_memory();

    return 0;
}