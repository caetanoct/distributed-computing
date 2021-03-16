#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// CHANGE AS NEEDED
//const char* HOST = "192.168.15.7";
const char* HOST = "127.0.0.1";
const unsigned short int PORT = 9734;

int main() {	
	struct sockaddr_in sockaddress;	
	// familiy, type, protocol
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// setaremos os valores da struct sockaddr_in para cada campo
	sockaddress.sin_family = AF_INET;		
	sockaddress.sin_addr.s_addr = inet_addr(HOST);	
	sockaddress.sin_port = PORT;
	// requests a connection to be made on a socket	
	int return_code = connect(sockfd, (struct sockaddr *)&sockaddress, sizeof(sockaddress));
	if(return_code != 0) {
		perror("An error ocurred while connecting on the socket");
		exit(1);
	} else {
		printf("Succesfully connected on the socket, peer: %s:%u\n", HOST,PORT);
	}
	printf("--- Instructions ---\n1) Syntax: operand#num1#num2:\n2) To end the connection with the host type \'sair\'\n");
	while (1) {
		char string[10];
	    scanf("%s", string);
	    // envia a string ao socket do cliente
		write(sockfd, string, sizeof(string));
	    if (strcmp("sair",string) == 0) {
			printf("Enviei solicitação de desconexão ao servidor e fecharei o socket local.\n");
			break;
		}
		int result_server;
		read(sockfd, &result_server, sizeof(result_server));
		printf("%d\n", result_server);
	}
	close(sockfd);
	exit(0);
}