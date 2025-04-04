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
	int server_response = 1;
	
	if (argc < 2) {
		perror("Usage: ./lettore {titolo del libro richiesto}\n");
		exit(1);
	}

	ItemType msg;
	msg.type = 1;
	strncpy(msg.title, argv[1], sizeof(msg.title) - 1);
	msg.title[sizeof(msg.title) - 1] = '\0';
	msg.copies = 0;
		
	struct sockaddr_in serv_addr;
 	struct hostent* server;	
	
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

	printf("Inviano il lettore al server\n");
	PrintItem(msg);
	printf("\n");

	if ( send( sockfd, &msg, sizeof(msg), 0) == -1)
	{
		perror("Error on send\n");
		exit(1);
	}

	printf("Message sent. Waiting for response...\n");
	
	do{
		if ( recv(sockfd, &server_response, sizeof(server_response), 0) == -1 ) 
		{
			perror("Error in receiving response from server\n");
			exit(1);
		}
	} while ( server_response != 0 );

	printf("Ho ricveuto il libro dal server");

	close(sockfd);

	return 0;
}



