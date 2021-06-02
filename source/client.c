/**
 * Author: Mark Perera
 * File Name: client.c
 * 
 * This is the client end for the ftpserver.
 * 
 */

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
// #define DIRPATH "./dataDir/"
#define PORT 7000
#define CONNECTION_STATUS "Connection Accepted"
#define LIST "list"
#define GET "get"

int main(int argc, char **argv)
{

    struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in));
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    char buffer[MAX_BUFFER_LEN];
    char *line = NULL;
    size_t line_len = 0;

    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_family = AF_INET;
    address->sin_port = htons(PORT);

    if (sockfd < 0)
    {
        perror("socket failed\n");
        goto cleanup;
    }

    if (connect(sockfd, (struct sockaddr *)address, (socklen_t)sizeof(struct sockaddr_in)) < 0)
    {
        perror("client connected failed\n");
        goto cleanup;
    }

    //get connection status
    int readval = read(sockfd, buffer, MAX_BUFFER_LEN);

    if (readval > 0 && (strncmp(buffer, CONNECTION_STATUS, readval) == 0))
    {
        memset(buffer, 0, MAX_BUFFER_LEN);
        while ((readval = read(sockfd, buffer, MAX_BUFFER_LEN)) > 0)
        {

            // start by printing anything the client has sent
            // this is like confirmation that the connection is working
            printf("%s\n", buffer);
            memset(buffer, 0, MAX_BUFFER_LEN);

        //User input and lowercase it
        RETRY:
            printf(">");
            scanf("%s", buffer);

            strncpy(buffer, line, line_len);
            buffer[line_len] = '\0';

            int count = 0;
            while (buffer[count] != '\0')
            {
                buffer[count] = tolower(buffer[count]);
                count++;
            }

            // printf("\n%s\n", buffer);
            if (0 == strncmp(buffer, LIST, strlen(buffer)))
                send(sockfd, buffer, MAX_BUFFER_LEN, 0);
            else if (0 == strncmp(buffer, GET, strlen(buffer)))
            {
                scanf("\n$GET FileName>%s", buffer);

                if (strlen(buffer) > 0)
                {

                    send(sockfd, GET, strlen(GET), 0);
                    sleep(1);
                    send(sockfd, buffer, MAX_BUFFER_LEN, 0);
                }
                else
                {
                    printf("Error: missing file name\n");
                    goto RETRY;
                }
                /**
                 * Sending the GET command and the file name separately. 
                 * Its extra work to get everything to happen in sync.
                 * 
                */
            }
            else
            {
                printf("Error: invalid command\n");
                goto RETRY;
            }
        }
    }

cleanup:

    if (sockfd)
        close(sockfd);
    // if (newSocket)
    //     close(newSocket);

    return (0);
}