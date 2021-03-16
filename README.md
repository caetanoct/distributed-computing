# Distributed Computing Repository

## Remote Method Invocation - Java

To compile and run go to rmi/ directory and run:

```sh
javac *.java
rmiregistry &
java -Djava.security.policy=server.policy Server
```

In another terminal execute:

```sh
java -Djava.security.policy=server.policy Client
```

The program implements a client-shared monotonic counter.

## Socket Implementation - C

Execute calculator server/client program:

```sh
cd sockets

make

./server &
./client
```

### Server-Side Functions
```C
/* 
Family: UNIX, Internet, XNS
Type: stream, datagram
Protocol: TCP/UDP
*/
sockfd = (int) socket (int family, int type, int protocol)
/*
Assigns to socket ip:port

The bind() function shall assign a local socket address address to a socket identified by descriptor socket that has no local socket address assigned. Sockets created with the socket() function are initially unnamed; they are identified only by their address family
*/
ret = (int) bind (int sockfd, struct sockaddr *myaddr, int addrlen)
/*
-marks the socket referred to by sockfd as a passive socket, that is, as a socket that will be used to accept incoming connection requests using accept()
-backlog = max queue size
-linux default: /proc/sys/net/ipv4/tcp_max_syn_backlog
*/
ret = (int) listen (int sockfd, int backlog)
/*
It extracts the first connection request on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket. The newly created socket is not in the listening state.  The original socket sockfd is unaffected by this call
https://man7.org/linux/man-pages/man2/accept.2.html
*/
newsock = (int)accept(int sockfd, struct sockaddr *peer, int *addrlen)
// close socket
ret = (int) close (int sockfd)
```
### Client-Side Functions
```C
/* 
Family: UNIX, Internet, XNS
Type: stream, datagram
Protocol: TCP/UDP
*/
sockfd = (int) socket (int family, int type, int protocol)
/*
https://man7.org/linux/man-pages/man2/connect.2.html
*/
ret = (int)connect(int sockfd, struct sockaddr *servaddr, int addrlen)
// send to sockfd
nbytes = (int)send(int sockfd, char *buf, int nbytes, int flags)
//server side:
nbytes =(int)recv(int sockfd, char *buf, int nbytes, int flags)
// close socker
ret = (int) close (int sockfd)
```
### Structs - sockaddr
```C
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
```
### Useful Links - Sockets

* [Programming UNIX Sockets in C](http://www.softlab.ntua.gr/facilities/documentation/unix/unix-socket-faq/unix-socket-faq.html)
* [Socket Developer Handbook](https://docs.freebsd.org/en_US.ISO8859-1/books/developers-handbook/sockets.html)
* [netinet/in.h](https://pubs.opengroup.org/onlinepubs/7908799/xns/netinetin.h.html)
* [arpa/inet.h](https://pubs.opengroup.org/onlinepubs/7908799/xns/arpainet.h.html)
* [unistd.h](https://pubs.opengroup.org/onlinepubs/7908799/xsh/unistd.h.html)
* [stdlib.h](https://pubs.opengroup.org/onlinepubs/009604499/basedefs/stdlib.h.html)
* [string.h](https://pubs.opengroup.org/onlinepubs/7908799/xsh/string.h.html)
* [sys/socket.h](https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_socket.h.html)
* [sys/types.h](https://pubs.opengroup.org/onlinepubs/009696899/basedefs/sys/types.h.html)