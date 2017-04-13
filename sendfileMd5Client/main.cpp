/*
 *简单TCP通信
 获取时延
 map显示时延
 * */
#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include <limits>
#include <string>
#include <queue>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <sys/wait.h>
#include <netinet/tcp.h>

#include "DefineVal.h"
#include "Md5.h"
#define eps 1e-8
#define BUFFER_SIZE 1024

using namespace std;

const int PORT = 124;
const char *IP = "127.0.0.1";

int listenInit(){
//TCP
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
#ifdef DEBUG
        std::cout << "socket error" << std::endl;
#endif
        exit(-1);
    }

    int optval;
    setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &optval, sizeof(optval));

    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &serverAddr.sin_addr);

    //TODO:设置为非阻塞
//    int flags = fcntl(sockfd, F_GETFL, 0);
//    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);


    int con = connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if(con < 0){
#ifdef DEBUG
        std::cout << "connect error" << std::endl;
#endif
        close(sockfd);
        exit(-1);
    }

    int len = sizeof(unsigned int);
    setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &optval, len);
    setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &optval, len);

    return sockfd;

}

void sendCmd(int sockfd, const void *buffer, size_t len, int flags){
    std::cout << string(static_cast<const char*>(buffer)) << std::endl;
    if(send(sockfd, buffer, len, flags) < 0){
#ifdef DEBUG
        std:: cout << "send cmd error" << std::endl;
#endif
        exit(-1);
    }
}
int main(){
    cout << "client start" << endl;
    int sockfd = listenInit();

    string filename = "Md5.h";
    FILE *fp = fopen(filename.c_str(),"r");
    if(fp == NULL){
        cout << filename << "not found" << endl;
        exit(-1);
    }

    cout << "client Md5 = " << md5file(filename.c_str()) << endl;

    //send transfer file msg
//    string transferFileMsg = "7E4500007E";
//    sendCmd(sockfd, transferFileMsg.c_str(), transferFileMsg.size(), 0);




    int fileBlockLen = 0;
    char buf[BUFFER_SIZE];
//    bzero(buf, sizeof(buf));
//
//    read(sockfd, buf, BUFFER_SIZE);
//
//    cout << "recv = " << buf << endl;
    bzero(buf,sizeof(buf));
    while((fileBlockLen = fread(buf, sizeof(char), BUFFER_SIZE, fp)) > 0){
        sendCmd(sockfd, buf, fileBlockLen, 0);
        usleep(6);
        bzero(buf, sizeof(buf));
    }

    fclose(fp);
    close(sockfd);

//
//    string endFileMsg = "7E4511117E";
//    sendCmd(sockfd, endFileMsg.c_str(), endFileMsg.size(), 0);
//    close(sockfd);



//    pid_t p;
//    int status;
//    if((p = fork()) == 0){
//        close(1);
//        dup(sockfd);
//
//        execl("/bin/cat", "/bin/cat", "1.rmvb", NULL);
//        exit(0);
//    }
//    else{
//        wait(&status);
//    }
//
//    sleep(1);

    /*calculate Md5*/
//    string strMd5 = md5file("makefile");
    return 0;
}
