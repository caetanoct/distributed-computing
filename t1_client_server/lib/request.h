#ifndef REQUEST_H
#define REQUEST_H

#define REQUEST_BUFFER_SIZE 4096

void write_to_server(char* position, char* buffer_size, char * buffer, int sockfd);

void read_from_server(char* position, char* buffer_size, int sockfd);

#endif