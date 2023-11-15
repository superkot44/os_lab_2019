#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

long long result = 1;
int k, pnum, mod;

void *factorial_thread(void *arg) {
    int thread_id = *((int *)arg);
    int start = thread_id + 1;
    long long local_result = 1;

    for (int i = start; i <= k; i += pnum) {
        local_result = (local_result * i) % mod;
    }

    // Защита от одновременного доступа к глобальному результату
    pthread_mutex_lock(&mutex);
    result = (result * local_result) % mod;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 7) {
        printf("Usage: %s -k <number> --pnum <number> --mod <number>\n", argv[0]);
        return 1;
    }

    // Парсинг входных параметров
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-k") == 0) {
            k = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "--pnum") == 0) {
            pnum = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "--mod") == 0) {
            mod = atoi(argv[i + 1]);
        }
    }

    // Создание потоков
    pthread_t *threads = malloc(pnum * sizeof(pthread_t));
    int *thread_ids = malloc(pnum * sizeof(int));

    for (int i = 0; i < pnum; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, factorial_thread, &thread_ids[i]);
    }

    // Ожидание завершения потоков
    for (int i = 0; i < pnum; i++) {
        pthread_join(threads[i], NULL);
    }

    // Вывод результата
    printf("Factorial of %d mod %d is %lld\n", k, mod, result);

    // Освобождение ресурсов
    free(threads);
    free(thread_ids);

    return 0;
}
