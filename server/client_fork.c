#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h> 

void error(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    int port;
    char *host;
    char buf[100];
    struct sockaddr_in serveraddr;

    if (argc != 3) {
        fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
        exit(0);
    }
    host = argv[1];
    port = atoi(argv[2]);

    memset(&serveraddr, 0, sizeof(serveraddr));
    if (inet_pton(AF_INET, host, &serveraddr.sin_addr) <= 0) {
        error("ERROR inet_pthon");
    }
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);

    for (int cnt = 1; cnt <= 5; cnt++) {
        switch (fork()) {
        case -1:
            error("ERROR fork");
            break;
        case 0:
            snprintf(buf, 100, "I am client %d\n", cnt);
            int sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) {
                error("ERROR opening socket");
            }
            if (connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
                error("ERROR connecting");
            }
            if (write(sockfd, buf, strlen(buf)) < 0) {
                error("ERROR writing to socket");
            }
            if (read(sockfd, buf, sizeof(buf)) < 0) {
                error("ERROR reading from socket");
            }
            printf("Echo from server: %s", buf);
            close(sockfd);
            _exit(EXIT_SUCCESS);
        default:
            while (wait(NULL) != -1) {
                continue;
            }
            if (errno != ECHILD) {
                error("ERROR wait");
            }
        }
    }
    
    exit(EXIT_SUCCESS);
}
