// controller.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

static volatile int lines_processed = 0;

void sigusr1_handler(int signo) {
    fprintf(stderr, "Processed %d lines\n", lines_processed);
}

int main() {
    int pipeA[2], pipeB[2], pipeC[2];

    if (pipe(pipeA) == -1 || pipe(pipeB) == -1 || pipe(pipeC) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    signal(SIGUSR1, sigusr1_handler);

    pid_t pid1 = fork();
    if (pid1 == 0) {
        // Child P1: generator
        close(pipeA[0]);
        dup2(pipeA[1], STDOUT_FILENO);
        close(pipeA[1]);
        close(pipeB[0]); close(pipeB[1]);
        close(pipeC[0]); close(pipeC[1]);

        char N[4];
        snprintf(N, sizeof(N), "%d", rand() % 61 + 120); // Random N between 120 and 180
        execl("./generator", "./generator", N, NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Child P2: bc
        close(pipeB[1]);
        dup2(pipeB[0], STDIN_FILENO);
        close(pipeB[0]);

        close(pipeC[0]);
        dup2(pipeC[1], STDOUT_FILENO);
        close(pipeC[1]);

        close(pipeA[0]); close(pipeA[1]);

        execl("/usr/bin/bc", "/usr/bin/bc", NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    }

    // Parent P0
    close(pipeA[1]);
    close(pipeB[0]);
    close(pipeC[1]);

    char expression[256];
    char result[256];

    while (read(pipeA[0], expression, sizeof(expression)) > 0) {
        write(pipeB[1], expression, strlen(expression));
        write(pipeB[1], "\n", 1);

        int bytes_read = read(pipeC[0], result, sizeof(result));
        if (bytes_read > 0) {
            result[bytes_read] = '\0';
            printf("%s = %s", expression, result);
            lines_processed++;
        }
    }

    close(pipeA[0]);
    close(pipeB[1]);
    close(pipeC[0]);

    wait(NULL); // Wait for P1
    wait(NULL); // Wait for P2

    return 0;
}
