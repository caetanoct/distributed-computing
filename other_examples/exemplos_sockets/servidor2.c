#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
  int server_sockfd, client_sockfd;
  int server_len, client_len;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;
  char ch;

  server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  server_address.sin_family = AF_INET;
  //server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
  //server_address.sin_addr.s_addr = inet_addr("192.168.0.15");
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(9734);
  server_len = sizeof(server_address);
  bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	listen(server_sockfd, 5);
	while(1) {		
		printf("server waiting\n");
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address, &client_len);
		read(client_sockfd, &ch, 1);
		ch++;
		write(client_sockfd, &ch, 1);
		close(client_sockfd);
	}
}
