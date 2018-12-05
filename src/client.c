#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

//print error message
void error(const char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd, portno, n, i;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[1024];											//holds the message

	if (argc < 3) {
		fprintf(stderr,"Usage %s hostname port\n", argv[0]);
		exit(0);
	}

	//create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}

	//define server address and port
	portno = atoi(argv[2]);										//port number from parameter
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
		(char *)&serv_addr.sin_addr.s_addr,
		server->h_length);
	serv_addr.sin_port = htons(portno);

	//connect to server
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
		error("ERROR connecting");

	//READ FROM USER AND SEND FILENAMES
	printf("Please enter up to 5 FILENAME separated by space that want to be searched!\n");
	printf("Only the first 5 filenames will be searched!\n");
	bzero(buffer, 1024);
	fgets(buffer, 1023, stdin);
	n = write(sockfd, buffer, strlen(buffer));
	if (n < 0)
		error("ERROR writing to socket");

    //READ FROM USER FIRST n-BYTES
	char size[8];												//holds the size as char array
	char *end;
	int num;
	do {
		printf("Please enter how many of the FIRST BYTES that want to be displayed!\n");
		printf("Enter integer from 0 - 10!\n");
		if (!fgets(size, sizeof size, stdin))
			break;
	     // remove \n
		size[strlen(size) - 1] = 0;
		num = strtol(size, &end, 10);
	} while (end != size + strlen(size) || num < 0 || num > 10);			//continue if inputted number < 0 or > 10

	//SEND FIRST n-BYTES
	n = write(sockfd, size, strlen(size));
	if (n < 0)
		error("ERROR writing to socket");

    //READ RESPONSE FROM SERVER
	bzero(buffer,1024);
	n = read(sockfd, buffer, 1023);
	if (n < 0)
		error("ERROR reading from socket");
	printf("%s\n", buffer);

	close(sockfd);
	usleep(1000);

	return 0;
}
