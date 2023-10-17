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
        // Это код дочернего процесса
        printf("Child process is running.\n");
        sleep(2); // Дочерний процесс ждет 2 секунды
        printf("Child process is exiting.\n");
    } else {
        // Это код родительского процесса
        printf("Parent process is running.\n");
        printf("Parent process is exiting without waiting for the child.\n");
    }

    return 0;
}
