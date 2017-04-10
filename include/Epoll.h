/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */

#ifndef _EPOLL_H_
#define _EPOLL_H_

#include <sys/epoll.h>

#include "NonCopyable.h"
#include "DefineVal.h"
#include "SOCKAcceptor.h"

class Util;
class ThreadPool;
class SOCKAcceptor;
class Epoll : public NonCopyable{
    public:
        Epoll();
        ~Epoll();

        bool getThreadPoolInfo(ThreadPool *pool);
        void getSockAcceptorInfo(SOCKAcceptor *sockAcceptor);
        void monitor();

        void addEvent(const int &fd, const int &state);
        void deleteEvent(const int &fd, const int &state);
        void modifyEvent(const int &fd, const int &state);

        void handleEvents(int eventNum, int listenfd);
        void handleAccept(int listenfd);
    private:
        int _epollfd;
        struct epoll_event events[EPOLLEVENTS];
        char buf[RECVMAXSIZE];
        int _sockfd;
        SOCKAcceptor*_sockAcceptor;

        ThreadPool *_pool;
};

#endif /*_EPOLL_H_*/
