all:
	gcc -Wall -pthread -g -o build/runserver server.c
	gcc -Wall -pthread -g -o build/runclient client.c
clean:
	rm *.o build/runclient build/runserver