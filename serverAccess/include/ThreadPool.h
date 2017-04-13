/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */

#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include <vector>
#include <queue>

#include <sys/epoll.h>

#include "NonCopyable.h"
#include "WorkThread.h"
#include "MutexLock.h"
#include "Condition.h"
#include "DefineVal.h"

class Epoll;

class ThreadPool : public NonCopyable{
        friend class Epoll;
    public:
        ThreadPool(std::vector<WorkThread>::size_type);
        ~ThreadPool();

        void addTaskToQueue(const threadMsg &);
        threadMsg getTaskFromQueue();

        bool isPoolOpen() const;

        void start();
        //bool stop();

        std::vector<WorkThread>::size_type
            getThreadQuantity() const;

        std::queue<threadMsg> threadMsgQueue;

    private:
        std::vector<WorkThread>::size_type _threadNum;
        std::vector<WorkThread> _threadPool;
        std::queue<threadMsg> _threadMsgQueue;

        MutexLock _mutex;
        Condition _cond;
        bool _isPoolOpen;
};

#endif /*_THREADPOOL_H_*/
