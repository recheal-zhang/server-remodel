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

Epoll::Epoll():
    _epollfd(epoll_create(FDSIZE))
{
}

Epoll::~Epoll(){
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
            if((nread = read(fd, buf, RECVMAXSIZE)) < 0){
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
                threadMsg msg;
                msg.epollfd = _epollfd;
                msg.fd = fd;
                msg.buf = buf;
                msg.event = events[i];

                _pool->addTaskToQueue(msg);
            }
        }

        else if(events[i].events & EPOLLOUT){
            //TODO: buf is NULL
        }

        else if(events[i].events & EPOLLERR){
            //TODO: how?
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



