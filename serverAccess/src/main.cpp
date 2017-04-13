#include <iostream>
#include <sys/epoll.h>
#include "Epoll.h"
#include "ThreadPool.h"
#include "DefineVal.h"
#include "INETAddr.h"
#include "SOCKAcceptor.h"
#include "LogThread.h"
#include "SockConnector.h"

using namespace std;

int main(){
    cout << "hello world" << endl;

    SockConnector *sockConnectorInstance =
        SockConnector::getInstance();
    sockConnectorInstance->sockConnect(
            SVRADDRESS,
            SVRPORT);

    ThreadPool threadPool(MAXTHREADNUM);
    threadPool.start();

    LogThread logThread;
    logThread.registerLogPermission();
    logThread.start();

    INETAddr _inetAddr(IPADDRESS, PORT);
    SOCKAcceptor sockAcceptor;
    //sockAcceptor.bindListen(_inetAddr);
    sockAcceptor.sockBindListen();
    Epoll epollContrller;
    epollContrller.getSockAcceptorInfo(&sockAcceptor);
    epollContrller.getThreadPoolInfo(&threadPool);

    //set read in epoll
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = sockConnectorInstance->_sockfd;
    epoll_ctl(epollContrller.getEpollfd(),
            EPOLL_CTL_ADD,
            sockConnectorInstance->_sockfd,
            &ev);

    epollContrller.monitor();

    return 0;
}
