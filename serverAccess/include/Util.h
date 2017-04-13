/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */

#ifndef _UTIL_H_
#define _UTIL_H_

#include <string>
#include <sstream>
#include <iostream>

#include <fcntl.h>
#include <unistd.h>

class Util{
    public:
        //TODO: static xx yyy(){}
        //<<< Util::yyy();
        static void setNonblock(int fd){
            int flags = fcntl(fd, F_GETFL, 0);
            fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }

        static std::string writeLog(const char *logMsg,
                const char *file,
                const int &line,
                const char *date,
                const char *timer ){
            std::stringstream ss;
            ss << "[TIME]:" << timer << "\t"
                << logMsg << "\t"
                << "[FILE]:" << file << "\t"
                << "[LINE]:" << line << "\n";
            return ss.str();
        }

        static void writeMsgToSock(int fd, void *buf, size_t len){
            int nwrite;
            if((nwrite = write(fd, buf, len)) == -1){
#ifdef DEBUG
                std::cout << "write error in Util" << std::endl;
#endif /*DEBUG*/
            }
        }

};

#endif /*_UTIL_H_*/
