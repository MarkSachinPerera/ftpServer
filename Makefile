CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -pthread



all:
	$(CC) $(CFLAGS) $(LDFLAGS) -o build/runserver source/server.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o build/runclient source/client.c
clean:
	rm *.o build/runclient build/runserver