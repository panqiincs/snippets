#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h> 

void error(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void do_echo(int fd) {
    char c, buf[100];
    int buf_cnt = 0;
    for (;;) {
        int n = read(fd, &c, 1);
        if (n < 0) {
            error("ERROR read from client");
        }
        else if (n == 0) {
            break;
        }
        buf[buf_cnt++] = c;
        if (c == '\n') {
            if (write(fd, buf, buf_cnt) < 0) {
                error("ERROR write to client");
            }
            buf_cnt = 0;
            continue;
        }
    }
}

int main(int argc, char **argv) {
    int lfd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(40713);

    if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("ERROR socket");
    }
    if (bind(lfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error("ERROR bind");
    }
    if (listen(lfd, 5) < 0) {
        error("ERROR listen");
    }

    for (;;) {
        socklen_t len = sizeof(client_addr);
        int cfd = accept(lfd, (struct sockaddr *)&client_addr, &len);
        if (cfd < 0) {
            error("ERROR accept");
        }
        do_echo(cfd);
    }

    close(lfd);
    exit(EXIT_SUCCESS);
}
