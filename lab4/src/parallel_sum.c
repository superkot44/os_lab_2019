#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "utils.h" // Включаем заголовочный файл с функцией GenerateArray

// Структура для передачи аргументов в поток
struct SumArgs {
  int *array;
  int begin;
  int end;
};

// Функция для вычисления суммы внутри заданного диапазона
int Sum(const struct SumArgs *args) {
  int sum = 0;
  for (int i = args->begin; i < args->end; i++) {
    sum += args->array[i];
  }
  return sum;
}

void *ThreadSum(void *args) {
  struct SumArgs *sum_args = (struct SumArgs *)args;
  return (void *)(size_t)Sum(sum_args);
}

int main(int argc, char **argv) {
  if (argc != 7) {
    printf("Usage: %s --threads_num <num> --seed <num> --array_size <num>\n", argv[0]);
    return 1;
  }

  // Извлекаем аргументы из командной строки
  uint32_t threads_num = atoi(argv[2]);
  uint32_t seed = atoi(argv[4]);
  uint32_t array_size = atoi(argv[6]);
  
  pthread_t threads[threads_num];
  struct SumArgs args[threads_num];

  // Генерируем массив с заданным seed и размером
  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);

  int step = array_size / threads_num;
  int start = 0;
  for (uint32_t i = 0; i < threads_num; i++) {
    args[i].array = array;
    args[i].begin = start;
    args[i].end = (i == threads_num - 1) ? array_size : start + step;
    start += step;

    if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args[i])) {
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
  printf("Total: %d\n", total_sum);
  return 0;
}
