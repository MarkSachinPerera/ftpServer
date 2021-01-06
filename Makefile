all:
	gcc -Wall -pthread -g -o runserver server.c
	gcc -Wall -pthread -g -o runclient client.c
clean:
	rm *.o runclient runserver