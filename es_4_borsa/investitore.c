#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#include "list.h"

#define BUF_SIZE 1000


char *host_name = "127.0.0.1"; /* local host */
int port = 8000;


int main(int argc, char *argv[]) 
{	
	ItemType msg, server_resp;
	msg.type = 1;

	int length = 0;
		
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



	if ( send(sockfd, &msg, sizeof(msg), 0) == -1 ) 
	{
		perror("Error on send\n");
		exit(1);
	}

	printf("Message sent. Waiting for response...\n");

	while (length <= 0){
		if ( recv(sockfd, &length, sizeof(length), 0) == -1 ) 
		{
			perror("Error in receiving response from server\n");
			exit(1);
		}
	}

	printf("Received list length: %d\n", length);

	for(int i = 0; i < length; i++) {
		if (recv(sockfd, &server_resp, sizeof(server_resp), 0) <= 0) {
			perror("Errore nel ricevere un ItemType");
			close(sockfd);
			exit(1);
		}
		PrintItem(server_resp);
	}

	char buffer[NAME_MAX_LENGTH+1];
	printf("What agent do you want to buy from?\n");
	if (fgets(buffer, sizeof(buffer), stdin) != NULL) {

		if ( send(sockfd, &buffer, strlen(buffer)+1, 0) == -1 ) 
		{
			perror("Error on send\n");
			exit(1);
		}
		printf("Sending agent to server...\n");
		
	}

	close(sockfd);

	return 0;
}



