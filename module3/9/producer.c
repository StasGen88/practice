#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>

#define FILE_NAME "data.txt"
#define SEM_NAME "/pc_sem"

int main()
{
    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    FILE *f = fopen(FILE_NAME, "a");
    srand(time(NULL));

    while (1) {
        sem_wait(sem);
        int n = rand() % 10 + 1;
        for (int j = 0; j < n; j++)
            fprintf(f, "%d ", rand() % 100);
        fprintf(f, "\n");
        fflush(f);
        sem_post(sem);
        usleep(100000);
    }

    fclose(f);
    sem_close(sem);
    return 0;
}
