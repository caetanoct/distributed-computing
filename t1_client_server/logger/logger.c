#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/request.h"
#include "logger.h"

#define BASE_SERVER_PORT 8080
#define SERVER_ADDRES "127.0.0.1"

void save_log(char * buffer);
void get_sibling_buffer(int port, int sibling_id, int server_mem, char * stream);
void make_read_request(char* position, char* buffer_size, int sockfd, char * stream);

// Logger is should be a thread worker 
void * logger(void * p_info) {
    int server_id = ((logger_info *) p_info)->server_id;
    char * mem_address = ((logger_info *) p_info)->mem_address;
    int num_servers = ((logger_info *) p_info)->num_servers;
    int server_mem = ((logger_info *) p_info)->server_mem;
    pthread_mutex_t * mem_lock = ((logger_info *) p_info)->mem_lock;

    int initial_sleep = GLOBAL_LOG_PERIOD * (server_id + 1);
    int sleep_period = num_servers * GLOBAL_LOG_PERIOD;

    sleep(initial_sleep);
    while (1) {
        char * stream = malloc(sizeof(char) * server_mem * num_servers + num_servers);

        for (int i = 0; i < num_servers; i++) {
            if (i != server_id) {
                printf("LOGGER: making request to server %d\n", i);
                // Request data
                get_sibling_buffer(BASE_SERVER_PORT + i, i, server_mem, stream);
            }
            else {
                pthread_mutex_lock(mem_lock);
                strcat(stream, mem_address);
                pthread_mutex_unlock(mem_lock);
            }
        }
        save_log(stream);
        free(stream);
        printf("LOGGER: server %d write on log.txt \n", server_id);
        sleep(sleep_period);
    }
}

void get_sibling_buffer(int port, int sibling_id, int server_mem, char * stream) {
    int sockfd;
    int len;
    struct sockaddr_in address;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(SERVER_ADDRES);
    address.sin_port = port;
    len = sizeof(address);
    
    int conn = connect(sockfd, (struct sockaddr *)&address, len);
    if (conn == -1) {
        perror("opps: connection error\n");
        return;
    }

    char * position = malloc(sizeof(char) * REQUEST_BUFFER_SIZE);
    sprintf(position, "%d", sibling_id * server_mem);
    char * size = malloc(sizeof(char) * REQUEST_BUFFER_SIZE);
    sprintf(size, "%d", server_mem);
    make_read_request(position, size, sockfd, stream);

}

void save_log(char * buffer) {
    FILE * f = fopen("log.txt", "w");
    fputs(buffer, f);
    fclose(f);
}

void make_read_request(char* position, char* buffer_size, int sockfd, char * stream) {
    char * request = malloc(sizeof(char) * REQUEST_BUFFER_SIZE);
    memset(request, 0, strlen(request));

    strcat(request, "1,");
    strcat(request, position);
    strcat(request, ",");
    strcat(request, buffer_size);

    free(position);
    free(buffer_size);

    send(sockfd, request, REQUEST_BUFFER_SIZE, 0);
    free(request);

    char response[REQUEST_BUFFER_SIZE];
    read(sockfd, response, REQUEST_BUFFER_SIZE);
    
    strcat(stream, response);
}