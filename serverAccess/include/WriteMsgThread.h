/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */

#ifndef _WRITEMSGTHREAD_H_
#define _WRITEMSGTHREAD_H_

#include <queue>

#include "Thread.h"
#include "DefineVal.h"
#include "MutexLock.h"
#include "Condition.h"

class WriteMsgThread : public Thread{
    public:
        WriteMsgThread();
        ~WriteMsgThread();
        void run();

        void addMsgToWriteMsgQueue(const threadMsg &);
        threadMsg getMsgFromWriteMsgQueue();

        std::queue<threadMsg> _sendMsgQueue;

    private:
        MutexLock _mutex;
        Condition _cond;
};

#endif /*_WRITEMSGTHREAD_H_*/
