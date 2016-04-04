/* 
 * tcpclient.c - A simple TCP client
 * usage: tcpclient <host> <port>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h> 
#include <sys/time.h>
#include <fcntl.h>

#define BUFSIZE 1024

/* 
 * error - wrapper for perror
 */
void error(char *msg) {
    perror(msg);
    exit(0);
}

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


int main(int argc, char **argv) {
    int sockfd, portno, n, tcp_info_length;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];
    struct tcp_info tcp_info;
    unsigned short opt_debug;

    /* check command line arguments */
    if (argc != 3) {
       fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
       exit(0);
    }
    hostname = argv[1];
    portno = atoi(argv[2]);

    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }

    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
	  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);

    /* connect: create a connection with the server */
    if (connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) 
      error("ERROR connecting");

    printf("Connection with server established. Now beginning to transfer file\n");

    //Time now to send a large file
<<<<<<< HEAD
    //int fp = open("/home/sanket/Downloads/Timeline.pdf", O_RDONLY, 0644);
    FILE *fp = fopen("/home/sanket/Downloads/sanket.txt", "r");
=======
    FILE *fp = fopen("~/Downloads/thor.mkv", "rb");
>>>>>>> afd331da2c23acfa9d7272e380ec14c60a424d40
    if(fp == NULL){
        printf("File open error\n");
        return 0;
    }
    int sent_block, transfer;
    struct timeval start_time, end_time;
    get_now(&start_time, opt_debug);
    //buf = "I am client and I am awesome";
    /*int buf_size = 28;
    transfer = send(sockfd, buf, buf_size, 0);
    printf("%u\n", transfer);
    printf("Sent\n");*/
    printf("%u\n", BUFSIZE);
    while(sent_block = fread(buf, 1, BUFSIZE, fp) > 0){
        printf("%d\n", sent_block);
        printf("LOL\n");
        if(transfer = send(sockfd, buf, sent_block, 0) < 0){
            printf("Error in sending file\n");
            break;
        }
        printf("%d\n", transfer);
        //bzero(buf, BUFSIZE);
        //Use get sock opt to get tcp parameters at this point of time
        tcp_info_length = sizeof(tcp_info);
        get_now(&end_time, opt_debug);
        int ret_sock = getsockopt(sockfd, SOL_TCP, TCP_INFO, (void *)&tcp_info, (socklen_t *)&tcp_info_length);
        if(ret_sock == 0){
            printf("%.6f\t%u\n", time_to_seconds(&start_time, &end_time), tcp_info.tcpi_snd_cwnd);
        }
        else{
            printf("Error in get sock opt\n");
            break;
        }
    }
    fclose(fp);
    close(sockfd);
    return 0;
}