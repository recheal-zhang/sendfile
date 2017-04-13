/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */

#ifndef _WORKTHREAD_H_
#define _WORKTHREAD_H_

#include "Thread.h"

class ThreadPool;

class WorkThread : public Thread{
    public:
        void run();
        bool registerThreadPool(ThreadPool *pool);

    private:
        ThreadPool *_pool;
};


#endif /*_WORKTHREAD_H_*/
