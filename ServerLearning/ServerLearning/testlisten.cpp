#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

static bool stop = false;
static void handle_term(int sig) 
{
	stop = true;
}

int testlisten(int argc, char* argv[]) {

	signal(SIGINT, handle_term);

	if (argc <= 2) {
		printf("usage: %s ip_address port_number backlog\n", basename(argv[0]));
		return -1;
	}

	//const char* ip = argv[1];
	//int port = atoi(argv[2]);
	//int backlog = atoi(argv[3]);

	int listenfd = socket(PF_INET, SOCK_STREAM, 0);
	if (listenfd == -1) {
		perror("listenfd");
		return -1;
	}

	struct sockaddr_in servAddr;
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(atoi(argv[1]));
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int ret = bind(listenfd, (struct sockaddr*)&servAddr, sizeof(servAddr));
	if (ret == -1) {
		perror("bind");
		close(listenfd);
		return -1;
	}

	ret = listen(listenfd, atoi(argv[2]));
	if (ret == -1) {
		perror("bind");
		close(listenfd);
		return -1;
	}

	while (!stop) {
		sleep(1);
	}

	close(listenfd);
	return 0;

}