#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <ctype.h>

#define MAX_BUFFER_LEN 1024
#define DIRPATH "./dataDir/"
#define PORT 8080
#define CONNECTION_STATUS "Connection Accepted"
#define LIST "list"
#define GET "GET"

void * connection_handler(void * socketfd);

int main(int argc, char **argv)
{

    struct sockaddr_in * myaddr = malloc(sizeof(struct sockaddr_in));
    pthread_t thread;

    int socketfd = socket(AF_INET, SOCK_STREAM, 0);

    if (socketfd == -1){
        perror("socketfd failed\n");
        goto cleanup;
    }

    // int sockopt = setsockopt(socketfd,SOL_SOCKET, SO_DEBUG,  )
    myaddr->sin_addr.s_addr = INADDR_ANY;
    myaddr->sin_family = AF_INET;
    myaddr->sin_port = htons(PORT);

    
    if (bind(socketfd, (struct sockaddr *) myaddr ,sizeof(struct sockaddr_in)) < 0){
        perror("bind failed\n");
        goto cleanup;
    }

    if (listen(socketfd, 5) < 0){
        perror("listen failed\n");
        goto cleanup;
    }

    int newSocket;
    struct sockaddr_in * incoming = malloc(sizeof(struct sockaddr_in));
    socklen_t incoming_len = sizeof(struct sockaddr_in);

    while((newSocket = accept(socketfd, (struct sockaddr *) incoming, &incoming_len)) > 0){

        if ((pthread_create(&thread, NULL, connection_handler, &newSocket)) < 0){
            perror("thread failed\n");
            goto cleanup;
        }

    }

cleanup:

    if (socketfd)
        close(socketfd);
    if (newSocket)
        close(newSocket);

    return (0);
}

void * connection_handler(void * socketfd){

    int sockfd = *(int *)socketfd;
    char buffer[MAX_BUFFER_LEN];
    char cmds[] = "$list - see the avaliable files\n$GET filename - get the file with filename\n";
    // int err;

    // send welcome msg
    send(sockfd, CONNECTION_STATUS, MAX_BUFFER_LEN, 0);
    memset(buffer,0,MAX_BUFFER_LEN);
    strncpy(buffer, cmds, strlen(cmds));
    send(sockfd, buffer, MAX_BUFFER_LEN, 0);

     while(( read(sockfd, buffer, MAX_BUFFER_LEN)) > 0){

        int count = 0;
        while(buffer[count] != '\0'){
            buffer[count ] = tolower(buffer[count]);
            count++;
        }

        if (strncmp(buffer,LIST,MAX_BUFFER_LEN) == 0){
            // send a list of file names

        } else if (strncmp(buffer, GET, MAX_BUFFER_LEN) == 0){
            //send a file

        }
     }


    return(0);
}