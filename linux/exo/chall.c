/*
to compile:
gcc -Wall -pie -fPIE -fstack-protector-all -D_FORTIFY_SOURCE=2 -Wl,-z,now -Wl,-z,relro chall.c -o chall
*/
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define WELCOME_MSG "Welcome on this super secure decompression server! Good luck breaking it!\n"

void doprocessing (int sock) {
	uint8_t output_buff[1024];
	uint8_t *tmp;
	uint8_t command, size, offset, repetitions, i;
	size_t cursor = 0;

	write(sock, WELCOME_MSG, sizeof(WELCOME_MSG)-1);
	while (1) {
		if (read(sock, &command, 1) != 1)
			goto fail;
		switch (command) {
			case 0:
				if (read(sock, &size, 1) != 1)
					goto fail;
				printf("my size %d\n", size);
                // prevent buffer overflow!
				if (cursor + size > 1024)
					goto fail;
				if (read(sock, &output_buff[cursor], size) != size) 
					goto fail;
				cursor += size;
				break;
			case 1:
				if (read(sock, &offset, 1) != 1) 
					goto fail;
				if (read(sock, &size, 1) != 1) 
					goto fail;
				if (read(sock, &repetitions, 1) != 1) 
					goto fail;
				// prevent OOB read!
				if (cursor < offset)
					goto fail;
				tmp = &output_buff[cursor-offset];
				for (i = 0; i < repetitions; i++) {
					// prevent buffer overflow!
					if (cursor > 1024)
						goto fail;
					memmove(&output_buff[cursor], tmp, size);
					cursor += size;
				}
				break;
			case 0xFF:
                write(sock, &cursor, 2);
				write(sock, output_buff, cursor);
				return;
		}
	}
fail:
	fprintf(stderr, "ERROR reading compressed string\n");
	exit(1);
}

int main( int argc, char *argv[] ) {
	int sockfd, newsockfd, portno;
	unsigned int clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int pid;
    
    /* ignore SIGCHLD, prevent zombies */
    struct sigaction sigchld_action = {
      .sa_handler = SIG_DFL,
      .sa_flags = SA_NOCLDWAIT
    };
    sigaction(SIGCHLD, &sigchld_action, NULL);

	/* First call to socket() function */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sockfd < 0) {
		perror("ERROR opening socket");
		exit(1);
	}
	
	/* Initialize socket structure */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = 5001;
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(portno);
	
	/* Now bind the host address using bind() call.*/
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR on binding");
		exit(1);
	}
	
	/* Now start listening for the clients, here
		* process will go in sleep mode and will wait
		* for the incoming connection
	*/
	
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	
	while (1) {
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		
		if (newsockfd < 0) {
			perror("ERROR on accept");
			exit(1);
		}
		
		/* Create child process */
		pid = fork();
		
		if (pid < 0) {
			perror("ERROR on fork");
			exit(1);
		}
		
		if (pid == 0) {
			/* This is the client process */
			close(sockfd);
			doprocessing(newsockfd);
			exit(0);
		}
		else {
			close(newsockfd);
		}
		
	} /* end of while */
}
