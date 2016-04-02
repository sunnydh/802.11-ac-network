#include <sys/types.h>
#include <sys/socket.h>


/*
1. Important Structures Used in Socket Programming

a. 

	struct sockaddr_in {
		short int sin_family; // Address family
		unsigned short int sin_port; // Port number
		struct in_addr sin_addr; // Internet address
		unsigned char sin_zero[8]; // Same size as struct sockaddr
	};


b. Internet address (a structure for historical reasons)

	struct in_addr {
		unsigned long s_addr; // that’s a 32-bit long, or 4 bytes
	};

2. Important methods to be used

	htons() – "Host to Network Short"
	htonl() – "Host to Network Long"
	ntohs() – "Network to Host Short"
	ntohl() – "Network to Host Long"
	ina.sin_addr.s_addr = inet_addr("10.12.110.57"); // converts an IP address in numbers-and-dots notation into an unsigned long
	int inet_aton("10.12.110.57", &(my_addr.sin_addr)); // converts an IP address, ascii to network, returns 0 on error and non-zero on success
	char* inet_ntoa() // netwotk to numbers and dot notation, network to ascii

3. System Calls

a. socket()–Get the File Descriptor!
	
	int socket(int domain, int type, int protocol);
		domain = AF_INET
		type = SOCK_STREAM or SOCK_DGRAM
		protcol = 0 to automatically select correct protocol
		returns a socket descriptor, -1 on error

b. bind()–What port am I on?

	int bind(int sockfd, struct sockaddr *my_addr, int addrlen);
		sockfd = file descriptor returned by socket()
		my_addr is a pointer to a struct sockaddr that contains information about your address, namely, port and IP address
		addrlen can be set to sizeof(struct sockaddr)
	



*/

