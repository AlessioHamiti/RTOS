#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <netdb.h>

#include "list.h"

#define BUF_SIZE 1000

int port = 8000;
int ok = 0;


int main() 
{
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;

	LIST clients;
	clients = NewList();
	LIST suppliers;
	suppliers = NewList();

	ItemType msg;
	ItemType *supplier;
	ItemType *client;

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

	if ( bind( sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr) ) == -1 ) 
	{
		perror("Error on binding");
		exit(1);
	}
	
	if ( listen( sockfd, 20 ) == -1 ) 
	{
		perror("Error on listen");
		exit(1);
	}


	socklen_t address_size = sizeof( cli_addr );	


	while(1) 
	{
		printf("\nWaiting for a new connection...\n");
				
		int newsockfd = accept( sockfd, (struct sockaddr *)&cli_addr, &address_size );     
		if (newsockfd == -1) 
		{
			perror("Error on accept");
			exit(1);
		}

		msg.sockfd = newsockfd;
		
		if ( recv( newsockfd, &msg, sizeof(msg), 0 ) == -1) 
		{
			perror("Error on receive");
			exit(1);
		}

		if (msg.type == 0){  // gestione supplier

			do {
				client = Find( clients, msg );
				if ( client != NULL )
				{
					msg.copies -= 1;

					if ( send( msg.sockfd, &ok, sizeof(ok), 0) == -1)
					{
						perror("Error on send\n");
						exit(1);
					}

					Dequeue( clients, *client );
				} 
				else
				{
					suppliers = EnqueueLast( suppliers, msg );
					break;
				}
			} while ( msg.copies != 0 );

		} else if (msg.type == 1){  // gestione cliente
			
			supplier = Find( suppliers, msg );

			if ( supplier != NULL ){

				supplier->copies -= 1;

				if (supplier->copies == 0)
				{
					Dequeue( suppliers, *supplier );
				}

				printf("Ho trovato il libro richiesto\n");

				if ( send( sockfd, &ok, sizeof(ok), 0) == -1)
				{
					perror("Error on send\n");
					exit(1);
				}

			} else {

				clients = EnqueueLast( clients, msg );
				printf("Libro non trovato, aggiungo il cliente alla lista di attesa\n");

			}

		} else {
			printf("Invalid ID");
		}


		printf("Case editrici: \n");
		PrintList(suppliers);
		printf("Clienti: \n");
		PrintList(clients);


		close(newsockfd);
	}

	close(sockfd);
	return 0;
}



