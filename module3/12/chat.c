#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

#define BUF 1024

int sockfd;

void handler(int s) {
    close(sockfd);
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 3) return 1;

    signal(SIGINT, handler);

    int local_port = atoi(argv[1]);
    int peer_port = atoi(argv[2]);

    struct sockaddr_in local, peer;
    char buf[BUF];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;
    local.sin_port = htons(local_port);

    bind(sockfd, (struct sockaddr *)&local, sizeof(local));

    peer.sin_family = AF_INET;
    peer.sin_addr.s_addr = inet_addr("127.0.0.1");
    peer.sin_port = htons(peer_port);

    if (fork() == 0) {
        while (1) {
            fgets(buf, BUF, stdin);
            sendto(sockfd, buf, strlen(buf), 0,
                   (struct sockaddr *)&peer, sizeof(peer));
        }
    } else {
        socklen_t len = sizeof(peer);
        while (1) {
            int n = recvfrom(sockfd, buf, BUF - 1, 0,
                             (struct sockaddr *)&peer, &len);
            buf[n] = 0;
            printf("peer: %s", buf);
            fflush(stdout);
        }
    }
}
