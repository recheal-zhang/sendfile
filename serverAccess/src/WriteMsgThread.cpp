/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */

#include "WriteMsgThread.h"
#include "Util.h"

MutexLock WriteMsgThread::_server2MsgMutex;
Condition WriteMsgThread::_server2MsgCond(
        &(WriteMsgThread::_server2MsgMutex));
int WriteMsgThread::_server2MsgCount = 1;

WriteMsgThread::WriteMsgThread() :
    _sendMsgQueue(),
    _mutex(),
    _cond(&_mutex)
{
}

WriteMsgThread::~WriteMsgThread(){}

void WriteMsgThread::addServer2MsgCount(){
    WriteMsgThread::_server2MsgMutex.lock();
    ++WriteMsgThread::_server2MsgCount;
    WriteMsgThread::_server2MsgCond.signal();
    WriteMsgThread::_server2MsgMutex.unlock();
}


void WriteMsgThread::subServer2MsgCount(){
    WriteMsgThread::_server2MsgMutex.lock();
    while(WriteMsgThread::_server2MsgCount <= 0){
        WriteMsgThread::_server2MsgCond.wait();
    }
    --WriteMsgThread::_server2MsgCount;
    WriteMsgThread::_server2MsgMutex.unlock();
}

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
    WriteMsgThread::subServer2MsgCount();
    while(_sendMsgQueue.empty()){
        _cond.wait();
    }
    msg = _sendMsgQueue.front();
    _sendMsgQueue.pop();
    _mutex.unlock();

    return msg;
}


