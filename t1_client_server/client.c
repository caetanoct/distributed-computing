#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "lib/request.h"

#define BASE_SERVER_PORT 8080
#define SERVER_ADDRES "127.0.0.1"
#define REQUEST_BUFFER_SIZE 4096

#ifndef MEMORY
#define MEMORY MEMORY
#define NUM_SERVERS NUM_SERVERS
#endif
// memory and num_servers will be defined in compilation time
int get_server_port(int position);

/* params
* operation: int write 0 read 1
* position: int:
* buffer_size: int
* buffer: string (only for writes)
*/
int main(int argc, char* argv[]) {
    int sockfd;
    int len;
    struct sockaddr_in address;

    int server_port = BASE_SERVER_PORT + get_server_port(atoi(argv[2]));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(SERVER_ADDRES);
    address.sin_port = server_port;
    len = sizeof(address);


    int conn = connect(sockfd, (struct sockaddr *)&address, len);
    if (conn == -1) {
        perror("opps: connection error");
        return 1;
    }
    // argv[1] == 0 means write to server, argv[1] == 1 means read from server
    // argv[2] == addr start
    // argv[3] == read amount/size of argv[4]
    // argv[4] == word
    if (strcmp(argv[1], "0") == 0) {
        write_to_server(argv[2], argv[3], argv[4], sockfd);
    } else if (strcmp(argv[1], "1") == 0) {
        read_from_server(argv[2], argv[3], sockfd);
    }

    return 0;
}
// gets server port based on argv[2] (addr start)
int get_server_port(int position) {
    for (int i = 1; i <= NUM_SERVERS; i++) {
        if (position < i * MEMORY) {
            return i - 1;
        }
    }
}