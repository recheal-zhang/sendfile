/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */

#ifndef _LOGTHREAD_H_
#define _LOGTHREAD_H_

#include "Thread.h"

class Log;
class LogThread : public Thread{
    public:
        void registerLogPermission();
        void run();

    private:
        Log *logInstance;
};

#endif /*_LOGTHREAD_H_*/
