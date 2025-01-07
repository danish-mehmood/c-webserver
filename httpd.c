/* httpd.c */

#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// listen on all interfaces
//
#define LISTENADDR "0.0.0.0"

/*globals */
char *error;

// returns 0 on error , or it returns the socked fd
//
int server_init(int port)
{
	int s;
	struct sockaddr_in srv;

	// get the socket
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0)
	{
		return 0;
	}
	// now bind the socket
	srv.sin_family = AF_INET;
	srv.sin_addr.s_addr = inet_addr(LISTENADDR);
	srv.sin_port = htons(port);

	if (bind(s, (struct sockaddr *)&srv, sizeof(srv)))
	{

		close(s);
		error = "bind() error";
		return 0;
	}

	if (listen(s, 5))
	{
		close(s);
		error = "listen() error";
		return 0;
	}
	return s;
}

int cli_accept(int fd)
{
	int conn;
	socklen_t addrlen = sizeof(struct sockaddr_in); // Initialize addrlen
	struct sockaddr_in cli;

	memset(&cli, 0, sizeof(cli));
	conn = accept(fd, (struct sockaddr *)&cli, &addrlen); // Pass initialized addrlen
	if (conn < 0)
	{
		error = "accept() error";
		return 0;
	}
	return conn;
}

int cli_conn()
{
	return;
}

int main(int argc, char *argv[])
{

	int fd, conn;
	char *port;

	if (argc < 2)
	{

		fprintf(stderr, "Usage: %s <listening port>\n", argv[0]);
		return -1;
	}
	else
	{
		port = argv[1];
	}

	fd = server_init(atoi(port));
	if (!fd)
	{
		fprintf(stderr, "%s\n", error);
		return -1;
	}

	printf("Listening on %s:%s\n", LISTENADDR, port);
	while (1)
	{

		conn = cli_accept(fd);
		if (!conn)
		{
			fprintf(stderr, "%s/n", error);
			continue;
		}
		printf("Incoming connection \n");
		if (!fork())
		{
			cli_conn(fd, conn);
		}
	}
	return -1;
}
