#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define MAX_BUFFER_LEN 1024
#define DIRPATH "./dataDir/"
#define PORT 8080
#define CONNECTION_STATUS "Connection Accepted"

int main(int argc, char ** argv){

    struct sockaddr_in * address = malloc(sizeof(struct sockaddr_in));
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    char buffer[MAX_BUFFER_LEN];

    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_family = AF_INET;
    address->sin_port = htons(PORT);

    if (sockfd < 0){
        perror("socket failed\n");
        goto cleanup;
    }

    if (connect(sockfd, (struct sockaddr *) address, (socklen_t) sizeof(struct sockaddr_in)) < 0 )
    {
        perror("client connected failed\n");
        goto cleanup;
    }

    //get connection status
    int readval = read(sockfd, buffer, MAX_BUFFER_LEN);

    if (readval > 0 && (strncmp(buffer, CONNECTION_STATUS, readval) == 0) ){
        memset(buffer,0,MAX_BUFFER_LEN);    
        while(( readval = read(sockfd, buffer, MAX_BUFFER_LEN)) > 0){
            printf("%s\n>", buffer);
            memset(buffer,0,MAX_BUFFER_LEN); 
            scanf("%s",buffer);
            break;
        }
    }

    cleanup:

    if (sockfd)
        close(sockfd);
    // if (newSocket)
    //     close(newSocket);

    return(0);
}