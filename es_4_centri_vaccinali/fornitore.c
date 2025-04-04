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
	int answer;
	char server_response[NAME_MAX_LENGTH];

	ItemType supplier;
	
	if (argc < 4) { 
		printf("Usage: %s name - availability - minimum\n", argv[0]);
		exit(-1);
	}
	strcpy( supplier.name, argv[1] );
	supplier.entity = 0;
	supplier.availability = atoi(argv[2]);
	supplier.minimum = atoi(argv[3]);
		
	
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


	printf("Sending my data:\n");
	PrintItem(supplier);

	if ( send(sockfd, &supplier, sizeof(supplier), 0) == -1 ) 
	{
		perror("Error on send\n");
		exit(1);
	}

	printf("waiting response \n");
	
	do{
		if ( recv(sockfd, &server_response, NAME_MAX_LENGTH+1, 0) == -1 ) 
		{
			perror("Error in receiving response from server\n");
			exit(1);
		}
		printf("Ho fornitro il seguente centro vaccinale: %s\n", server_response);
	} while (1);

	close(sockfd);

	return 0;
}



