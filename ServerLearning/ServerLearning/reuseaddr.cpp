#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int reuseAddr(int argc, char* argv[]) {

	if (argc != 2) {
		printf("args error\n");
		return -1;
	}
	int socketfd = socket(PF_INET, SOCK_STREAM, 0);

	assert(socketfd != -1);

	int reuse = 1;

	setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[1]));
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret = bind(socketfd, (struct sockaddr*)&addr, sizeof(addr));
	assert(ret != -1);

	ret = listen(socketfd, 5);
	assert(ret != -1);

	struct sockaddr_in client;
	socklen_t clilen = sizeof(client);

	int connfd = accept(socketfd, (struct sockaddr*)&client, &clilen);
	if (connfd < 0) {
		perror("accept");
		return -1;
	}

	char remote[INET_ADDRSTRLEN];

	printf("客户端的IP地址 %s, 端口是 %d", inet_ntop(AF_INET, &client.sin_addr, remote, INET_ADDRSTRLEN), ntohs(client.sin_port));

	close(connfd);
	close(socketfd);


	return 0;
}