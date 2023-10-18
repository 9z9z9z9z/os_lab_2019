#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <getopt.h>
#include <stdint.h>

// Структура, которая будет передаваться потокам
struct FactorialArgs {
    uint64_t k;
    uint64_t mod;
    uint64_t result;
    pthread_mutex_t *mutex;
};

// Функция для вычисления факториала
void *Factorial(void *args) {
    struct FactorialArgs *fargs = (struct FactorialArgs *)args;
    uint64_t k = fargs->k;
    uint64_t mod = fargs->mod;
    uint64_t result = 1;

    for (uint64_t i = 1; i <= k; i++) {
        result = (result * i) % mod;
    }
    pthread_mutex_lock(fargs->mutex);
    fargs->result = (fargs->result * result) % mod;
    pthread_mutex_unlock(fargs->mutex);

    return NULL;
}

int main(int argc, char *argv[]) {
    uint64_t k = 0;
    uint64_t mod = 0;
    int pnum = 1;

    while (1) {
        int current_optind = optind ? optind : 1;

        static struct option options[] = {
            {"k", required_argument, 0, 0},
            {"mod", required_argument, 0, 0},
            {"pnum", required_argument, 0, 0},
            {0, 0, 0, 0}
        };

        int option_index = 0;
        int c = getopt_long(argc, argv, "k:", options, &option_index);

        if (c == -1) break;

        switch (c) {
            case 0:
                switch (option_index) {
                    case 0:
                        k = strtoull(optarg, NULL, 10);
                        break;
                    case 1:
                        mod = strtoull(optarg, NULL, 10);
                        break;
                    case 2:
                        pnum = atoi(optarg);
                        break;
                    default:
                        printf("Index %d is out of options\n", option_index);
                }
                break;
            default:
                printf("getopt returned character code 0%o?\n", c);
        }
    }

    if (k == 0 || mod == 0 || pnum <= 0) {
        printf("Usage: %s --k 10 --mod 10 --pnum 4\n", argv[0]);
        return 1;
    }

    pthread_t *threads = (pthread_t *)malloc(pnum * sizeof(pthread_t));
    struct FactorialArgs *thread_args = (struct FactorialArgs *)malloc(pnum * sizeof(struct FactorialArgs));
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    uint64_t result = 1;

    for (int i = 0; i < pnum; i++) {
        thread_args[i].k = k / pnum;
        thread_args[i].mod = mod;
        thread_args[i].result = 1;
        thread_args[i].mutex = &mutex;

        if (i == pnum - 1) {
            thread_args[i].k += k % pnum;
        }

        if (pthread_create(&threads[i], NULL, Factorial, (void *)&thread_args[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    for (int i = 0; i < pnum; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < pnum; i++) {
        result = (result * thread_args[i].result) % mod;
    }

    if (!result) {
        result = 1;
    }

    printf("%lu! mod %lu = %lu\n", k, mod, result);

    free(threads);
    free(thread_args);
    pthread_mutex_destroy(&mutex);

    return 0;
}
