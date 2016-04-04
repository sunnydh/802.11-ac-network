#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <syslog.h>
#include <string.h>





#define BACKLOG 10 // how many pending connections queue will hold
#define BUFFER_SIZE 1024

/*

1. Important Structures Used in Socket Programming

a.
	struct sockaddr {
		unsigned short sa_family; // address family, AF_xxx
		char sa_data[14]; // 14 bytes of protocol address
	};

b. 

	struct sockaddr_in {
		short int sin_family; // Address family
		unsigned short int sin_port; // Port number
		struct in_addr sin_addr; // Internet address
		unsigned char sin_zero[8]; // Same size as struct sockaddr
	};

c. Internet address (a structure for historical reasons)

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
		returns -1 on error

c. connect()–Hey, you!

	int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);
		sockfd = file descriptor returned by socket()
		serv_addr is a struct sockaddr containing the destination port and IP address
		addrlen can be set to sizeof(struct sockaddr)
		return -1 on error

d. listen()–Will somebody please call me?

	int listen(int sockfd, int backlog);
		sockfd = file descriptor returned by socket()
		backlog = number of connections allowed on the incoming queue
		return -1 on error

e. accept()–"Thank you for calling the port"

	int accept(int sockfd, void *addr, int *addrlen);
		sockfd = file descriptor returned by socket()
		addr will usually be a pointer to a local struct sockaddr_in. This is where the information about the incoming connection will go
		addrlen = sizeof(struct sockaddr_in)
		returns file descriptior for new connection, returns -1 on error

f. send() and recv()–Talk to me, baby!

	int send(int sockfd, const void *msg, int len, int flags);
		sockfd is the socket descriptor you want to send data to (whether it’s the one returned by socket() or the one you got with accept().)
		msg is a pointer to the data you want to send
		len is the length of that data in bytes
		flags = 0
		returns the number of bytes actually sent out, or -1 on error

	int recv(int sockfd, void *buf, int len, unsigned int flags);
		buf is the buffer to read the information into
		len is the maximum length of the buffer
		flags = 0
		returns the number of bytes actually read into the buffer, or -1 on error
		recv() can return 0, the remote side has closed the connection on you

g. close() and shutdown()–Get outta my face!

	close(sockfd);
		This will prevent any more reads and writes to the socket

*/

/* Get current time. */
void get_now( struct timeval *time, unsigned short debug ) {
	if ( gettimeofday( time, NULL ) != 0 ) {
		fprintf(stderr,"Can't get current time.\n");
	}
	return;
}

/* Convert "struct timeval" to fractional seconds. */
double time_to_seconds ( struct timeval *tstart, struct timeval *tfinish ) {

	double t;
	t = (tfinish->tv_sec - tstart->tv_sec) + (tfinish->tv_usec - tstart->tv_usec) / 1e6;
	return t;
}



int main(int argc, char **argv)
{
	if(argc<3){
		printf("Less no of arguments [file.o] [IP] [port] required\n");
		return 0;	
	}



	int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
	struct sockaddr_in my_addr; // my address information
	struct sockaddr_in their_addr; // connector’s address information
	struct sigaction sa;
	int yes=1;
	int opt_buffer, opt_debug=0;
	

	/* Structures needed for measuring time intervals */
	struct timeval time_start, time_now, time_delta;


	opt_buffer = BUFFER_SIZE;

	// create listening socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	// reuse the socket in different runs
	if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}

	// binding
	my_addr.sin_family = AF_INET; // host byte order
	my_addr.sin_port = htons(atoi(argv[2])); // short, network byte order
	inet_aton(argv[1], &(my_addr.sin_addr)); // fill with my IP
	// my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
	memset(&(my_addr.sin_zero), '0', 8); // zero the rest of the struct
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))== -1) {
		perror("bind");
		exit(1);
	}

	// listen
	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	while(1) { 
		// main accept() loop
		unsigned int sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1){
			perror("accept");
			continue;
		}
		printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr));
		if (!fork()) { 
			// this is the child process
			close(sockfd); // child doesn’t need the listener



			// if (send(new_fd, "Hello, world!\n", 14, 0) == -1)
			// 	perror("send");
			// char buff[1024];
			// int recvd;
			// while((recvd=recv(new_fd, buff, sizeof(buff), 0)) != 0){
			// 	printf("Recieved %d bytes\n",recvd);
			// }

			void *tcp_buffer;
			tcp_buffer = malloc(opt_buffer*sizeof(char));
			struct tcp_info tcp_info;
			int recv_bytes;
			FILE *statistics;
			FILE *fp;
			statistics = fopen( "/home/sunman/Desktop/btp/802.11-ac-network/Sockets/Serever_stats.txt", "w+" );
			fp = fopen("/home/sunman/Downloads/temp.txt","w+");

			printf("File transfer start\n");

			get_now( &time_start, opt_debug );
			while ( (recv_bytes = recv( new_fd, tcp_buffer, opt_buffer, 0 ) ) > 0 ) {
				printf("No of recv bytes %d\n", recv_bytes);
				fwrite(tcp_buffer,recv_bytes,1,fp);
				/* Measure time in order to create time intervals. */
				get_now( &time_now, opt_debug );
				/* Fill tcp_info structure with data */
				unsigned int tcp_info_length = sizeof(tcp_info);
				if ( getsockopt( new_fd, SOL_TCP, TCP_INFO, (void *)&tcp_info, &tcp_info_length ) == 0 ) {
					fprintf(statistics,"%.6f %u %u %u %u %u %u %u %u %u %u %u %u\n",
							time_to_seconds( &time_start, &time_now ),
							tcp_info.tcpi_last_data_sent,
							tcp_info.tcpi_last_data_recv,
							tcp_info.tcpi_snd_cwnd,
							tcp_info.tcpi_snd_ssthresh,
							tcp_info.tcpi_rcv_ssthresh,
							tcp_info.tcpi_rtt,
							tcp_info.tcpi_rttvar,
							tcp_info.tcpi_unacked,
							tcp_info.tcpi_sacked,
							tcp_info.tcpi_lost,
							tcp_info.tcpi_retrans,
							tcp_info.tcpi_fackets
						   );
					if ( fflush(statistics) != 0 ) {
						fprintf(stderr, "Cannot flush buffers: %s\n", strerror(errno) );
					}
				}
			}
			printf("File transfer ends\n");
			free(tcp_buffer);
			close(new_fd);
			fclose(fp);
			fclose(statistics);
			exit(0);
		}
		close(new_fd); // parent doesn’t need this
	}

	return 0;
}