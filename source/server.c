/**
 * Author: Mark Perera
 * File Name: Server.c
 * 
 * This is the server end for the ftpserver.
 * 
 * Resources:
 * https://stackoverflow.com/questions/11736060/how-to-read-all-files-in-a-folder-using-c
 * https://stackoverflow.com/questions/5106674/error-address-already-in-use-while-binding-socket-with-address-but-the-port-num/35419032#35419032
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
#include <dirent.h>
#include <unistd.h>

#define MAX_BUFFER_LEN 1024
// #define DIRPATH "/home/ghsot/workspace/data"
#define PORT 7000
#define CONNECTION_STATUS "Connection Accepted"
#define LIST "list"
#define GET "get"
#define LEADNAME "data"

void *connection_handler(void *socketfd);

int main(int argc, char **argv)
{

    struct sockaddr_in *myaddr = malloc(sizeof(struct sockaddr_in));
    pthread_t thread;
    int option = 1;
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);

    if (socketfd == -1)
    {
        perror("socketfd failed\n");
        goto cleanup;
    }
    setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    // int sockopt = setsockopt(socketfd,SOL_SOCKET, SO_DEBUG,  )
    myaddr->sin_addr.s_addr = INADDR_ANY;
    myaddr->sin_family = AF_INET;
    myaddr->sin_port = htons(PORT);

    if (bind(socketfd, (struct sockaddr *)myaddr, sizeof(struct sockaddr_in)) < 0)
    {
        perror("bind failed\n");
        goto cleanup;
    }

    if (listen(socketfd, 5) < 0)
    {
        perror("listen failed\n");
        goto cleanup;
    }

    int newSocket;
    struct sockaddr_in *incoming = malloc(sizeof(struct sockaddr_in));
    socklen_t incoming_len = sizeof(struct sockaddr_in);

    while ((newSocket = accept(socketfd, (struct sockaddr *)incoming, &incoming_len)) > 0)
    {

        if ((pthread_create(&thread, NULL, connection_handler, &newSocket)) < 0)
        {
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

char *list_function(char *path)
{

    DIR *FD = NULL;
    struct dirent *in_file = NULL;
    FILE *output_file = NULL;
    FILE *entry_file = NULL;
    char * ret = NULL;
    char * temp = NULL;
    ret = (char *) calloc(MAX_BUFFER_LEN, sizeof(char));
    temp = ret;

    // attempt to open the folder, if it fails let the user know.
    if ((FD = opendir(path)) == NULL)
    {
        perror("directory open failed in list function");
        return (NULL);
    }

    /**
     * Read the folder content,
     * make a list to return to be sent to the client.
     */

    while ((in_file = readdir(FD)))
    {

        if (!strcmp(in_file->d_name, "."))
            continue;
        if (!strcmp(in_file->d_name, ".."))
            continue;

        entry_file = fopen(in_file->d_name, "r");

        if (entry_file == NULL)
        {
            perror("error on opening entry file");

            if (output_file)
                fclose(output_file);
            return (NULL);
        }

        // printf("\n %s ", in_file->d_name);
        if (strncmp(in_file->d_name,LEADNAME,strlen(LEADNAME)) == 0){
            
            int count = sprintf(temp, "%s\n", in_file->d_name);
            temp += count;
        }
    }
    return (ret);
}

void *connection_handler(void *socketfd)
{

    int sockfd = *(int *)socketfd;
    char buffer[MAX_BUFFER_LEN];
    char cmds[] = "$list - see the avaliable files\n$GET filename - get the file with filename\n";
    // int err;

    // send welcome msg
    send(sockfd, CONNECTION_STATUS, MAX_BUFFER_LEN, 0);
    memset(buffer, 0, MAX_BUFFER_LEN);
    strncpy(buffer, cmds, strlen(cmds));
    send(sockfd, buffer, MAX_BUFFER_LEN, 0);

    while ((read(sockfd, buffer, MAX_BUFFER_LEN)) > 0)
    {

        int count = 0;
        while (buffer[count] != '\0')
        {
            buffer[count] = tolower(buffer[count]);
            count++;
        }

        if (strncmp(buffer, LIST, MAX_BUFFER_LEN) == 0)
        {

            printf("\nin list\n");

            // send a list of file names
            char path[MAX_BUFFER_LEN];
            memset((char *)path, 0, MAX_BUFFER_LEN);
            getcwd((char *)path, MAX_BUFFER_LEN);

            char *listbuffer = NULL;
            if ((listbuffer = list_function(path)) == NULL)
            {
                /*TODO: MORE ERROR HANDLING*/
                return(0);
            }

            send(sockfd, listbuffer, MAX_BUFFER_LEN, 0);
            
            if (listbuffer)
                free(listbuffer);
            listbuffer = NULL;
        }
        else if (strncmp(buffer, GET, MAX_BUFFER_LEN) == 0)
        {
            //send a file
        }

    }

    return (0);
}