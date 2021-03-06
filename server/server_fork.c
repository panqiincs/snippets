#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h> 

#define PORT_NUM 40713

void error(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void do_echo(int fd) {
    char c, buf[100];
    int cnt = 0;
    for (;;) {
        int n = read(fd, &c, 1);
        if (n < 0) {
            error("ERROR read from client");
        } else if (n == 0) {
            break;
        }
        buf[cnt++] = c;
        if (c == '\n') {
            if (write(fd, buf, cnt) < 0) {
                error("ERROR write to client");
            }
            cnt = 0;
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
    server_addr.sin_port = htons(PORT_NUM);

    if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("ERROR socket");
    }
    if (bind(lfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error("ERROR bind");
    }
    if (listen(lfd, 16) < 0) {
        error("ERROR listen");
    }

    for (;;) {
        socklen_t len = sizeof(client_addr);
        int cfd = accept(lfd, (struct sockaddr *)&client_addr, &len);
        if (cfd < 0) {
            error("ERROR accept");
        }

        switch(fork()) {
        case -1:
            perror("fork");
            close(cfd);
            break;
        case 0:
            close(lfd);
            do_echo(cfd);
            _exit(EXIT_SUCCESS);
        default:
            close(cfd);
            while (wait(NULL) != -1) {
                continue;
            }
            if (errno != ECHILD) {
                error("ERROR wait");
            }
        }
    }

    close(lfd);
    exit(EXIT_SUCCESS);
}
