#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    // Создаем дочерний процесс
    pid_t child_pid = fork();
    
    if (child_pid == -1) {
        perror("Fork failed");
        exit(1);
    } else if (child_pid == 0) {
        // Этот код выполняется в дочернем процессе
        // Здесь мы вызываем exec для запуска sequential_min_max
       execlp("./sequential_min_max", "./sequential_min_max", "42", "10000", NULL);

        // Если выполнение дошло сюда, значит произошла ошибка
        perror("Exec failed");
        exit(1);
    } else {
        // Этот код выполняется в родительском процессе
        // Мы ожидаем завершение дочернего процесса
        int status;
        wait(&status);
        
        if (WIFEXITED(status)) {
            // Дочерний процесс успешно завершился
            printf("Child process exited with status: %d\n", WEXITSTATUS(status));
        } else {
            // Дочерний процесс завершился с ошибкой
            printf("Child process exited with an error.\n");
        }
    }

    return 0;
}
