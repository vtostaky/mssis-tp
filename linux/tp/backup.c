/*
to compile:
gcc -Wall -pie -fPIE -fstack-protector-all -D_FORTIFY_SOURCE=2 -Wl,-z,now -Wl,-z,relro chall.c -o chall
*/
#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define WRITE_STR(s) write(sock, s, sizeof(s)-1);
#define WELCOME_MSG "Welcome on this super secure server! Good luck breaking it!\n"

long pgcd(long a, long b)
{
	long r;
	while (b != 0)
	{
		r = a % b;
		a = b;
		b = r;
	}
	return a;
}

long super_pgcd(long *longs, size_t nb_longs)
{
	long r = longs[0];
	for (size_t i = 1; i < nb_longs; i++)
	{
		r = pgcd(r, longs[i]);
		if (r == 1)
			return r;
	}
	return r;
}

int read_long(int sock, long * output)
{
	char c;
	long r = 0;

	while (read(sock, &c, 1) == 1)
	{
		if (c == '\n')
		{
			*output = r;
			return 0;
		}
		if (('0' > c) || (c > '9'))
		{
			WRITE_STR("invalid input\n");
			return 1;
		}
		r = r * 10 + c - '0';
	}
	WRITE_STR("unable to read input\n");
	return -1;
}

void ltoa(long l, char* output)
{
	bool negative = l < 0;
	if (l == 0)
	{
		output[0] = '0';
		output[1] = '\0';
		return;
	}
	if (l < 0)
	{
		output[0] = '-';
		output ++;
	}
	ssize_t i;
	for (i = 0; l != 0; i++)
	{
		output[i] = negative ? -(l % 10) : l % 10;
		l /= 10;
	}
	output[i] = '\0';
	ssize_t j;
	for (j = 0, i--; j <= i; j++, i--)
	{
		char c;
		c = output[i] + '0';
		output[i] = output[j] + '0';
		output[j] = c;
	}
}

void doprocessing (int sock)
{
	WRITE_STR(WELCOME_MSG);
	WRITE_STR("operation? (pgcd: 1, secure shuffle: 2)\n");
	long operation;
	if (read_long(sock, &operation) != 0)
		return;
	if ((operation != 1) && (operation != 2))
	{
		WRITE_STR("invalid operation\n");
		return;
	}

	WRITE_STR("how many inputs?\n");
	long nb_inputs;
	if (read_long(sock, &nb_inputs) != 0)
		return;
	if (nb_inputs <= 0)
	{
		WRITE_STR("invalid number of inputs\n");
		return;
	}

	long *longs;
	size_t sizeof_longs = nb_inputs * sizeof(long);
	if (sizeof_longs > 256)
	{
		longs = malloc(sizeof_longs);
		if (longs == NULL)
		{
			WRITE_STR("unable to allocate space for all the inputs (too much inputs?)\n");
			return;
		}
	}
	else
	{
		longs = alloca(sizeof_longs);
	}

	for (long i = 0; i < nb_inputs;)
	{
		int r = read_long(sock, &longs[i]);
		if (r < 0)
			break;
		else if (r == 0)
			i++;
	}

	char output[23];
	if (operation == 1)
	{
		ltoa(super_pgcd(longs, nb_inputs), output);
		WRITE_STR ("result: ");
		write (sock, output, strlen(output));
		WRITE_STR ("\n");
	}
	else
	{
		// todo: implement a more secure random....
		srand(time(NULL));
		// Fisher-Yates algorithm
		for (long i = 0; i < nb_inputs; i++)
		{
			long j = rand() % (nb_inputs - i) + i;
			long tmp = longs[i];
			longs[i] = longs[j];
			longs[j] = tmp;
		}
		WRITE_STR ("result: ");
		WRITE_STR ("\n");
		for (long i = 0; i < nb_inputs; i++)
		{
			ltoa(longs[i], output);
			write (sock, output, strlen(output));
			WRITE_STR ("\n");
		}
	}
}

int main( int argc, char *argv[] )
{
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

	if (sockfd < 0)
	{
		perror("ERROR opening socket");
		exit(1);
	}

	int enable = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
	{
		perror("setsockopt(SO_REUSEADDR) failed");
		exit(1);
	}

	/* Initialize socket structure */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = 5001;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(portno);

	/* Now bind the host address using bind() call. */
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("ERROR on binding");
		exit(1);
	}

	/* Now start listening for the clients, here
		* process will go in sleep mode and will wait
		* for the incoming connection
	*/

	listen(sockfd, 5);
	clilen = sizeof(cli_addr);

	while (1)
	{
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

		if (newsockfd < 0)
		{
			perror("ERROR on accept");
			exit(1);
		}

		/* Create child process */
		pid = fork();

		if (pid < 0)
		{
			perror("ERROR on fork");
			exit(1);
		}

		if (pid == 0)
		{
			/* This is the client process */
			close(sockfd);
			doprocessing(newsockfd);
			exit(0);
		}
		else
		{
			close(newsockfd);
		}

	} /* end of while */
}