#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void *thread1_function(void *arg) {
    printf("Thread 1: Trying to lock mutex1...\n");
    pthread_mutex_lock(&mutex1);
    printf("Thread 1: Locked mutex1.\n");

    // Имитация работы с ресурсом
    sleep(2);

    printf("Thread 1: Trying to lock mutex2...\n");
    pthread_mutex_lock(&mutex2);
    printf("Thread 1: Locked mutex2.\n");

    // Работа с ресурсом

    pthread_mutex_unlock(&mutex2);
    printf("Thread 1: Unlocked mutex2.\n");

    pthread_mutex_unlock(&mutex1);
    printf("Thread 1: Unlocked mutex1.\n");

    return NULL;
}

void *thread2_function(void *arg) {
    printf("Thread 2: Trying to lock mutex2...\n");
    pthread_mutex_lock(&mutex2);
    printf("Thread 2: Locked mutex2.\n");

    // Имитация работы с ресурсом
    sleep(2);

    printf("Thread 2: Trying to lock mutex1...\n");
    pthread_mutex_lock(&mutex1);
    printf("Thread 2: Locked mutex1.\n");

    // Работа с ресурсом

    pthread_mutex_unlock(&mutex1);
    printf("Thread 2: Unlocked mutex1.\n");

    pthread_mutex_unlock(&mutex2);
    printf("Thread 2: Unlocked mutex2.\n");

    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    // Создание потоков
    pthread_create(&thread1, NULL, thread1_function, NULL);
    pthread_create(&thread2, NULL, thread2_function, NULL);

    // Ожидание завершения потоков
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Program finished.\n");

    return 0;
}
