
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
    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_maxmsg = 10,
        .mq_msgsize = MAX_TEXT,
        .mq_curmsgs = 0
    };

    mq_unlink(MQ_A_TO_B);
    mq_unlink(MQ_B_TO_A);

    mqd_t mq_ab = mq_open(MQ_A_TO_B, O_CREAT | O_WRONLY, 0666, &attr);
    mqd_t mq_ba = mq_open(MQ_B_TO_A, O_CREAT | O_RDONLY, 0666, &attr);

    if (mq_ab == -1 || mq_ba == -1) {
        perror("mq_open");
        exit(1);
    }

    char buf[MAX_TEXT];
    unsigned prio;

    while (1) {
        printf("A> ");
        fflush(stdout);

        if (!fgets(buf, sizeof(buf), stdin))
            break;

        buf[strcspn(buf, "\n")] = 0;

        unsigned send_prio = PRIO_NORMAL;
        if (strcmp(buf, "exit") == 0)
            send_prio = PRIO_EXIT;

        mq_send(mq_ab, buf, strlen(buf) + 1, send_prio);

        if (send_prio == PRIO_EXIT)
            break;

        mq_receive(mq_ba, buf, sizeof(buf), &prio);

        if (prio == PRIO_EXIT) {
            printf("B завершил чат\n");
            break;
        }

        printf("B: %s\n", buf);
    }

    mq_close(mq_ab);
    mq_close(mq_ba);
    mq_unlink(MQ_A_TO_B);
    mq_unlink(MQ_B_TO_A);

    return 0;
}
