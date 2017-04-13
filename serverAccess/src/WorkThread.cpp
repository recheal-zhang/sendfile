/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */
#include <iostream>
#include <stdlib.h>

#include <unistd.h>
#include <sys/epoll.h>
#include <pthread.h>

#include "WorkThread.h"
#include "ThreadPool.h" // ?
#include "SockConnector.h"
#include "Util.h"


void WorkThread::run(){
    detach();
    while(true){
        threadMsg msg;
        msg = _pool->getTaskFromQueue();

        int server2fd = msg.svrProMsg.serverConnectFd;
        //transmit the msg from client to server2
        Util::writeMsgToSock(server2fd, &msg, sizeof(threadMsg));

        //TODO: Acknowlegement Control
    }
}

bool WorkThread::registerThreadPool(ThreadPool *pool){
    if(!pool){
#ifdef DEBUG
        std::cout << "register thread pool error" << std::endl;
#endif /*DEBUG*/
        return false;
    }

#ifdef SEETHREADID
    std::cout << "thread ID = " << pthread_self() << std::endl;
    std::cout << "pid = " << getpid() << std::endl;
#endif /*SEETHREADID*/

    _pool = pool;
    return true;
}
