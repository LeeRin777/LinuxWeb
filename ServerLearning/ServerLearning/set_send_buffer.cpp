#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>

#define BUFFER_SIZE 512

int main(int argc, char* argv[]) {

    if (argc <= 3)
    {
        printf("usage: %s ip_address port_number send_bufer_size\n", basename(argv[0]));
        return 1;
    }


    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket"); return -1;
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    struct hostent* ips= gethostbyname(argv[1]);
    memcpy(&servaddr.sin_addr, ips->h_addr, ips->h_length);
    servaddr.sin_port = htons(atoi(argv[2]));

    int sendbuf = atoi(argv[3]);
    int len = sizeof(sendbuf);
    setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sendbuf, sizeof(sendbuf));
    getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sendbuf, (socklen_t*)&len);
    printf("the tcp send buffer size after setting is %d\n", sendbuf);

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != -1)
    {
        char buffer[BUFFER_SIZE];
        memset(buffer, 'a', BUFFER_SIZE);
        send(sockfd, buffer, BUFFER_SIZE, 0);
    }

    close(sockfd);
    return 0;

}