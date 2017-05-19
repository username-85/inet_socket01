#include "util.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "usage: client hostname\n");
		exit(EXIT_FAILURE);
	}

	int sockfd = inet_connect(argv[1], PORT_SRV, SOCK_DGRAM);
	if (sockfd < 0) {
		fprintf(stderr, "socket error");
		exit(EXIT_FAILURE);
	}

	if (send(sockfd, MSG_CLIENT, sizeof(MSG_CLIENT), 0) == -1) {
		perror("send");
		exit(EXIT_FAILURE);
	}

	char buf[MAXDSIZE] = {0};
	int numbytes = recv(sockfd, buf, MAXDSIZE - 1, 0);
	if (numbytes == -1) {
		perror("recv");
		exit(EXIT_FAILURE);
	}
	buf[numbytes] = '\0';

	printf("client: received '%s'\n",buf);
	close(sockfd);

	exit(EXIT_SUCCESS);
}

