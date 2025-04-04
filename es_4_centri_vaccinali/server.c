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

	LIST suppliers;
	LIST centers;

	suppliers = NewList();
	centers = NewList();

	ItemType msg, c;
	ItemType* s;

	int totalRequired = 0;
	/*
	strcpy( msg.name, "cE" );
	msg.entity = 1;
	msg.patients = 11;
	centers = EnqueueLast( centers, msg );

	strcpy( msg.name, "cB" );
	msg.entity = 1;
	msg.patients = 6;
	centers = EnqueueLast( centers, msg );

	strcpy( msg.name, "cD" );
	msg.entity = 1;
	msg.patients = 5;
	centers = EnqueueLast( centers, msg );

	strcpy( msg.name, "cC" );
	msg.entity = 1;
	msg.patients = 3;
	centers = EnqueueLast( centers, msg );
	*/
	int answer;

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
		printf("----------------------------\n");
		printf("\nWaiting for a new connection...\n");
		printf("Lista centri:\n");
		PrintList(centers);
		printf("\nLista fornitori:\n");
		PrintList(suppliers);
		printf("\n----------------------------\n");
		
		// New connection acceptance		
		int newsockfd = accept( sockfd, (struct sockaddr *)&cli_addr, &address_size );      
		if (newsockfd == -1) 
		{
			perror("Error on accept");
			exit(1);
		}
		
		// Message reception
		if ( recv( newsockfd, &msg, sizeof(msg), 0 ) == -1) 
		{
			perror("Error on receive");
			exit(1);
		}

		msg.sockfd = newsockfd;
		printf("Received from client:\n");
		PrintItem(msg);

		if ( msg.entity == 0 ) // SUPPLIER
		{
			suppliers = EnqueueLast(suppliers, msg);
		} 
		else if ( msg.entity == 1 ) // CENTER
		{
			centers = EnqueueLast(centers, msg);
		} 
		else 
		{
			printf("Ivalid id\n");
		}

		while ( !isEmpty(suppliers) && !isEmpty(centers) ) {
			s = getHead( suppliers );
			
			totalRequired = TotalRequired(centers);

			if (!isEmpty(centers) && totalRequired >= s->minimum) {

				c = FindMax(centers, s->availability);

				if ( c.patients != 0 ){

					s->availability -= c.patients;
					if ( send(s->sockfd, c.name, strlen(c.name)+1, 0) == -1 )
					{
						perror("Error on send\n");
						exit(1);
					}

					if ( send(c.sockfd, s->name, strlen(s->name)+1, 0) == -1 )
					{
						perror("Error on send\n");
						exit(1);
					}
					close(c.sockfd);
					centers = Dequeue(centers, c);

				} else {
					break;
				}

				if ( s->availability == 0 ) {
					close(s->sockfd);
					suppliers = DequeueFirst( suppliers );
				}

			} else {
				printf("No centers available, waiting for new center...\n");
			}

		}

	}

	close(sockfd);
	return 0;
}



