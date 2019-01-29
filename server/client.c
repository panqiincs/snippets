#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h> 

char buf[100] = "Hello server, are you OK?\n";

void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char **argv) {
    int sockfd, port;
    struct sockaddr_in serveraddr;
    char *host;

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

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

    sleep(5);
    close(sockfd);

    return 0;
}
