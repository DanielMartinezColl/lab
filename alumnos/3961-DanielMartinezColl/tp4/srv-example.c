#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include "service.h"

int main(int argc, char **argv)
{
	struct sockaddr_in direccion = {};
	
	int sofd;
	int conec;
	
 	sofd = socket(PF_INET, SOCK_STREAM, 0);

	if (sofd == -1) { 
		perror("socket()");
		exit(EXIT_FAILURE); 
	}

	/* ADDR. FAMILY */
	direccion.sin_family= AF_INET;
	/* PORT */
	direccion.sin_port = htons(9000);
	/* ADDRESS */
	inet_pton(AF_INET, "127.0.0.1", &direccion.sin_addr);
	
	if (bind(sofd, (struct sockaddr *) &direccion , sizeof (direccion)) < 0) { 
		perror("bind()");
		exit(EXIT_FAILURE); 
	}

	if (listen(sofd, 5) < 0) { 
		perror("listen()");
		exit(EXIT_FAILURE); 
	}

	while ( (conec = accept(sofd, NULL, 0)) > 0) {
		switch (fork()) {
			case 0: 	// hijo
				service(conec);
				return 0;

			case -1:	// error
				perror("fork()");
				return 1;
		}

		close(conec);
	}

	return 0;	
}

