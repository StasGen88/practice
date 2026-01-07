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
#define MAX_CLIENTS 128

struct chat_msg {
    long mtype;         
    int sender;
    char text[MAX_TEXT];
};

int msqid = -1;

typedef struct {
    int priority;  
    int active;    
} client_rec;

client_rec clients[MAX_CLIENTS];
int clients_count = 0;

void cleanup(int signo) {
    if (msqid != -1) {
        printf("\nУдаляем очередь сообщений (msgid=%d)\n", msqid);
        msgctl(msqid, IPC_RMID, NULL);
    }
    exit(0);
}

void add_or_activate_client(int priority) {
    for (int i = 0; i < clients_count; ++i) {
        if (clients[i].priority == priority) {
            if (!clients[i].active) {
                clients[i].active = 1;
                printf("Клиент %d повторно активирован.\n", priority);
            }
            return;
        }
    }
    if (clients_count < MAX_CLIENTS) {
        clients[clients_count].priority = priority;
        clients[clients_count].active = 1;
        clients_count++;
        printf("Добавлен новый клиент %d.\n", priority);
    } else {
        fprintf(stderr, "Достигнут максимум клиентов, не могу добавить %d\n", priority);
    }
}

void deactivate_client(int priority) {
    for (int i = 0; i < clients_count; ++i) {
        if (clients[i].priority == priority) {
            clients[i].active = 0;
            printf("Клиент %d отключён (shutdown).\n", priority);
            return;
        }
    }

    if (clients_count < MAX_CLIENTS) {
        clients[clients_count].priority = priority;
        clients[clients_count].active = 0;
        clients_count++;
        printf("Клиент %d добавлен в список как отключённый.\n", priority);
    }
}

int main(void) {
    key_t key = ftok(FTOK_PATH, FTOK_PROJ_ID);
    if (key == -1) {
        perror("ftok");
        return 1;
    }

    msqid = msgget(key, IPC_CREAT | 0666);
    if (msqid == -1) {
        perror("msgget");
        return 1;
    }

    printf("Сервер запущен. msgid=%d\n", msqid);
    signal(SIGINT, cleanup);
    signal(SIGTERM, cleanup);

    struct chat_msg inmsg;
    ssize_t recvlen;
    while (1) {
        recvlen = msgrcv(msqid, &inmsg, sizeof(inmsg) - sizeof(long), SERVER_TYPE, 0);
        if (recvlen == -1) {
            if (errno == EINTR) continue;
            perror("msgrcv");
            break;
        }
        inmsg.text[MAX_TEXT-1] = '\0';
        int sender = inmsg.sender;
        printf("Получено от %d: %s\n", sender, inmsg.text);

        if (strcasecmp(inmsg.text, "connect") == 0 || strcasecmp(inmsg.text, "register") == 0) {
            add_or_activate_client(sender);
            continue;
        }
        if (strcasecmp(inmsg.text, "shutdown") == 0 || strcasecmp(inmsg.text, "disconnect") == 0) {
            deactivate_client(sender);
            continue;
        }

        for (int i = 0; i < clients_count; ++i) {
            if (!clients[i].active) continue;
            if (clients[i].priority == sender) continue;
            struct chat_msg outmsg;
            outmsg.mtype = clients[i].priority;
            outmsg.sender = sender;
            strncpy(outmsg.text, inmsg.text, MAX_TEXT);
            outmsg.text[MAX_TEXT-1] = '\0';

            if (msgsnd(msqid, &outmsg, sizeof(outmsg) - sizeof(long), 0) == -1) {
                perror("msgsnd (forward)");
            } else {
                printf("Переслано клиенту %d: от %d -> \"%s\"\n", clients[i].priority, sender, outmsg.text);
            }
        }
    }

    cleanup(0);
    return 0;
}
