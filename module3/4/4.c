#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_CMDS 32
#define MAX_ARGS 64
#define MAX_LEN 1024

void parse_command(char *cmd, char *args[], char **infile, char **outfile, int *append) {
    int i = 0;
    *infile = NULL;
    *outfile = NULL;
    *append = 0;

    char *token = strtok(cmd, " \t");
    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " \t");
            *infile = token;
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " \t");
            *outfile = token;
        } else if (strcmp(token, ">>") == 0) {
            token = strtok(NULL, " \t");
            *outfile = token;
            *append = 1;
        } else {
            args[i++] = token;
        }
        token = strtok(NULL, " \t");
    }
    args[i] = NULL;
}

void execute_pipeline(char *commands[], int cmd_count) {
    int pipes[MAX_CMDS][2];
    char *args[MAX_ARGS];
    char *infile, *outfile;
    int append;

    for (int i = 0; i < cmd_count - 1; i++) {
        pipe(pipes[i]);
    }

    for (int i = 0; i < cmd_count; i++) {
        parse_command(commands[i], args, &infile, &outfile, &append);

        pid_t pid = fork();
        if (pid == 0) {
            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            if (i < cmd_count - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            if (infile != NULL) {
                int fd = open(infile, O_RDONLY);
                if (fd < 0) { perror("open infile"); exit(1); }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }

            if (outfile != NULL) {
                int fd;
                if (append)
                    fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
                else
                    fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);

                if (fd < 0) { perror("open outfile"); exit(1); }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            for (int j = 0; j < cmd_count - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            execvp(args[0], args);
            perror("execvp");
            exit(1);
        }
    }

    for (int j = 0; j < cmd_count - 1; j++) {
        close(pipes[j][0]);
        close(pipes[j][1]);
    }

    for (int i = 0; i < cmd_count; i++) {
        wait(NULL);
    }
}

int main() {
    char input[MAX_LEN];
    char *commands[MAX_CMDS];

    printf("Добро пожаловать!\nВведите 'exit' для выхода.\n\n");

    while (1) {
        printf("mybash> ");
        fflush(stdout);

        if (!fgets(input, sizeof(input), stdin))
            break;

        if (strcmp(input, "exit\n") == 0)
            break;

        input[strcspn(input, "\n")] = 0;

        if (strlen(input) == 0)
            continue;

        int cmd_count = 0;
        char *cmd = strtok(input, "|");
        while (cmd != NULL && cmd_count < MAX_CMDS) {
            commands[cmd_count++] = cmd;
            cmd = strtok(NULL, "|");
        }

        execute_pipeline(commands, cmd_count);
    }

    return 0;
}
