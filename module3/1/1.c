#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

int is_number(const char *str) {
    char *endptr;
    strtod(str, &endptr);
    
    if (*endptr == '\0' && endptr != str) {
        return 1;
    } else {
        return 0;
    }
}

double get_number_value(const char *str) {
    return atof(str);
}

void process_arguments(char *argv[], int start, int end) {
    for (int i = start; i < end; i++) {
        if (is_number(argv[i])) {
            double value = get_number_value(argv[i]);
            printf("%s -> %.2f\n", argv[i], value * 2);
        } else {
            printf("%s -> %s\n", argv[i], argv[i]);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Использование: %s [аргументы...]\n", argv[0]);
        return 1;
    }

    int mid = argc / 2;
    
    pid_t pid = fork();
    
    if (pid == -1) {
        perror("Ошибка при создании процесса");
        return 1;
    }
    
    if (pid == 0) {
        printf("=== Дочерний процесс (PID: %d) ===\n", getpid());
        process_arguments(argv, mid, argc);
        printf("=== Дочерний процесс завершен ===\n");
    } else {
        printf("=== Родительский процесс (PID: %d) ===\n", getpid());
        process_arguments(argv, 1, mid);

        wait(NULL);
        printf("=== Родительский процесс завершен ===\n");
    }
    
    return 0;
}