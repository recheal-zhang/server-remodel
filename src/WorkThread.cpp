/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */
#include <iostream>

#include <unistd.h>
#include <sys/epoll.h>

#include "WorkThread.h"
#include "ThreadPool.h" // ?
#include "SockConnector.h"
#include "Util.h"


void WorkThread::run(){
    detach();
    while(true){
        threadMsg msg;
        msg = _pool->getTaskFromQueue();

//        int epollfd = msg.epollfd;
//        int fd = msg.fd;
//        char *buf = msg.buf;
//        struct epoll_event event = msg.event;
//
//        event.events = EPOLLOUT;
//        epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);

        if(msg.isClientQuery == true){//come from client
            msg.isClientQuery = false;
            //TODO:send to server2
            int server2fd = msg.svrProMsg.serverConnectFd;
            //TODO: write
            Util::writeMsgToSock(server2fd,
                    &msg,
                    sizeof(msg));
        }
        else{//come from server2
            //TODO:send to server2 & send to client if 10
            int server2fd = msg.svrProMsg.serverConnectFd;
            int count = msg.svrProMsg.count;
            if(count == 10){//send msg to client
                Util::writeMsgToSock(msg.cliMsg.clientAcceptFd,
                        &msg,
                        sizeof(msg));
            }else if(count < 10){//send msg to server2
                Util::writeMsgToSock(
                        server2fd,
                        &msg,
                        sizeof(msg));
            }
        }

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
