#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <pthread.h>
#include <getopt.h>

#include "utils.h"

struct SumArgs {
  int *array;
  int begin;
  int end;
};

int Sum(const struct SumArgs *args) {
  int sum = 0;
  for (int i = args->begin; i < args->end; ++i) {
    sum += args->array[i];
  }
  return sum;
}

void *ThreadSum(void *args) {
  struct SumArgs *sum_args = (struct SumArgs *)args;
  return (void *)(size_t)Sum(sum_args); 
}

int main(int argc, char **argv) {
  uint32_t threads_num = 0;
  uint32_t array_size = 0;
  uint32_t seed = 0;
  while(true) {
    int current_optind = optind ? optind : 1;
    static struct option options[] = {
      {"threads_num", required_argument, 0, 0},
      {"seed", required_argument, 0, 0},
      {"array_size", required_argument, 0, 0},
      {0, 0, 0, 0}
    };
    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);
    
    if (c == -1) 
      break;
    switch (c)
    {
    case 0:
      switch (option_index)
      {
        case 0:
          threads_num = atoi(optarg);
          break;
        case 1:
          seed = atoi(optarg);
          break;
        case 2:
          array_size = atoi(optarg);
          break;
        default:
        printf("Index %d is out of options\n", option_index);
      }
      break;
    case '?':
      break;
    default:
      printf("getopt returned character code 0%o?\n", c);
    }
  }

  pthread_t threads[threads_num];

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);

  struct SumArgs args[threads_num];
  for (uint32_t i = 0; i < threads_num; i++) {
    args[i].array = array;
    args[i].begin = 0;
    args[i].end = array_size;
    bool status = pthread_create(&threads[i], NULL, ThreadSum, (void *)(&args));
    if (status) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }

  int total_sum = 0;
  for (uint32_t i = 0; i < threads_num; i++) {
    int sum = 0;
    pthread_join(threads[i], (void **)&sum);
    total_sum += sum;
  }

  free(array);
  printf("Total: %d\n", total_sum / threads_num);
  return 0;
}