all:
	gcc -Wall -pthread -g -o server server.c
clean:
	rm *.o