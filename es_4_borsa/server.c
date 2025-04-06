#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <netdb.h>
#include "list.h"

int port = 8000;



LIST updateStock(LIST l, ItemType agent) {
    if (isEmpty(l)) {
        return l;
    }

    LIST curr = l;
    while (!isEmpty(curr)) 
	{
        if (strcmp(agent.name, curr->item.name) == 0) 
		{
			curr->item.gain += curr->item.unitary_price;
            curr->item.quantity -= 1;
            curr->item.unitary_price += 1;
        } 
		else 
		{
            curr->item.unitary_price -= 1;
			if ( curr->item.unitary_price <= curr->item.minimum_price )
			{
				curr = Dequeue( curr, curr->item );
				if ( send(curr->item.sockfd, &(curr->item.gain), sizeof(curr->item.gain), 0) == -1 )
				{
					perror("Error on send length");
					exit(1);
				}
			}
        }
        curr = curr->next;
    }

    return l;
}


int main() 
{
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;

	ItemType msg;
	ItemType* agent;
	LIST stock;
	stock = NewList();

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
		printf("\n-------------------------------\n");
		printf("\nWaiting for a new connection...\n");
		printf("Agenti:\n");
		PrintList(stock);
		printf("\n-------------------------------\n");
			
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

		if ( msg.type == 0){

			stock = EnqueueLast( stock, msg );

		} else if ( msg.type == 1) {

			int n = getLength(stock);

			if ( send(msg.sockfd, &n, sizeof(n), 0) == -1 ) {
				perror("Error on send length");
				exit(1);
			}

			LIST tmp = stock;
			while (!isEmpty(tmp)) {

				if ( send(msg.sockfd, &(tmp->item), sizeof(ItemType), 0) == -1 ) {
					perror("Error on send length");
					exit(1);
				}
				tmp = tmp->next;

				char a[NAME_MAX_LENGTH];
				if ( recv( newsockfd, &a, sizeof(a), 0 ) == -1) 
				{
					perror("Error on receive");
					exit(1);
				}
				
				ItemType tmp_agent;
				a[strcspn(a, "\n")] = '\0';
				strcpy( tmp_agent.name, a );
				agent = Find( stock, tmp_agent );
				if ( agent == NULL){
					printf("not found!\n");
				} else {
					PrintItem(*agent);
					printf("\n");
					stock = updateStock( stock, *agent );
				}
				

			}

		}

	    

	}


	close(sockfd);
	return 0;
}



