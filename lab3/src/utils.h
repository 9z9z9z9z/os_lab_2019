#ifndef UTILS_H
#define UTILS_H

struct MinMax {
  int min;
  int max;
};

void WriteMinMaxToFile(const char *fileName, struct MinMax);
void ReadMinMaxFromFile(const char *filename, struct MinMax *min_max);
void GenerateArray(int *array, unsigned int array_size, unsigned int seed);

#endif
