/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */


#include "Condition.h"

Condition::Condition(MutexLock *mutexIter) :
        _mutexIter(mutexIter){
    pthread_cond_init(&_cond, NULL);
}

Condition::~Condition(){
    pthread_cond_destroy(&_cond);
}

void Condition::wait(){
    pthread_cond_wait(&_cond,
            &(_mutexIter->_mutex));
}

void Condition::signal(){
    pthread_cond_signal(&_cond);
}

void Condition::broadcast(){
    pthread_cond_broadcast(&_cond);
}

