#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
const unsigned short int PORT = 9734;

void print_ip(unsigned int ip)
{
    unsigned char bytes[4];
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;   
    printf("%d.%d.%d.%d\n", bytes[0], bytes[1], bytes[2], bytes[3]);        
}

int main() {
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	// instancia sockfd do servidor
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = PORT;
	server_len = sizeof(server_address);
	
	//server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	printf("\nServer ipv4 address: ");
	print_ip(server_address.sin_addr.s_addr);

	// assign a local socket address (server_address) to a socket (server_sockfd)
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

	// declara que esta pronto p/ receber conexoes e ate quantas devem ser enfileiradas
	listen(server_sockfd, 5);

	while(1) {
		printf("server waiting\n");
		client_len = sizeof(client_address);
		
		// comando bloqueante - bloqueia ate algum cliente/processo tente executar um connect.
		// cliente invoca connect e uma vez que o accept ocorre com sucesso a variavel recebera o endereco do cliente e atualiza client_address/client_len
		client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address, &client_len);

		printf("peer socket ipv4 address: ");
		print_ip(client_address.sin_addr.s_addr);

		while (1) {
			char string_from_client[10];
			int result;
			// reads 10bytes from client socket and store the value to the buffer
			read(client_sockfd, &string_from_client, 10);
			if (strcmp("sair",string_from_client) == 0) {
				printf("Solicitação de encerramento de conexão do cliente, fecharei o seu socket e voltarei a escutar solicitações.\n");
				break;
			}
			char* token = strtok(string_from_client, "#");
			char* token1 = strtok(NULL, "#");
			char* token2 = strtok(NULL, "#");		
			if (string_from_client[0]=='+') {			
				result = atoi(token1) + atoi(token2);			
			}
			if (string_from_client[0]=='-') {			
				result = atoi(token1) - atoi(token2);
			}
			if (string_from_client[0]=='*') {			
				result = atoi(token1) * atoi(token2);
			}
			if (string_from_client[0]=='/') {			
				result = atoi(token1) / atoi(token2);
			}			
			write(client_sockfd, &result, sizeof(result));	
		}
		// finaliza conexão com o cliente
		close(client_sockfd);
	}
	exit(0);
}