#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <getopt.h>
#include <signal.h>
#include <time.h>

#include "find_min_max.h"
#include "utils.h"

void child_slayer(pid_t child) {
    kill(child, SIGKILL);
}

int main(int argc, char **argv) {
    int seed = -1;
    int array_size = -1;
    int pnum = -1;
    int timeout = -1;
    bool with_files = false;
    while (true) {
        int current_optind = optind ? optind : 1;

        static struct option options[] = {
            {"seed", required_argument, 0, 0},
            {"array_size", required_argument, 0, 0},
            {"pnum", required_argument, 0, 0},
            {"timeout", optional_argument, 0, 0},
            {"by_files", no_argument, 0, 'f'},
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
                        timeout = atoi(optarg);
                        break;
                    case 4:
                        with_files = true;
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
    pid_t child_pids[pnum];
    if (optind < argc) {
        printf("Has at least one no option argument\n");
        return 1;
    }

    if (array_size == -1 || pnum == -1) {
        printf("Usage: %s --array_size \"num\" --pnum \"num\" \n", argv[0]);
        return 1;
    }

    // Если seed не указан, используем текущее время
    if (seed == -1) {
        seed = time(NULL);
    }

    int *array = (int *)malloc(sizeof(int) * array_size);
    GenerateArray(array, array_size, seed);
    int active_child_processes = 0;

    struct timeval start_time;
    gettimeofday(&start_time, NULL);

    int fd[2]; // Дескрипторы для канала
    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    for (int i = 0; i < pnum; i++) {
        pid_t child_pid = fork();
        if (child_pid >= 0) {
            active_child_processes += 1;
            if (child_pid == 0) {
                // Child process

                // Вычислить начало и конец подмассива для текущего процесса
                int begin = i * array_size / pnum;
                int end = (i == pnum - 1) ? array_size : (i + 1) * array_size / pnum;

                if (timeout > 0) {
                    alarm(timeout);
                }
                // Закрыть неиспользуемый конец канала
                if (close(fd[0]) == -1) {
                    perror("close");
                    return 1;
                }

                // Вычислить минимум и максимум для своего подмассива
                struct MinMax local_min_max = GetMinMax(array, begin, end);

                // Записать результаты в канал
                if (write(fd[1], &local_min_max, sizeof(struct MinMax)) == -1) {
                    perror("write");
                    return 1;
                }

                // // Закрыть записывающий конец канала
                // if (close(fd[1]) == -1) {
                //     perror("close");
                //     return 1;
                // }
        return 0;
            }
        } else {
            if (close(fd[1]) == -1) {
                perror("close");
                return 1;
            };
        }
    }


    while (active_child_processes > 0) {
        int status;
        if (wait(&status) > 0) {
            active_child_processes -= 1;
        }
    }

    // Отключение таймера после завершения дочерних процессов, если он был установлен
    if (timeout > 0) {
        sleep(timeout);
        for (int i = 0; i < pnum; ++i) {
            if (!waitpid(-1, NULL, WNOHANG)) {
                kill(child_pids[i], SIGKILL);
            }
        }
        
    }

    struct MinMax min_max;
    min_max.min = INT_MAX;
    min_max.max = INT_MIN;

    for (int i = 0; i < pnum; i++) {
        struct MinMax local_min_max;

        // Прочитать результаты из канала
        if (read(fd[0], &local_min_max, sizeof(struct MinMax)) == -1) {
            perror("read");
            return 1;
        }

        // Обновить глобальный минимум и максимум
        if (local_min_max.min < min_max.min) min_max.min = local_min_max.min;
        if (local_min_max.max > min_max.max) min_max.max = local_min_max.max;
    }

    struct timeval finish_time;
    gettimeofday(&finish_time, NULL);

    double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
    elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

    free(array);

    printf("Min: %d\n", min_max.min);
    printf("Max: %d\n", min_max.max);
    printf("Elapsed time: %fms\n", elapsed_time);

    return 0;
}