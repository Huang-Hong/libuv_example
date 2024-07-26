
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <iostream>

// g++ -o tcpClient tcpClient.cc

int initTcpClient() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cout << "create socket err" << std::endl;
        return sockfd;
    }

    sockaddr_in serverAddr; //服务器信息
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7000);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int ret = connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (ret != 0) {
        std::cout << "client socket connect err, ret is " << ret << std::endl;
        return -1;
    }
    int i = 0;
    while (1) {
        char message[128];
        snprintf(message, 128, "%s%d", "hello", i);
        ssize_t sent = send(sockfd, message, strlen(message), 0);
        i++;
        sleep(2);
    }

    return sockfd;
}

int main() {
    initTcpClient();
    return 0;
}