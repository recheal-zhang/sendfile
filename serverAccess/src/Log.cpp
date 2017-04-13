/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */

#include "Log.h"
#include "Util.h"

Log *Log::_loger = new Log();
MutexLock Log::_mutex;
std::vector<std::string> Log::_logVec;
long long Log::_logNum = 0;

Log::Log(){}


Log *Log::getInstance(){
    if(_loger == NULL){
        Log::_mutex.lock();
        if(_loger == NULL){
            _loger = new Log();
        }
        Log::_mutex.unlock();
    }
    return _loger;
}
std::vector<std::string>::size_type Log::getLogVecSize() const{
    return Log::_logNum;
//    return Log::_logVec.size();
}
void Log::writeLog(const char *logMsg,
                const char *file = __FILE__,
                int line = __LINE__,
                const char *date = __DATE__,
                const char *timer = __TIME__){
    Log::_mutex.lock();
    Log::_logNum = Log::_logNum + 1;
//    Log::_logVec.push_back(Util::writeLog(logMsg,
//                file, line, date, timer));
    Log::_mutex.unlock();
}

void Log::clearVecSize(){
    Log:: _mutex.lock();
    Log::_logNum = 0;
//    Log::_logVec.clear();
    Log::_mutex.unlock();
}

