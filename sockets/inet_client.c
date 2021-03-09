#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
int main() {
	// socket file descriptor
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	char ch = 'A';
	// familia, type, protocol
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// no endereço setaremos os valores para cada campo
	address.sin_family = AF_INET;	
	// endereco do destinatario
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	// porta
	address.sin_port = 9734;
	len = sizeof(address);
	// invoca o connect e informa o endereco do servidor (onde tem um processo aguardando accept).
	// sockfd = socket criado anteriormente, conexao sera feita nesse socket
	result = connect(sockfd, (struct sockaddr *)&address, len);
	if(result == -1) {
		perror("oops: client1");
		exit(1);
	}
	//The write() function attempts to write nbyte bytes from the buffer pointed to by buf to the file associated with the open file descriptor, fildes.
	// envia 1 byte pro socket
	write(sockfd, &ch, 1);
	//The read() function attempts to read nbyte bytes from the file associated with the open file descriptor, fildes, into the buffer pointed to by buf.
	read(sockfd, &ch, 1);
	// le 1 byte do socket
	// o conteudo que vier do socket no read ira sobreescrever &ch
	printf("char from server = %c\n", ch);
	close(sockfd);
	exit(0);
}