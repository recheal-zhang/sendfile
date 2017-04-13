/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */
#include <iostream>

#include "Thread.h"

Thread::Thread() : _pid(pthread_self()){
#ifdef SEETHREADID
    std::cout << "thread id in Thread() :"
        << _pid << std::endl;
#endif /*SEETHREADID*/
}

void Thread::start(){
    pthread_create(&_pid, NULL, Thread::thread_func, this);

#ifdef SEETHREADID
    std::cout << "thread id after start :"
        << _pid << std::endl;
#endif /*SEETHREADID*/
}

void Thread::join(){
    pthread_join(_pid, NULL);
}

void Thread::detach(){
    pthread_detach(_pid);
}

void *Thread::thread_func(void *arg){
    Thread *pThread = static_cast<Thread*>(arg);
    //dynamic binding
    pThread->run();
    return NULL;
}

pthread_t Thread::getPthreadSelf() const{
    return _pid;
}
