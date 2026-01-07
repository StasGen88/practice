#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#define FTOK_PATH "/tmp"
#define FTOK_PROJ_ID 'S'

#define SERVER_TYPE 10L
#define MAX_TEXT 256

struct chat_msg {
    long mtype;
    int sender;
    char text[MAX_TEXT];
};

int msqid = -1;
int client_priority = 0;
pid_t receiver_pid = -1;

void cleanup_and_exit(int signo) {
    if (msqid != -1 && client_priority != 0) {
        struct chat_msg m;
        m.mtype = SERVER_TYPE;
        m.sender = client_priority;
        strncpy(m.text, "shutdown", MAX_TEXT);
        m.text[MAX_TEXT-1] = '\0';
        msgsnd(msqid, &m, sizeof(m) - sizeof(long), IPC_NOWAIT);
    }
    if (receiver_pid > 0) kill(receiver_pid, SIGTERM);
    printf("\nКлиент %d завершает работу.\n", client_priority);
    exit(0);
}

void receiver_loop(int msqid_local, int priority) {
    struct chat_msg m;
    while (1) {
        ssize_t r = msgrcv(msqid_local, &m, sizeof(m) - sizeof(long), priority, 0);
        if (r == -1) {
            if (errno == EINTR) continue;
            perror("msgrcv (client)");
            break;
        }
        m.text[MAX_TEXT-1] = '\0';
        printf("\n[From %d] %s\n> ", m.sender, m.text);
        fflush(stdout);
    }
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <client_number>\nПример: %s 2   (приоритет 20)\n", argv[0], argv[0]);
        return 1;
    }
    int num = atoi(argv[1]);
    if (num <= 0) {
        fprintf(stderr, "Неверный номер клиента: %s\n", argv[1]);
        return 1;
    }
    client_priority = num * 10;

    key_t key = ftok(FTOK_PATH, FTOK_PROJ_ID);
    if (key == -1) {
        perror("ftok");
        return 1;
    }
    msqid = msgget(key, 0666);
    if (msqid == -1) {
        perror("msgget (проверьте, запущен ли сервер)");
        return 1;
    }

    struct chat_msg reg;
    reg.mtype = SERVER_TYPE;
    reg.sender = client_priority;
    strncpy(reg.text, "connect", MAX_TEXT);
    reg.text[MAX_TEXT-1] = '\0';
    if (msgsnd(msqid, &reg, sizeof(reg) - sizeof(long), 0) == -1) {
        perror("msgsnd (connect)");
    } else {
        printf("Отправлен запрос регистрации на сервер (priority=%d)\n", client_priority);
    }

    signal(SIGINT, cleanup_and_exit);
    signal(SIGTERM, cleanup_and_exit);
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        receiver_loop(msqid, client_priority);
    } else {
        receiver_pid = pid;
        char line[512];
        while (1) {
            printf("> ");
            if (!fgets(line, sizeof(line), stdin)) {
                cleanup_and_exit(0);
            }
            size_t L = strlen(line);
            if (L > 0 && line[L-1] == '\n') line[L-1] = '\0';

            struct chat_msg out;
            out.mtype = SERVER_TYPE;
            out.sender = client_priority;
            strncpy(out.text, line, MAX_TEXT);
            out.text[MAX_TEXT-1] = '\0';

            if (msgsnd(msqid, &out, sizeof(out) - sizeof(long), 0) == -1) {
                perror("msgsnd (to server)");
            }

            if (strcasecmp(out.text, "shutdown") == 0) {
                sleep(1);
                cleanup_and_exit(0);
            }
        }
    }

    return 0;
}
