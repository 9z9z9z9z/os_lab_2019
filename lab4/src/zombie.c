#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t child_pid = fork();
    if (child_pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    if (child_pid == 0) {
        printf("Child process is running.\n");
        sleep(20); 
        printf("Child process is exiting.\n");
    } else {
        printf("Parent process is running.\n");
        printf("Parent process is exiting without waiting for the child.\n");
        exit(0);
    }
    return 0;
}
