#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    printf("%d", argc);
    // Создаем дочерний процесс
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // В дочернем процессе
        execv("./sequential_min_max", argv);
        perror("execl");
        exit(EXIT_FAILURE);
    } else {
        // В родительском процессе
        int status;
        waitpid(child_pid, &status, 0);
        printf("Child process exited with status %d\n", status);
    }

    return 0;
}
