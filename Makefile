all:
	gcc -Wall -pthread -g -o build/runserver source/server.c
	gcc -Wall -pthread -g -o build/runclient source/client.c
clean:
	rm *.o build/runclient build/runserver