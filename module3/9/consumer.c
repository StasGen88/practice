#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>

#define FILE_NAME "data.txt"
#define SEM_NAME "/pc_sem"

int main()
{
    sem_t *sem = sem_open(SEM_NAME, 0);
    long pos = 0;

    while (1) {
        sem_wait(sem);
        FILE *f = fopen(FILE_NAME, "r");
        if (!f) {
            sem_post(sem);
            break;
        }

        fseek(f, pos, SEEK_SET);
        char line[1024];
        if (!fgets(line, sizeof(line), f)) {
            fclose(f);
            sem_post(sem);
            break;
        }

        int min, max, x;
        char *p = line;
        sscanf(p, "%d", &min);
        max = min;

        while (sscanf(p, "%d", &x) == 1) {
            if (x < min) min = x;
            if (x > max) max = x;
            while (*p && *p != ' ') p++;
            while (*p == ' ') p++;
        }

        printf("pid %d: min=%d max=%d\n", getpid(), min, max);
        pos = ftell(f);

        fclose(f);
        sem_post(sem);
        usleep(150000);
    }

    sem_close(sem);
    return 0;
}
