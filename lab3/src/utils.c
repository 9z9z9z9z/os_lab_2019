#include "utils.h"

#include <stdio.h>
#include <stdlib.h>


void ReadMinMaxFromFile(const char *filename, struct MinMax *min_max) {
    FILE *file = fopen(filename, "rb"); // Открываем файл для чтения в бинарном режиме

    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    if (fread(min_max, sizeof(struct MinMax), 1, file) != 1) {
        perror("Error reading from file");
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

void WriteMinMaxToFile(const char *filename, struct MinMax min_max) {
    FILE *file = fopen(filename, "wb"); // Открываем файл для записи в бинарном режиме

    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    // Записываем структуру MinMax в файл
    if (fwrite(&min_max, sizeof(struct MinMax), 1, file) != 1) {
        perror("Error writing to file");
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

void GenerateArray(int *array, unsigned int array_size, unsigned int seed) {
  srand(seed);
  for (int i = 0; i < array_size; ++i) {
    if (array != NULL) {
      array[i] = rand();
    } else {
      printf("nullptr");
    }    
  }
}
