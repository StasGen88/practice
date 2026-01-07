#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define MQ_A_TO_B "/mq_a_to_b"
#define MQ_B_TO_A "/mq_b_to_a"

#define MAX_TEXT 256
#define PRIO_NORMAL 1
#define PRIO_EXIT   99

int main() {
    mqd_t mq_ab = mq_open(MQ_A_TO_B, O_RDONLY);
    mqd_t mq_ba = mq_open(MQ_B_TO_A, O_WRONLY);

    if (mq_ab == -1 || mq_ba == -1) {
        perror("mq_open");
        exit(1);
    }

    char buf[MAX_TEXT];
    unsigned prio;

    while (1) {
        mq_receive(mq_ab, buf, sizeof(buf), &prio);

        if (prio == PRIO_EXIT) {
            mq_send(mq_ba, "exit", 5, PRIO_EXIT);
            break;
        }

        printf("A: %s\n", buf);

        printf("B> ");
        fflush(stdout);

        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = 0;

        unsigned send_prio = PRIO_NORMAL;
        if (strcmp(buf, "exit") == 0)
            send_prio = PRIO_EXIT;

        mq_send(mq_ba, buf, strlen(buf) + 1, send_prio);

        if (send_prio == PRIO_EXIT)
            break;
    }

    mq_close(mq_ab);
    mq_close(mq_ba);

    return 0;
}
