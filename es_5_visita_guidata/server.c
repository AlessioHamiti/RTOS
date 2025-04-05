#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <netdb.h>

#include "list.h"


int port = 8000;


int main() 
{
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;

	ItemType msg;
	ItemType *guide;
	LIST guides;
	guides = NewList();
	LIST visitors;
	visitors = NewList();

	char response_visitors[NAME_MAX_LENGTH+1] = ""; 
	int response_guide = 0;

	// Socket opening
	int sockfd = socket( PF_INET, SOCK_STREAM, 0 );  
	if ( sockfd == -1 ) 
	{
		perror("Error opening socket");
		exit(1);
	}
	
	int options = 1;
	if(setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, &options, sizeof (options)) < 0) {
		perror("Error on setsockopt");
		exit(1);
	}

	bzero( &serv_addr, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	// Address bindind to socket
	if ( bind( sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr) ) == -1 ) 
	{
		perror("Error on binding");
		exit(1);
	}
	
	// Maximum number of connection kept in the socket queue
	if ( listen( sockfd, 20 ) == -1 ) 
	{
		perror("Error on listen");
		exit(1);
	}

	socklen_t address_size = sizeof( cli_addr );	


	while(1) 
	{
		printf("---------------------------------\n");
		printf("\nWaiting for a new connection...\n");
		printf("Guides list:\n");
		PrintList(guides);
		printf("\nVisitors list:\n");
		PrintList(visitors);
		printf("---------------------------------\n");
			
		int newsockfd = accept( sockfd, (struct sockaddr *)&cli_addr, &address_size );      
		if (newsockfd == -1) 
		{
			perror("Error on accept");
			exit(1);
		}
		
		if ( recv( newsockfd, &msg, sizeof(msg), 0 ) == -1) 
		{
			perror("Error on receive");
			exit(1);
		}

		msg.sockfd = newsockfd;
		
		if ( msg.type == 0)  // guide
		{
			printf("Received guide from client: \n");
			PrintItem(msg);
			printf("\n");

			guides = EnqueueLast( guides, msg );
		} 
		else if ( msg.type == 1 )  // visitatori
		{ 
			printf("Received visitors from client: \n");
			PrintItem(msg);
			printf("\n");

			guide = FindGuide( guides, msg.visitors );

			if ( guide == NULL )
			{
				strcpy( response_visitors, "" );
				if ( send(msg.sockfd, &response_visitors, strlen(response_visitors), 0) == -1 ) {
					perror("Error on send\n");
					exit(1);
				}
				close(msg.sockfd);
			}
			else if (guide->min_visitors > msg.visitors)
			{
				visitors = EnqueueLast( visitors, msg );
			}
			else
			{
				response_guide = msg.visitors;
				if ( send(guide->sockfd, &response_guide, sizeof(response_guide) + 1, 0) == -1 ) {
					perror("Error on send\n");
					exit(1);
				}
				guides = Dequeue(guides, *guide);
				close(guide->sockfd);

				strcpy( response_visitors, guide->name );
				if ( send(msg.sockfd, &response_visitors, strlen(response_visitors), 0) == -1 ) {
					perror("Error on send\n");
					exit(1);
				}
				close(msg.sockfd);
			}

		}
		
	}

	close(sockfd);
	return 0;
}



