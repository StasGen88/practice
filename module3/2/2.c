#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_LENGHT 1024
#define MAX_ARGS 64

void print_promt() {
    printf("mybash> ");
    fflush(stdout);
}

void execute_command(char *argv[]) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork faild");
        return;
    }
    if (pid == 0) {
        
        execvp(argv[0], argv);
        perror("exec failed");
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}

void parse_input(char *input, char *args[]) {
    int i = 0;
    char *token = strtok(input, " \t\n");
    while (token != NULL && i < MAX_ARGS-1)
    {
        args[i] = token;
        i++;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
}

int main() {
    char input[MAX_INPUT_LENGHT];
    char *args[MAX_ARGS];
    
    printf("Добро пожаловать!\n");
    printf("Введите 'exit' для выхода\n\n");

    while (1) {
        print_promt();
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        };

        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0) {
            break;
        }

        parse_input(input, args);
        if (args[0] == NULL) {
            continue;
        }

        execute_command(args);
    }
    
}