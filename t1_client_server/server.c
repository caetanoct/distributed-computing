#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "queue.h"
#include "logger/logger.h"

#define BASE_SERVER_PORT 8080
#define SERVER_ADDRES "127.0.0.1"
#define REQUEST_BUFFER_SIZE 4096
#define THREAD_POOL_SIZE 10

#ifndef MEMORY
#define MEMORY MEMORY
#define NUM_SERVERS NUM_SERVERS
#endif

char memory[MEMORY];

int server_id = 0;

pthread_t thread_pool[THREAD_POOL_SIZE];
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t memory_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;

typedef struct requestData {
    int position;
    int size;
    char * buffer;
} requestData;

void initialize_mem(char * memory);

requestData deserialize_write(char * request);

requestData deserialize_read(char * request);

void * handle_connection(void * p_client_sockfd);

void * thread_worker(void * arg);

void main(int argc, char * argv[]) {
    int server_sockfd, client_sockfd, server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        pthread_create(&thread_pool[i], NULL, thread_worker, NULL);
    }

    // Deslocates server port based on server number
    server_id = atoi(argv[1]);
    int server_port = BASE_SERVER_PORT + server_id;

    pthread_t logger_thread;
    logger_info l_info = {
        .mem_address = memory,
        .server_id = server_id,
        .num_servers = NUM_SERVERS,
        .server_mem = MEMORY,
        .mem_lock = &memory_mutex,
    };

    pthread_create(&logger_thread, NULL, logger, (void *) &l_info);

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = server_port;
    server_len = sizeof(server_address);

    bind(server_sockfd, (struct sockaddr *) &server_address, server_len);
    listen(server_sockfd, 50);
  
    initialize_mem(memory);

    printf("Server listening on port: %d\n", server_port);

    while (1) {
        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, (struct sockaddr *) &client_address, &client_len);
        
        int * pclient = malloc(sizeof(int));
        *pclient = client_sockfd;
        
        pthread_mutex_lock(&queue_mutex);
        enqueue(pclient);
        pthread_cond_signal(&condition_var);
        pthread_mutex_unlock(&queue_mutex);

        // pthread_t t;
        // int *pclient = malloc(sizeof(int));
        // *pclient = client_sockfd;
        // pthread_create(&t, NULL, handle_connection, (void *) pclient);
    }

}

void initialize_mem(char * memory) {
    for (int i = 0; i < MEMORY; i++) {
        memory[i] = '0';
    }
    memory[MEMORY - 1] = '\0';
}

void * handle_connection(void * p_client_sockfd) {
    int client_sockfd = *((int *) p_client_sockfd);
    free(p_client_sockfd);
    char req[REQUEST_BUFFER_SIZE];

    read(client_sockfd, req, REQUEST_BUFFER_SIZE);
    requestData data;

    // Checks first char to see if request type is read or write
    if (req[0] == '0') { // Write
        data = deserialize_write(req + 2);

        pthread_mutex_lock(&memory_mutex);
        strncpy(memory + data.position, data.buffer, data.size);
        pthread_mutex_unlock(&memory_mutex);

        free(data.buffer);
    }
    else if (req[0] == '1') {
        data = deserialize_read(req + 2);
        char response[data.size];
        
        pthread_mutex_lock(&memory_mutex);
        strncpy(response, memory + data.position, data.size);
        pthread_mutex_unlock(&memory_mutex);
        
        write(client_sockfd, response, data.size);

    }

    close(client_sockfd);
    
    return NULL;
}

void * thread_worker(void * arg) {
    while (1) {
        int * pclient;
        pthread_mutex_lock(&queue_mutex);
        if ((pclient = dequeue()) == NULL) {
            pthread_cond_wait(&condition_var, &queue_mutex);
            // Tries again
            pclient = dequeue();
        }
        pthread_mutex_unlock(&queue_mutex);

        if (pclient != NULL) {
            printf("thread busy\n");
            handle_connection(pclient);
            printf("thread clear\n");
        }
    }
}

requestData deserialize_write(char * request) {
    int len = strlen(request);
    int last_sep = 0;
    int sep_positions[3];


    for (int i = 0; i < len; i++) {
        if (request[i] == ',') {
            sep_positions[last_sep] = i;
            last_sep++;
        }
    }

    char position[sep_positions[0]];
    char size[sep_positions[1] - sep_positions[0] - 1];
    char * buffer = (char *) malloc(len - sep_positions[2] * sizeof(char));

    strncpy(position, request, sep_positions[0]);
    strncpy(size, request + sep_positions[0] + 1, sep_positions[1] - sep_positions[0] - 1);
    strncpy(buffer, request + sep_positions[1] + 1, len - sep_positions[2]);

    requestData data = {
        .position = 
            atoi(position) - server_id * MEMORY,
        .size = atoi(size),
        .buffer = buffer
    };

    return data;
}

requestData deserialize_read(char * request) {
    int len = strlen(request);
    int sep_pos = 0;
    
    for (int i = 0; i < len; i++) {
        if (request[i] == ',') {
            sep_pos = i;
            break;
        }
    }
    
    char position[sep_pos];
    char size[len - sep_pos - 1];

    strncpy(position, request, sep_pos);
    strncpy(size, request + sep_pos + 1, len - sep_pos - 1);
    
    requestData data = {
        .position = 
            atoi(position) - server_id * MEMORY,
        .size = atoi(size),
    };

    return data;
}
