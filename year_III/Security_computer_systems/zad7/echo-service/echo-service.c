/*
Compile with:
gcc -m32 -o echo-service echo-service.c
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>


void echo(void)
{
	int c, i = 0;
	char buffer[64];

	bzero(buffer, sizeof(buffer));

	while ((c = getchar()) != EOF) {
		if (c == '\n') {
			break;
		}
		buffer[i++] = (char) c;
	}

	if (c == EOF) {
		fputs("Got EOF, closing client handler\n", stderr);
		exit(0);
	}

	puts(buffer);
	fflush(stdout);
}

void echo_loop(void)
{
    puts("Welcome to the echo service. I will echo everything you send to me.");
    fflush(stdout);
    while (1) {
    	echo();
    }
}

void accept_clients(int socketfd)
{
    int c;
    struct sockaddr_in client;

    //Accept and incoming connection
    fputs("Waiting for incoming connections...\n", stderr);
    c = sizeof(struct sockaddr_in);

    while (1)
    {
        //accept connection from an incoming client
        int clientfd = accept(socketfd,
        	(struct sockaddr *)&client, (socklen_t *)&c);
        if (clientfd < 0)
        {
            perror("accept failed");
            exit(-1);
        }
        fputs("Connection accepted\n", stderr);


        switch (fork()) {
        	case -1:
        		perror("fork failed");
        		exit(-1);
        	case 0:
        		/* child */
        		close(socketfd);
        		dup2(clientfd, 0);
        		dup2(clientfd, 1);
        		echo_loop();
        	default:
        		/* parent */
        		close(clientfd);
        		while (waitpid((pid_t) -1, (int *) 0, WNOHANG) > 0) {
	   				 /* Wait for children that might have terminated */
				}
        		break;
        }

    }
}

int bind_to_port(unsigned short port)
{
    int socketfd;
    struct sockaddr_in server;

    //Create socket
    socketfd = socket(AF_INET , SOCK_STREAM , 0);
    if (socketfd == -1)
    {
        perror("Could not create socket");
        exit(-1);
    }
    fputs("socket created\n", stderr);

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    //Bind
    if (bind(socketfd, (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("Bind failed. Error");
        exit(-1);
    }
    fputs("bind complete\n", stderr);


    return socketfd;
}

int main(int argc , char *argv[])
{
    int socketfd;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s PORT\n", argv[0]);
		exit(1);
	}

    socketfd = bind_to_port(atoi(argv[1]));

    //Listen
    listen(socketfd, 100);

    accept_clients(socketfd);

	return 0;
}
