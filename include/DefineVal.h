/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */

#ifndef _DEFINEVAL_H_
#define _DEFINEVAL_H_

#include <sys/epoll.h>

#define INVALID_SOCKFD_VALUE 0

#define IPADDRESS "127.0.0.1"
#define PORT 124
#define RECVMAXSIZE 1024
#define LISTENQ 5
#define FDSIZE 1000
#define EPOLLEVENTS 100
#define MAXTHREADNUM 10
#define TIMEOUT 500

#define SVRADDRESS "127.0.0.1"
#define SVRPORT 126

#define LOGTHREADSLEEPTIME 20

struct clientMsg{
    int seqId;
    int clientAcceptFd;
    int cost;
    //TODO: should extend
};

struct serverProcessMsg{
    int serverConnectFd;
    int count;
};

struct threadMsg{
    int epollfd;
//    char *buf;
    struct clientMsg cliMsg;
    struct serverProcessMsg svrProMsg;

    bool isClientQuery;
    struct epoll_event event;
};


#endif /*_DEFINEVAL_H_*/


