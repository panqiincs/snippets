#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h> 

#define PORT_NUM   40713
#define MAX_EVENTS 1024

void error(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void set_nonblocking(int fd)
{
    int flags;
    if ((flags = fcntl(fd, F_GETFL, NULL)) < 0) {
        error("ERROR fcntl F_GETFL");
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        error("ERROR fcntl F_SETFL");
    }
}

void do_echo(int fd) {
    char c, buf[100];
    int buf_cnt = 0;
    for (;;) {
        int n = read(fd, &c, 1);
        if (n < 0) {
            if (errno == EAGAIN) {
                break;
            }
            error("ERROR read from client");
        } else if (n == 0) {
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

    int epfd;
    struct epoll_event event;
    struct epoll_event *evlist;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT_NUM);

    if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("ERROR socket");
    }
    set_nonblocking(lfd);
    if (bind(lfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error("ERROR bind");
    }
    if (listen(lfd, 16) < 0) {
        error("ERROR listen");
    }

    epfd = epoll_create1(0);
    if (epfd < 0) {
        perror("ERROR epoll_create1");
    }
    event.events = EPOLLIN;
    event.data.fd = lfd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &event) < 0) {
        error("ERROR epoll_ctl");
    }
    evlist = malloc(sizeof(struct epoll_event)*MAX_EVENTS);

    for (;;) {
        int cfd;
        socklen_t len = sizeof(client_addr);
        int ready = epoll_wait(epfd, evlist, MAX_EVENTS, -1);
        if (ready == -1) {
            error("ERROR epoll_wait");
        }

        printf("Ready: %d\n", ready);
        for (int i = 0; i < ready; i++) {
            printf("  fd = %d; events: %s%s%s\n", evlist[i].data.fd,
                   (evlist[i].events & EPOLLIN)  ? "EPOLLIN "  : "",
                   (evlist[i].events & EPOLLHUP) ? "EPOLLHUP " : "",
                   (evlist[i].events & EPOLLERR) ? "EPOLLERR " : "");
        }

        for (int i = 0; i < ready; i++) {
            if (evlist[i].data.fd == lfd) {
                cfd = accept(lfd, (struct sockaddr *)&client_addr, &len);
                if (cfd < 0) {
                    perror("ERROR accept");
                    continue;
                }
                printf("    accepting fd %d\n", cfd);
                set_nonblocking(cfd);
                event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
                event.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &event);
            } else if (evlist[i].events & EPOLLIN) {
                cfd = evlist[i].data.fd;
                printf("    reading from fd %d\n", cfd);
                do_echo(cfd);
            } else if (evlist[i].events & (EPOLLHUP | EPOLLERR)) {
                printf("    closing fd %d\n", evlist[i].data.fd);
                if (close(evlist[i].data.fd) == -1) {
                    error("ERROR close");
                }
            }
        }
    }

    close(lfd);
    exit(EXIT_SUCCESS);
}
