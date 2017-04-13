/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */

#ifndef _CONDITION_H_
#define _CONDITION_H_

#include <pthread.h>
#include "MutexLock.h"
#include "NonCopyable.h"

class Condition : public NonCopyable{
    public:
        Condition(MutexLock *mutexIter);
        ~Condition();

        void wait();
        void signal();
        void broadcast();

    private:
        pthread_cond_t _cond;
        MutexLock *_mutexIter;
};

#endif /*_CONDITION_H_*/
