#include <sys/un.h>
#include <netinet/in.h>
//socket address to unix
struct sockaddr_un {
	sa_family_t sun_family; /* AF_UNIX */
	char sun_path[]; /* pathname */
};
//socket address to network
struct sockaddr_in {
	short int sin_family; /* AF_INET */
	unsigned short int sin_port; /* Port number */
	struct in_addr sin_addr; /* Internet address */
};
// ip address to socket
struct in_addr {
	unsigned long int s_addr;
};
