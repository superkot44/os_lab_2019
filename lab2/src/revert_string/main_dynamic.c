// main_dynamic.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h> // Для динамической загрузки библиотеки

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s string_to_revert\n", argv[0]);
        return -1;
    }

    char *reverted_str = malloc(sizeof(char) * (strlen(argv[1]) + 1));
    strcpy(reverted_str, argv[1]);

    // Динамически загружаем библиотеку
    void *handle = dlopen("./librevertstring.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Error: %s\n", dlerror());
        return -1;
    }

    // Получаем указатель на функцию из библиотеки
    void (*RevertString)(char *) = dlsym(handle, "RevertString");

    if (!RevertString) {
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(handle);
        return -1;
    }

    // Вызываем функцию из динамически загруженной библиотеки
    RevertString(reverted_str);

    printf("Reverted (Dynamic): %s\n", reverted_str);
    free(reverted_str);
    dlclose(handle);
    return 0;
}
