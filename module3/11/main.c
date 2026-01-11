#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>

#define MAX 100
#define SHM_NAME "/posix_shm_example"

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
    signal(SIGINT, handler);
    srand(time(NULL));

    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof(struct data));

    struct data *d = mmap(NULL, sizeof(struct data),
                          PROT_READ | PROT_WRITE,
                          MAP_SHARED, fd, 0);

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
        munmap(d, sizeof(struct data));
        close(fd);
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
        munmap(d, sizeof(struct data));
        close(fd);
        shm_unlink(SHM_NAME);
    }
    return 0;
}
