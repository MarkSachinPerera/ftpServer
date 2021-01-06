all:
	gcc -Wall -pthread -g -o runserver server.c
clean:
	rm *.o