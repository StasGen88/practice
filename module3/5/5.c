#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

FILE *fp;
int counter = 0;
int sigint_count = 0;

void handler(int sig) {

    if (sig == SIGINT) {
        sigint_count++;
        fprintf(fp, "[SIGNAL] Caught SIGINT (%d time)\n", sigint_count);
        fflush(fp);

        if (sigint_count >= 3) {
            fprintf(fp, "[EXIT] Received 3 SIGINT → exiting program.\n");
            fflush(fp);
            fclose(fp);
            exit(0);
        }
    }
    else if (sig == SIGQUIT) {
        fprintf(fp, "[SIGNAL] Caught SIGQUIT\n");
        fflush(fp);
    }
}

int main() {
    fp = fopen("log.txt", "a");
    if (!fp) {
        perror("fopen");
        exit(1);
    }

    signal(SIGINT, handler);
    signal(SIGQUIT, handler);

    fprintf(fp, "[START] Program started.\n");
    fflush(fp);

    while (1) {
        sleep(1);
        counter++;
        fprintf(fp, "%d\n", counter);
        fflush(fp);
    }

    fclose(fp);
    return 0;
}
