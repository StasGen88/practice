#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>

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
    int semid = semget(key, 1, IPC_CREAT | 0666);

    union semun arg;
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);

    srand(time(NULL) ^ getpid());

    while (1) {
        sem_lock(semid);

        FILE *f = fopen(filename, "a");
        if (!f) {
            sem_unlock(semid);
            return 1;
        }

        int count = rand() % 10 + 1;
        for (int i = 0; i < count; i++) {
            fprintf(f, "%d ", rand() % 1000);
        }
        fprintf(f, "\n");
        fclose(f);

        sem_unlock(semid);
        sleep(1);
    }
}
