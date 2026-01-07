#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#define MAX 100

volatile sig_atomic_t stop = 0;

struct data {
    int count;
    int nums[MAX];
    int min;
    int max;
    int ready;
};

void handler(int s) {
    stop = 1;
}

int main() {
    key_t key = IPC_PRIVATE;
    int shmid = shmget(key, sizeof(struct data), IPC_CREAT | 0666);
    struct data *d = shmat(shmid, NULL, 0);

    signal(SIGINT, handler);
    srand(time(NULL));

    pid_t pid = fork();
    int sets = 0;

    if (pid == 0) {
        while (!stop) {
            if (d->ready == 1) {
                d->min = d->nums[0];
                d->max = d->nums[0];
                for (int i = 1; i < d->count; i++) {
                    if (d->nums[i] < d->min) d->min = d->nums[i];
                    if (d->nums[i] > d->max) d->max = d->nums[i];
                }
                d->ready = 2;
            }
        }
        shmdt(d);
        exit(0);
    } else {
        while (!stop) {
            d->count = rand() % MAX + 1;
            for (int i = 0; i < d->count; i++)
                d->nums[i] = rand() % 1000 - 500;

            d->ready = 1;
            while (d->ready != 2 && !stop);

            if (stop) break;

            printf("min=%d max=%d\n", d->min, d->max);
            sets++;
            d->ready = 0;
            sleep(1);
        }

        kill(pid, SIGINT);
        wait(NULL);
        printf("processed=%d\n", sets);
        shmdt(d);
        shmctl(shmid, IPC_RMID, NULL);
    }
    return 0;
}
