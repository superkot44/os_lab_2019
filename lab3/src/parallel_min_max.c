#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

int main(int argc, char **argv) {
    int seed = -1;
    int array_size = -1;
    int pnum = -1;
    bool with_files = false;

    while (true) {
        int current_optind = optind ? optind : 1;

        static struct option options[] = {
            {"seed", required_argument, 0, 0},
            {"array_size", required_argument, 0, 0},
            {"pnum", required_argument, 0, 0},
            {"by_files", no_argument, 0, 'f'},
            {0, 0, 0, 0}
        };

        int option_index = 0;
        int c = getopt_long(argc, argv, "f", options, &option_index);

        if (c == -1) break;

        switch (c) {
            case 0:
                switch (option_index) {
                    case 0:
                        seed = atoi(optarg);
                        break;
                    case 1:
                        array_size = atoi(optarg);
                        break;
                    case 2:
                        pnum = atoi(optarg);
                        break;
                    case 3:
                        with_files = true;
                        break;
                    default:
                        printf("Index %d is out of options\n", option_index);
                }
                break;
            case 'f':
                with_files = true;
                break;
            case '?':
                break;
            default:
                printf("getopt returned character code 0%o?\n", c);
        }
    }

    if (optind < argc) {
        printf("Has at least one no option argument\n");
        return 1;
    }

    if (seed == -1 || array_size == -1 || pnum == -1) {
        printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" [--by_files]\n", argv[0]);
        return 1;
    }

    int *array = malloc(sizeof(int) * array_size);
    GenerateArray(array, array_size, seed);

    int active_child_processes = 0;

    struct timeval start_time;
    gettimeofday(&start_time, NULL);

    int pipefd[pnum][2];

    for (int i = 0; i < pnum; i++) {
        if (pipe(pipefd[i]) == -1) {
            perror("Pipe creation failed");
            exit(1);
        }

        pid_t child_pid = fork();
        if (child_pid >= 0) {
            active_child_processes += 1;
            if (child_pid == 0) {
                struct MinMax local_min_max = GetMinMax(array, i * (array_size / pnum), (i == pnum - 1) ? array_size : (i + 1) * (array_size / pnum));

                if (with_files) {
                    char min_filename[100];
                    char max_filename[100];
                    snprintf(min_filename, sizeof(min_filename), "min_%d.txt", i);
                    snprintf(max_filename, sizeof(max_filename), "max_%d.txt", i);

                    FILE *min_file = fopen(min_filename, "w");
                    FILE *max_file = fopen(max_filename, "w");

                    fprintf(min_file, "%d\n", local_min_max.min);
                    fprintf(max_file, "%d\n", local_min_max.max);

                    fclose(min_file);
                    fclose(max_file);
                } else {
                    write(pipefd[i][1], &local_min_max.min, sizeof(int));
                    write(pipefd[i][1], &local_min_max.max, sizeof(int));
                }

                close(pipefd[i][1]);
                return 0;
            }
        } else {
            printf("Fork failed!\n");
            return 1;
        }
    }

    while (active_child_processes > 0) {
        int status;
        wait(&status);
        active_child_processes -= 1;
    }

    struct MinMax min_max;
    min_max.min = INT_MAX;
    min_max.max = INT_MIN;

    for (int i = 0; i < pnum; i++) {
        int local_min, local_max;

        if (with_files) {
            char min_filename[100];
            char max_filename[100];
            snprintf(min_filename, sizeof(min_filename), "min_%d.txt", i);
            snprintf(max_filename, sizeof(max_filename), "max_%d.txt", i);

            FILE *min_file = fopen(min_filename, "r");
            FILE *max_file = fopen(max_filename, "r");

            fscanf(min_file, "%d", &local_min);
            fscanf(max_file, "%d", &local_max);

            fclose(min_file);
            fclose(max_file);
        } else {
            read(pipefd[i][0], &local_min, sizeof(int));
            read(pipefd[i][0], &local_max, sizeof(int));

            close(pipefd[i][0]);
        }

        if (local_min < min_max.min) min_max.min = local_min;
        if (local_max > min_max.max) min_max.max = local_max;
    }

    struct timeval finish_time;
    gettimeofday(&finish_time, NULL);

    double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
    elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

    free(array);

    printf("Min: %d\n", min_max.min);
    printf("Max: %d\n", min_max.max);
    printf("Elapsed time: %fms\n", elapsed_time);
    fflush(NULL);
    return 0;
}
