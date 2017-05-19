#include "util.h"
#include "common.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	socklen_t addrlen = 0;
	int sockfd = inet_listen(PORT_SRV, BACKLOG, &addrlen);
	if (sockfd < 0) {
		fprintf(stderr, "could not create socket\n");
		exit(EXIT_FAILURE);
	}

	printf("server: waiting for connections...\n");
	struct sockaddr_storage client_addr;
	while(1) {

		socklen_t ca_size = sizeof(client_addr);
		int client_fd = accept(sockfd, (struct sockaddr *)&client_addr,
		                       &ca_size);
		if (client_fd == -1) {
			perror("accept");
			continue;
		}

		char addr_str[INET6_ADDRSTRLEN + 10] = {0};
		inet_addr_str((struct sockaddr *)&client_addr,
		              ca_size, addr_str, sizeof(addr_str));
		printf("server: got connection from %s\n", addr_str);

		char buf[MAXDSIZE] = {0};
		int numbytes = recv(client_fd, buf, MAXDSIZE - 1, 0);
		if (numbytes == -1) {
			perror("recv");
			exit(EXIT_FAILURE);
		}
		buf[numbytes] = '\0';
		printf("server: got message from %s: '%s'\n", addr_str, buf);

		if (send(client_fd, MSG_SRV, sizeof(MSG_SRV), 0) == -1)
			perror("send");

		close(client_fd);
	}
	close(sockfd);

	exit(EXIT_SUCCESS);
}

