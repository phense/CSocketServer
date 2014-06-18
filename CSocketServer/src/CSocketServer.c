/*
 ============================================================================
 Name        : CSocketServer.c
 Author      : Peter Hense
 Version     : 0.1a
 Copyright   : (c) by Peter Hense 2014 - 2017 GPL v2 or higher
 Description : Socket Server Linux
 ============================================================================
 */

#define PORT 5001
#define MAXMSGLENGTH 32

#include <stdio.h>
#include <stdlib.h>			// strlen
#include <string.h>			// strlen
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>		// inet_addr
#include <unistd.h>			// write


/*
 * This function is where the actual processing of information takes place
 */
void forkmain(int sock) {
	int rwstatus;
	char buffer[MAXMSGLENGTH];						// Message content

	memset(&buffer, 0, sizeof(buffer));
	rwstatus = read(sock,buffer,MAXMSGLENGTH);
	if (rwstatus < 0) {
		perror("Error reading from socket");
		close(sock);
		return;
	}
	printf("Here is the message: %s\n",buffer);

	rwstatus = write(sock,"Message received",18);
	if (rwstatus < 0) {
		perror("Error writing to socket");
		close(sock);
		return;
	}
}

/*
 * Server - Loop for Socket-Connection
 */
int main( int argc, char *argv[] )
{
    int sockfd, newsockfd, clilen, on;				// Socket stuff
    pid_t pid; 										// child process id
    struct sockaddr_in serveraddr, cli_addr;

    /*
     * SOCKET INITIALIZATION
     */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);		// initialize new socket
    if (sockfd < 0)
    {
        perror("Could not create socket");
        exit(1);
    }
    puts("Socket created");

    /* Enable address reuse */
    on = 1;
    if (setsockopt( sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
    	perror("Error setting socket options, exiting.");
    	exit(1);
    }

    /* Allow connections to PORT from any available interface  */
    memset(&serveraddr, '0', sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl( INADDR_ANY );
    serveraddr.sin_port = htons(PORT);

    /* Bind the host address using bind() call.  */
    if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
         perror("Error binding to socket, exiting.");
         exit(1);
    }

    /*  wait for an incoming connection   */
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    /*
     * SERVER LOOP
     */

    while (1) {
    	newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    	if (newsockfd < 0) {
    		perror("Error on client connection. Abort.");
    	    exit(1);
    	}
    	puts("Connection accepted");
    	/* connection established, let child process handle the work */

    	pid = fork();

    	/* Error */
    	if (pid < 0) {
    		perror("Error creating child process. Abort.");
    		exit(1);
    	}

    	/* Server child */
    	if (pid == 0) {
    	    close(sockfd);
    	    forkmain(newsockfd);
    	    close(newsockfd);
    	    puts("Client connection terminated.");
    	    exit(0);
    	}

    	/* Close connection */
    	else {
    		close(newsockfd);
    	}
    } /* of while */
} /* of main */
