#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

int active_child_processes = 0;

void handle_child(int signo) {
    // Обработка завершения дочерних процессов
    while (waitpid(-1, NULL, WNOHANG) > 0) {
        active_child_processes--;
    }
}

void handle_timeout(int signo) {
    // Обработка таймаута
    printf("Timeout expired. Sending SIGKILL to child processes...\n");
    // Отправляем SIGKILL всем дочерним процессам
    for (int i = 0; i < active_child_processes; i++) {
        kill(SIGKILL, 0);
    }
}

int main(int argc, char **argv) {
    int seed = -1;
    int array_size = -1;
    int pnum = -1;
    bool with_files = false;
    int timeout = 0;

    while (true) {
        int current_optind = optind ? optind : 1;

        static struct option options[] = {
            {"seed", required_argument, 0, 0},
            {"array_size", required_argument, 0, 0},
            {"pnum", required_argument, 0, 0},
            {"by_files", no_argument, 0, 'f'},
            {"timeout", required_argument, 0, 0},
            {0, 0, 0, 0}
        };

        int option_index = 0;
        int c = getopt_long(argc, argv, "f", options, &option_index);

        if (c == -1) break;

        switch (c) {
            case 0:
                switch (option_index) {
                    case 0:
                        seed = atoi(optarg);
                        break;
                    case 1:
                        array_size = atoi(optarg);
                        break;
                    case 2:
                        pnum = atoi(optarg);
                        break;
                    case 3:
                        with_files = true;
                        break;
                    case 4:
                        timeout = atoi(optarg);
                        break;
                    default:
                        printf("Index %d is out of options\n", option_index);
                }
                break;
            case 'f':
                with_files = true;
                break;
            case '?':
                break;
            default:
                printf("getopt returned character code 0%o?\n", c);
        }
    }

    if (optind < argc) {
        printf("Has at least one no option argument\n");
        return 1;
    }

    if (seed == -1 || array_size == -1 || pnum == -1) {
        printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" [--timeout \"num\"]\n", argv[0]);
        return 1;
    }

    int *array = malloc(sizeof(int) * array_size);
    GenerateArray(array, array_size, seed);

    // Установка обработчика сигнала для завершения дочерних процессов
    signal(SIGCHLD, handle_child);

    struct timeval start_time;
    gettimeofday(&start_time, NULL);

    if (timeout > 0) {
        // Установка таймаута с использованием функции alarm
        signal(SIGALRM, handle_timeout);
        alarm(timeout);
    }

    int *pipefd = NULL; // Для хранения дескрипторов pipe

    for (int i = 0; i < pnum; i++) {
        if (!with_files) {
            // Создаем pipes только если не используем файлы
            pipefd = malloc(sizeof(int) * 2);
            if (pipe(pipefd) == -1) {
                perror("pipe");
                return 1;
            }
        }

        pid_t child_pid = fork();
        if (child_pid >= 0) {
            active_child_processes += 1;
            if (child_pid == 0) {
                // Дочерний процесс

                // Найдите минимум и максимум в своей части массива
                struct MinMax local_min_max;
                local_min_max = GetMinMax(array, i * (array_size / pnum), (i + 1) * (array_size / pnum));

                if (with_files) {
                    // Используйте файлы для записи результатов
                } else {
                    // Используйте pipe для передачи результатов
                    close(pipefd[0]);
                    write(pipefd[1], &local_min_max, sizeof(struct MinMax));
                    close(pipefd[1]);
                }

                free(array);

                if (!with_files) {
                    free(pipefd); // Освобождаем память, если используется pipe
                }

                return 0;
            }
        } else {
            printf("Fork failed!\n");
            return 1;
        }
    }

    while (active_child_processes > 0) {
        // Ожидаем завершения дочерних процессов
        pause();
    }

    struct MinMax min_max;
    min_max.min = INT_MAX;
    min_max.max = INT_MIN;

    if (!with_files) {
        // Считать результаты из pipe
        for (int i = 0; i < pnum; i++) {
            struct MinMax local_min_max;
            read(pipefd[0], &local_min_max, sizeof(struct MinMax));
            close(pipefd[0]);
            if (local_min_max.min < min_max.min) min_max.min = local_min_max.min;
            if (local_min_max.max > min_max.max) min_max.max = local_min_max.max;
        }
        free(pipefd);
    }

    struct timeval finish_time;
    gettimeofday(&finish_time, NULL);

    double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
    elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

    free(array);

    printf("Min: %d\n", min_max.min);
    printf("Max: %d\n", min_max.max);
    printf("Elapsed time: %fms\n", elapsed_time);
    fflush(NULL);

    return 0;
}
