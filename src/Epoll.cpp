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
            if((nread = read(fd, &_threadMsg, sizeof(_threadMsg))) < 0){
#ifdef DEBUG
                std::cout << "read error" << std::endl;
#endif /*DEBUG*/
                deleteEvent(fd, EPOLLIN);
                close(fd);
            }
            else if(nread == 0){ //same as EPOLLRDHUP
                deleteEvent(fd, EPOLLIN);
                close(fd);
            }
            else{
                struct threadMsg tempMsg;
                if(_threadMsg.isClientQuery == true){
                     //TODO: if buf should be msg buf, write log
                     //client come in
                    Log *logInstance = Log::getInstance();
                    logInstance->writeLog(buf,
                            __FILE__,
                            __LINE__,
                            __DATE__,
                            __TIME__);
                    tempMsg.cliMsg.seqId = _threadMsg.cliMsg.seqId;
                    tempMsg.cliMsg.clientAcceptFd = fd;
                    tempMsg.cliMsg.cost = _threadMsg.cliMsg.cost;

                    //TODO: add server2
                    tempMsg.svrProMsg.serverConnectFd = SockConnector::_sockfd;
                    tempMsg.svrProMsg.count = 1;

                    tempMsg.isClientQuery = true;
                }
                else{//come from server2
                    tempMsg = _threadMsg;
                }
               //                std::cout << "logVecSize=" <<
//                    logInstance->getLogVecSize() <<
//                    std::endl;

                tempMsg.epollfd = _epollfd;
                tempMsg.event = events[i];

                _pool->addTaskToQueue(tempMsg);
            }
        }

        else if(events[i].events & EPOLLOUT){
            //TODO: buf is NULL
            int nwrite;
            if((nwrite = write(fd, buf, strlen(buf))) < 0){
#ifdef DEBUG
                std::cout << "write error" << std::endl;
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



