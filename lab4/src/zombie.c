#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <getopt.h>

int main(int argc, char **argv) {
    int opt;
    int create_zombie = 0;

    while ((opt = getopt(argc, argv, "z")) != -1) {
        switch (opt) {
            case 'z':
                create_zombie = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-z]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    pid_t child_pid = fork();

    if (child_pid == 0) {
        // Это код дочернего процесса
        printf("Child process is running.\n");
        if (create_zombie) {
            printf("Child process is a zombie.\n");
            sleep(10); // Зомби-процесс будет ждать 10 секунд
        }
        exit(0);
    } else if (child_pid > 0) {
        // Это код родительского процесса
        if (!create_zombie) {
            wait(NULL); // Родительский процесс ждет завершения дочернего процесса
        }
        printf("Parent process is exiting.\n");
    } else {
        perror("Fork failed");
        exit(1);
    }

    return 0;
}
