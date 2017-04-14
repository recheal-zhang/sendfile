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

        static void addServer2MsgCount();
        static void subServer2MsgCount();

        std::queue<threadMsg> _sendMsgQueue;

    private:
        MutexLock _mutex;
        Condition _cond;
        static MutexLock _server2MsgMutex;
        static Condition _server2MsgCond;
        static int _server2MsgCount;
};

#endif /*_WRITEMSGTHREAD_H_*/
