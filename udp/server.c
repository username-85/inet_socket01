#include "util.h"
#include "common.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	socklen_t addrlen = 0;
	int sockfd = inet_bind(PORT_SRV, SOCK_DGRAM, &addrlen);
	if (sockfd < 0) {
		fprintf(stderr, "could not create socket\n");
		exit(EXIT_FAILURE);
	}

	printf("server: waiting for messages ...\n");
	struct sockaddr_storage client_addr;
	while(1) {
		char buf[MAXDSIZE] = {0};
		socklen_t ca_size = sizeof(client_addr);
		int numbytes = recvfrom(sockfd, buf, MAXDSIZE - 1, 0,
		                        (struct sockaddr *)&client_addr,
		                        &ca_size);
		if (numbytes == -1) {
			perror("recvfrom");
			exit(EXIT_FAILURE);
		}
		buf[numbytes] = '\0';

		char addr_str[INET6_ADDRSTRLEN + 10] = {0};
		inet_addr_str((struct sockaddr *)&client_addr,
		              ca_size, addr_str, sizeof(addr_str));
		printf("server: got message from %s :%s\n", addr_str, buf);

		numbytes = sendto(sockfd, MSG_SRV, strlen(MSG_SRV), 0,
		                  (struct sockaddr *)&client_addr, ca_size);
		if (numbytes == -1) {
			perror("sendto");
			exit(EXIT_FAILURE);
		}
	}
	close(sockfd);

	exit(EXIT_SUCCESS);
}

