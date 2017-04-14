/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */
#include <iostream>

#include <string.h>

#include "ThreadPool.h"

ThreadPool::ThreadPool(std::vector<WorkThread>::size_type num) :
    _threadNum(num),
    _threadPool(_threadNum), //vector initialize
    _threadMsgQueue(),
    _mutex(),               //auto initialize
    _cond(&_mutex),         //use mutex to initialize it
    _isPoolOpen(false)
{
    for(std::vector<WorkThread>::iterator iter = _threadPool.begin();
            iter != _threadPool.end(); iter++){
        iter->registerThreadPool(this);
    }
}

ThreadPool::~ThreadPool(){
    //TODO: add stop() if no detach
    while(!_threadMsgQueue.empty()){
        _threadMsgQueue.pop();
    }
}

void ThreadPool::getSendMsgThreadInfo(
        WriteMsgThread *writeMsgThread){
    if(!writeMsgThread){
#ifdef DEBUG
        std::cout << "sendMsgThread invalid" << std::endl;
#endif /*DEBUG*/
    }
    _writeMsgThread = writeMsgThread;
}

void ThreadPool::addTaskToQueue(const threadMsg &msg){
    _mutex.lock();
#ifdef SHOWMSG
//    char dst[msg.cliMsg.length];
//    memcpy(dst, msg.cliMsg.msg, msg.cliMsg.length);
    std::cout << msg.cliMsg.msg <<std::endl;
#endif /*SHOWMSG*/
    _threadMsgQueue.push(msg);
    _cond.signal();
    _mutex.unlock();
}


threadMsg ThreadPool::getTaskFromQueue(){
    _mutex.lock();
    while(_threadMsgQueue.empty()){
        _cond.wait();
    }

    threadMsg msg;
    msg = _threadMsgQueue.front();
    _threadMsgQueue.pop();

    _mutex.unlock();


    return msg;
}

bool ThreadPool::isPoolOpen() const{
    return _isPoolOpen;
}

void ThreadPool::start(){
    if(isPoolOpen()){
        return;
    }

    _isPoolOpen = true;
    for(std::vector<WorkThread>::iterator iter = _threadPool.begin();
            iter != _threadPool.end(); iter++){
        iter->start();
    }
}

std::vector<WorkThread>::size_type
ThreadPool::getThreadQuantity() const{
    return _threadPool.size();
}
