/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */
#include <iostream>

#include <unistd.h>

#include "LogThread.h"
#include "Log.h"
#include "DefineVal.h"
void LogThread::registerLogPermission(){
    logInstance = Log::getInstance();
}
void LogThread::run(){
    detach();
    while(true){
        sleep(LOGTHREADSLEEPTIME);
        long long logNum =
            static_cast<long long>(logInstance->getLogVecSize());
        logInstance->clearVecSize();

        long long qps = logNum / LOGTHREADSLEEPTIME;
        std::cout << "qps = " << qps << std::endl;
    }
}
