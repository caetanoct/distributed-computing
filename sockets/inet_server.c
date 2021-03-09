#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
int main() {
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	char ch;
	// instancia sockfd
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = 9734;
	server_len = sizeof(server_address);
	// atribui ao socket ip+porta
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	// declara que esta pronto p/ receber conexoes e ate quantas devem ser enfileiradas
	listen(server_sockfd, 5);
	while(1) {
		printf("server waiting\n");
		client_len = sizeof(client_address);
		// comando bloqueante - bloqueia ate algum cliente/processo tente executar um connect.
		// cliente invoca connect e uma vez que o accept ocorre com sucesso a variavel recebera o endereco do cliente e atualiza client_address/client_len
		client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address, &client_len);
		// se accept executou ele criou o client_socketfd (associado ao socket do cliente)
		// lendo do socket do cliente (se o client enviar algo é a partir desse socket que leremos)
		// armazenando o valor que lermos do client_sockfd em ch
		read(client_sockfd, &ch, 1);
		// prox codificacao ascii
		ch++;
		// tenta escrever no scoket do client o conteudo de ch (nesse caso envia B)
		write(client_sockfd, &ch, 1);
		close(client_sockfd);
	}
	exit(0);
}