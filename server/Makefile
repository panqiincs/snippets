CC=gcc

client:
	$(CC) -Wall client.c       -o client.out
	$(CC) -Wall server_iter.c  -o server_iter.out
	$(CC) -Wall server_fork.c  -o server_fork.out
	$(CC) -Wall server_epoll.c -o server_epoll.out
clean:
	rm *.out
