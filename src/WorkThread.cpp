/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */
#include <iostream>

#include <unistd.h>

#include "WorkThread.h"
#include "ThreadPool.h" // ?


void WorkThread::run(){
    detach();
    while(true){
        threadMsg msg;
        msg = _pool->getTaskFromQueue();

        int epollfd = msg.epollfd;
        int fd = msg.fd;
        char *buf = msg.buf;
        struct epoll_event event = msg.event;

        event.events = EPOLLOUT;
        epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);

        //core worker
//        int nwrite;
//        if((nwrite = write(fd, buf, sizeof(buf))) == -1){
//#ifdef DEBUG
//            std::cout << "write error" << std::endl;
//#endif /*DEBUG*/
//
//            //TODO: delete_event(epollfd, fd, EPOLLIN);
//            epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &event);
//        }
//
    }
}

bool WorkThread::registerThreadPool(ThreadPool *pool){
    if(!pool){
#ifdef DEBUG
        std::cout << "register thread pool error" << std::endl;
#endif /*DEBUG*/
        return false;
    }

    _pool = pool;
    return true;
}
