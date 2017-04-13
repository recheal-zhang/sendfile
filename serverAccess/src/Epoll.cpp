/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */

#include <iostream>

#include <string.h>
#include <unistd.h>

#include "Epoll.h"
#include "Util.h"
#include "ThreadPool.h"
#include "INETAddr.h"
#include "SockConnector.h"

#include "Log.h"

Epoll::Epoll():
    _epollfd(epoll_create(FDSIZE))
{
}

Epoll::~Epoll(){
}

int Epoll::getEpollfd() const{
    return _epollfd;
}


void Epoll::getSockAcceptorInfo(SOCKAcceptor *sockAcceptor){
    _sockAcceptor = sockAcceptor;
    addEvent(_sockAcceptor->_sockfd, EPOLLIN | EPOLLET);
    _sockfd = _sockAcceptor->_sockfd;

    memset(buf, 0, RECVMAXSIZE);
}
bool Epoll::getThreadPoolInfo(ThreadPool *pool){
    if(!pool){
#ifdef DEBUG
        std::cout << "thread pool invalid" << std::endl;
#endif /*DEBUG*/
        return false;
    }
    _pool = pool;
    return true;
}

void Epoll::monitor(){
    int ret = 0;
    while(true){
        ret = epoll_wait(_epollfd, events, EPOLLEVENTS, TIMEOUT);
        handleEvents(ret, _sockfd);
    }
}

void Epoll::handleEvents(int eventNum, int listenfd){
    int fd;
    for(int i = 0; i < eventNum; i++){
        fd = events[i].data.fd;

        if((fd == listenfd) && (events[i].events & EPOLLIN)){
            handleAccept(listenfd);
        }

        else if(events[i].events & EPOLLIN){
            //TODO: add query num
            int nread;

            //----------------------------------------
            //if the msg come from server2
            //TODO: Md5
            if(fd == SockConnector::_sockfd){
                //send it to thread which send msg to client
                if((nread = read(fd, &_threadMsg, sizeof(_threadMsg))) < 0){
#ifdef DEBUG
                    std::cout << "read error from server2 "  << std::endl;
#endif /*DEBUG*/
                    close(fd);
                    deleteEvent(fd, EPOLLIN);
                }
                else if(nread == 0){
                    close(fd);
                    deleteEvent(fd, EPOLLIN);
                }
                else{
                    struct threadMsg server2Msg;
                    //TODO: push it to queue which
                    //the send thread controls
                }
            }
            else{//if the msg come from client
                //struct it and send it to server2
                if((nread = read(fd, buf, RECVMAXSIZE)) < 0){
#ifdef DEBUG
                    std::cout << "read error from client" << std::endl;
#endif /*DEBUG*/
                    close(fd);
                    deleteEvent(fd, EPOLLIN);
                }
                else if(nread == 0){//same as EPOLLRDHUP
                    close(fd);
                    deleteEvent(fd, EPOLLIN);
                }
                else{
                    //TODO:struct threadMsg and push it to task queue
                    struct threadMsg tempMsg;
                    std::string msgStr = std::string(buf);
                    tempMsg.epollfd = _epollfd;
                    tempMsg.cliMsg.clientAcceptFd = fd;
                    memcpy(tempMsg.cliMsg.msg, msgStr.c_str(), RECVMAXSIZE);
                    tempMsg.svrProMsg.serverConnectFd
                        = SockConnector::_sockfd;
//                   tempMsg.svrProMsg.md5Result = true;
                    tempMsg.svrProMsg.serverMd5Result = true;
                    tempMsg.event = events[i];

                    _pool->addTaskToQueue(tempMsg);

                    modifyEvent(fd, EPOLLOUT);  //send ack to client
                }
            }
        }

        else if(events[i].events & EPOLLOUT){
            //TODO: buf is NULL
            int nwrite;
            std::string ack = "7E457E";
            if((nwrite = write(fd, ack.c_str(), ack.size())) < 0){
#ifdef DEBUG
                std::cout << "write error in epoll" << std::endl;
#endif /*DEBUG*/
                deleteEvent(fd, EPOLLOUT);
                close(fd);
            }
            else{
                modifyEvent(fd, EPOLLIN);
            }
        }

        else if((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)){
#ifdef DEBUG
            std::cout << "epoll error" << std::endl;
#endif /*DEBUG*/
            close(fd);
        }
    }
}

void Epoll::handleAccept(int listenfd){
    int clifd = _sockAcceptor->cliAccept();
    if(clifd == -1){
#ifdef DEBUG
        std::cout << "accept error" << std::endl;
#endif /*DEBUG*/
        return ;
    }

#ifdef NONBLOCK
    Util::setNonblock(clifd);
#endif /*NONBLOCK*/

    addEvent(clifd, EPOLLIN | EPOLLET);
}

void Epoll::addEvent(const int &fd, const int &state){
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &ev);
}

void Epoll::deleteEvent(const int &fd, const int &state){
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, &ev);
}

void Epoll::modifyEvent(const int &fd, const int &state){
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(_epollfd, EPOLL_CTL_MOD, fd, &ev);
}



