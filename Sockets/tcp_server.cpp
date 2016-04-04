/*

Build instructions

g++ -o server.o tcp_server.cpp
./server.o 192.168.1.1 3300 2-20

Output

Server_stats.txt in Server_out folder
format of file in fprintf statement below

*/

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


// Get current time.
void get_now( struct timeval *time, unsigned short debug ) {
	if ( gettimeofday( time, NULL ) != 0 ) {
		fprintf(stderr,"Can't get current time.\n");
	}
	return;
}

// Convert "struct timeval" to fractional seconds.
double time_to_seconds ( struct timeval *tstart, struct timeval *tfinish ) {

	double t;
	t = (tfinish->tv_sec - tstart->tv_sec) + (tfinish->tv_usec - tstart->tv_usec) / 1e6;
	return t;
}



int main(int argc, char **argv)
{
	if(argc<4){
		printf("Less no of arguments [file.o] [IP] [port] [2-20|2-40|5-20|5-40|5-80] required\n");
		return 0;	
	}

	int sockfd, new_fd, opt_buffer, opt_debug=0, yes=1; // listen on sock_fd, new connection on new_fd
	struct sockaddr_in my_addr; // my address information
	struct sockaddr_in their_addr; // connector’s address information
	struct sigaction sa;

	/* Structures needed for measuring time intervals */
	struct timeval time_start, time_now, time_delta;


	opt_buffer = BUFFER_SIZE;

	// create listening socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket");
		exit(1);
	}

	// reuse the socket in different runs
	if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1){
		perror("setsockopt");
		exit(1);
	}

	// bind
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(atoi(argv[2]));
	inet_aton(argv[1], &(my_addr.sin_addr));
	memset(&(my_addr.sin_zero), '0', 8);
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))== -1) {
		perror("bind");
		exit(1);
	}

	// listen
	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	while(1){ 
		
		unsigned int sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1){
			perror("accept");
			continue;
		}

		printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr));

		if (!fork()) { 
			// this is the child process
			close(sockfd); // child doesn’t need the listener

			void *tcp_buffer;
			int recv_bytes;
			FILE *statistics;
			FILE *fp;
			unsigned int tcp_info_length;
			struct tcp_info tcp_info;

			tcp_buffer = malloc(opt_buffer*sizeof(char));
			statistics = fopen( "/home/sunman/Desktop/btp/802.11-ac-network/Sockets/Server_out/Server_stats.txt", "a+");
			fp = fopen("/home/sunman/Downloads/ca.mkv","w+");

			fprintf(statistics,"File transfer start for %s\n",argv[3]);
			printf("File transfer start for %s\n",argv[3]);

			get_now( &time_start, opt_debug );
			while ((recv_bytes = recv(new_fd, tcp_buffer, opt_buffer, 0)) > 0){
				
				fwrite(tcp_buffer,recv_bytes,1,fp);
				get_now( &time_now, opt_debug );
				
				tcp_info_length = sizeof(tcp_info);
				if (getsockopt(new_fd, SOL_TCP, TCP_INFO, (void *)&tcp_info, &tcp_info_length) == 0){

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

			fprintf(statistics, "File transfer ends %s\n", argv[3]);
			printf("File transfer ends %s\n", argv[3]);
			free(tcp_buffer);
			close(new_fd); fclose(fp); fclose(statistics);
			exit(0);

		}

		close(new_fd); // parent doesn’t need this
	}

	return 0;
}