#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include "request.h"

void write_to_server(char* position, char* buffer_size, char * buffer, int sockfd) {
    char request[REQUEST_BUFFER_SIZE];

    strcat(request, "0,");
    strcat(request, position);
    strcat(request, ",");
    strcat(request, buffer_size);
    strcat(request, ",");
    strcat(request, buffer);

    printf("Write Request: %s\n", request);
    write(sockfd, request + 1, REQUEST_BUFFER_SIZE);
}

void read_from_server(char* position, char* buffer_size, int sockfd) {
    char request[REQUEST_BUFFER_SIZE];

    strcat(request, "1,");
    strcat(request, position);
    strcat(request, ",");
    strcat(request, buffer_size);

    printf("READ Request: %s\n", request);
    send(sockfd, request, REQUEST_BUFFER_SIZE, 0);
    
    char response[REQUEST_BUFFER_SIZE];
    read(sockfd, response, REQUEST_BUFFER_SIZE);
    printf("%s\n", response);
}