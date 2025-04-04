#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <netdb.h>

#include "list.h"

#define BUF_SIZE 1000


char *host_name = "127.0.0.1"; /* local host */
int port = 8000;


int main(int argc, char *argv[]) 
{
	struct sockaddr_in serv_addr;
 	struct hostent* server;	
	int answer = 0;
	int rcv = 0;
	char server_response[NAME_MAX_LENGTH];

	ItemType center;

	
	if (argc < 3) { 
		printf("Usage: %s name - patients per day\n", argv[0]);
		exit(-1);
	}
	strcpy( center.name, argv[1] );
	center.entity = 1;
	center.patients = atoi(argv[2]);
	
		
	if ( ( server = gethostbyname(host_name) ) == 0 ) 
	{
		perror("Error resolving local host\n");
		exit(1);
	}

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = ((struct in_addr *)(server->h_addr))->s_addr;
	serv_addr.sin_port = htons(port);
	
	int sockfd = socket( PF_INET, SOCK_STREAM, 0 );
	if ( sockfd == -1 ) 
	{
		perror("Error opening socket\n");
		exit(1);
	}    

	if ( connect(sockfd, (void*)&serv_addr, sizeof(serv_addr) ) == -1 ) 
	{
		perror("Error connecting to socket\n");
		exit(1);
	}

	printf("Sending my data\n");
	PrintItem(center);

	if ( send(sockfd, &center, sizeof(center), 0) == -1 ) 
	{
		perror("Error on send\n");
		exit(1);
	}

	printf("Waiting response\n");

	do{
		if ( recv(sockfd, &server_response, NAME_MAX_LENGTH+1, 0) == -1 ) 
		{
			perror("Error in receiving response from server\n");
			exit(1);
		}
	} while ( strlen(server_response) <= 0 );

	printf("Ho ricevuto i vaccini richiesti dal seguente fornitore: %s\n", server_response);

	close(sockfd);

	return 0;
}



