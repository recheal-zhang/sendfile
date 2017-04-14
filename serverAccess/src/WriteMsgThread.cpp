/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */

#include "WriteMsgThread.h"
#include "Util.h"

WriteMsgThread::WriteMsgThread() :
    _sendMsgQueue(),
    _mutex(),
    _cond(&_mutex)
{
}

WriteMsgThread::~WriteMsgThread(){}

void WriteMsgThread::run(){
    detach();
    while(true){
        threadMsg msg;
        msg = getMsgFromWriteMsgQueue();

        //TODO:send to server2
        int server2fd = msg.svrProMsg.serverConnectFd;
        Util::writeMsgToSock(server2fd, &msg, sizeof(msg));
    }
}

void WriteMsgThread::addMsgToWriteMsgQueue(const threadMsg &msg){
    _mutex.lock();
    _sendMsgQueue.push(msg);
    _cond.signal();
    _mutex.unlock();
}

threadMsg WriteMsgThread::getMsgFromWriteMsgQueue(){
    threadMsg msg;
    _mutex.lock();
    while(_sendMsgQueue.empty()){
        _cond.wait();
    }
    msg = _sendMsgQueue.front();
    _sendMsgQueue.pop();
    _mutex.unlock();

    return msg;
}


