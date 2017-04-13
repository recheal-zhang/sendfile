/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */

#ifndef _LOG_H_
#define _LOG_H_

#include <vector>
#include <string>

#include <stdlib.h>
#include <unistd.h>

#include "MutexLock.h"

class Log{
    public:
        static Log *getInstance();
        void writeLog(const char *logMsg,
                const char *file,
                int line,
                const char *date,
                const char *timer);

        std::vector<std::string>::size_type  getLogVecSize() const;
        void clearVecSize();
        static MutexLock _mutex;

        static std::vector<std::string> _logVec;
        static long long _logNum;

    private:
        Log();
        virtual ~Log(){}
        static Log *_loger;
       };

#endif /*_LOG_H_*/
