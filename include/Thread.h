/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */

#ifndef _THREAD_H_
#define _THREAD_H_

#include <pthread.h>

class Thread{
    public:
        Thread() : _pid(pthread_self()){}
        virtual ~Thread(){}

        void start();
        void detach();
        void join();

        static void *thread_func(void *);

        virtual void run() = 0;
        pthread_t getPid() const;

    protected:
        pthread_t _pid;
};

#endif /*_THREAD_H_*/
