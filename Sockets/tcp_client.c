/* 
 * tcpclient.c - A simple TCP client
 * usage: tcpclient <host> <port> <folder(2.4-20)> <run_number>
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
#include <string.h>

#define BUFSIZE 1024

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

    int sockfd, portno, n, tcp_info_length;         //sockfd: socket file descriptor, tcp_info_length: length of structure for storing tcp parameters returned by getsockopt
    struct sockaddr_in serveraddr;                  //Server address 
    struct hostent *server;                         //structure storing the name converted host
    char *hostname;                                 //For storing host name
    char buf[BUFSIZE];                              //Buffer for storing the data to be transmitted
    struct tcp_info tcp_info;                       //structure for storing tcp parameters returned by getsockopt
    unsigned short opt_debug;                       //debug variable to be used
    int read_block;                                 //Stores the block of data read at a time from the file
    struct timeval start_time, end_time;            //start_time: time(in secs) when file transfer has been initiated, end_time: time when the block is sent


    /* check command line arguments */
    if (argc != 4) {
       fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
       exit(0);
    }

    /* get host name and port number of server */
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


    //Time now to send a large file
    /* First open the file in read mode */
    FILE *fp = fopen("/home/susan/Downloads/1.jpg", "r");
    if(fp == NULL){
        printf("File open error\n");
        return 0;
    }
    /* File to write the output TCP parameters */
    char filepath[1024];
    filepath[0]='\0';
    strcat(filepath,"/home/susan/Desktop/BTP/Client/");
    strcat(filepath,argv[3]);
    strcat(filepath"/stats");
    strcat(filepath,argv[4]);
    strcat(filepath,".txt");

    FILE *fp_out = fopen(filepath, "w+");
    if(fp_out == NULL){
        printf("Output file open error\n");
        return 0;
    }

    /* Get the time when the file transfer is initiated */
    get_now(&start_time, opt_debug);
    
    /* Read one block at a time and send it to socket */
    while((read_block = fread(buf, 1, BUFSIZE, fp)) > 0){

        if(send(sockfd, buf, read_block, 0) < 0){
            printf("Error in sending file\n");
            break;
        }
        bzero(buf, BUFSIZE);

        //Use get sock opt to get tcp parameters at this point of time
        tcp_info_length = sizeof(tcp_info);
        get_now(&end_time, opt_debug);
        int ret_sock = getsockopt(sockfd, SOL_TCP, TCP_INFO, (void *)&tcp_info, (socklen_t *)&tcp_info_length);
        if(ret_sock == 0){
            fprintf(fp_out, "%.6f\t%u\t%u\t%u\n", time_to_seconds(&start_time, &end_time), tcp_info.tcpi_snd_cwnd, tcp_info.tcpi_snd_ssthresh, tcp_info.tcpi_rtt);
        }
        else{
            printf("Error in get sock opt\n");
            break;
        }
    }

    /*And finally time to close the file and the socket */
    fclose(fp);
    close(sockfd);
    fclose(fp_out);

    return 0;
}