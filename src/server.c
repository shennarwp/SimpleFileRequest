/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "findfile.h"

//print error message
void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	char buffer[1024];											//holds the message
	char names[1024];											//holds the names received from client
	char size[8];												//holds the size

	int sockfd, newsockfd, portno;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	pid_t childpid;

	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}

	//create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	//define server address and port
	portno = atoi(argv[1]);										//port number from parameter
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	//bind socket to port
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		sizeof(serv_addr)) < 0)
		error("ERROR on binding");

	//start listening
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	int counter = 0;
	while(1)													//endless loop
	{
		//accept connection
		newsockfd = accept(sockfd,
			(struct sockaddr *) &cli_addr,
			&clilen);
		if (newsockfd < 0)
			error("ERROR on accept");
	 	printf("Request nr: %d\n", ++counter);

		childpid = fork();										//fork() so it can serve multiple clients
		switch(childpid)
		{
			 case 0: //child process
			 {
			 	close(sockfd);

				//READ FILENAMES
			 	bzero(names, 1024);
			 	n = read(newsockfd, names, 1023);
			 	if (n < 0)
			 		error("ERROR reading from socket");

				//READ FIRST n-BYTES
			 	bzero(size, 8);
			 	n = read(newsockfd, size, 7);
			 	if (n < 0)
			 		error("ERROR reading from socket");

			 	findfile(buffer, names, atoi(size));			//call the findfile, result will be saved in buffer

			 	usleep(100000);

		 		//SEND RESPONSE
			 	n = write(newsockfd, buffer,1024);
			 	if (n < 0)
			 		error("ERROR writing to socket");
			 	close(newsockfd);
			 	exit(0);
			 }

			 default: //parent process
			 {
			 	close(newsockfd);
			 }
			}
		}
		return 0;
	}
