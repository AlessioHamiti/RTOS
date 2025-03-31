#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <netdb.h>
#include <time.h>


#define BUF_SIZE 1000
#define MAX_CLIENTS 4


char *host_name = "127.0.0.1"; /* local host */
int port = 8000;


int main(int argc, char *argv[]){
    struct sockaddr_in srv_addr;
    struct hostent* server;
    int num, answer, ret;

    if ( ( server = gethostbyname(host_name) ) == 0 ){
		perror("Error resolving local host\n");
		exit(1);
	}

    bzero(&srv_addr, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = ((struct in_addr *)(server->h_addr))->s_addr;
	srv_addr.sin_port = htons(port);

    for (int i=0; i<MAX_CLIENTS; i++){
        if( ( ret = fork() ) == 0 ){
            // Opening socket
            int sockfd = socket( PF_INET, SOCK_STREAM, 0 );
            if ( sockfd == -1 ){
                perror("Error openening socket");
                exit(1);
            }

            // Connection to socket
            if ( connect(sockfd, (void*)&srv_addr, sizeof(srv_addr) ) == -1 ) 
            {
                perror("Error connecting to socket\n");
                close(sockfd);
                exit(1);
            }

            int pid = getpid();
            srand(time(NULL) + pid);
            num = rand() % 100 + 1;

            printf("Sending random number: %d, my PID is: %d\n", num, pid);

            if ( send(sockfd, &num, sizeof(num), 0) == -1 ) 
            {
                perror("Error on send\n");
                close(sockfd);
                exit(1);
            }

            if (recv(sockfd, &num, sizeof(num), 0 ) == -1) {
                perror("Error on receive");
                close(sockfd);
                exit(1);
            }

            printf("received %d, my PID is:%d\n", num, pid);

            exit(0);

            close(sockfd);
        }
    }

    return(0);
}