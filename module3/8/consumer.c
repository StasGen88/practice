#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <limits.h>

union semun {
    int val;
};

int sem_lock(int semid) {
    struct sembuf sb = {0, -1, 0};
    return semop(semid, &sb, 1);
}

int sem_unlock(int semid) {
    struct sembuf sb = {0, 1, 0};
    return semop(semid, &sb, 1);
}

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;

    const char *filename = argv[1];
    key_t key = ftok(filename, 'S');
    int semid = semget(key, 1, 0666);

    if (semid == -1) return 1;

    while (1) {
        sem_lock(semid);

        FILE *f = fopen(filename, "r+");
        if (!f) {
            sem_unlock(semid);
            return 1;
        }

        char line[1024];
        if (!fgets(line, sizeof(line), f)) {
            fclose(f);
            sem_unlock(semid);
            sleep(1);
            continue;
        }

        long pos = ftell(f);
        FILE *tmp = tmpfile();
        char rest[1024];
        while (fgets(rest, sizeof(rest), f)) {
            fputs(rest, tmp);
        }

        rewind(f);
        rewind(tmp);
        while (fgets(rest, sizeof(rest), tmp)) {
            fputs(rest, f);
        }
        ftruncate(fileno(f), ftell(f));

        fclose(tmp);
        fclose(f);

        sem_unlock(semid);

        int min = INT_MAX, max = INT_MIN, x;
        char *p = line;
        while (sscanf(p, "%d", &x) == 1) {
            if (x < min) min = x;
            if (x > max) max = x;
            while (*p && *p != ' ') p++;
            if (*p) p++;
        }

        if (min != INT_MAX)
            printf("min=%d max=%d\n", min, max);
    }
}
