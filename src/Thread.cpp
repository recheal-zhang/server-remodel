/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */

#include "Thread.h"

void Thread::start(){
    pthread_create(&_pid, NULL, Thread::thread_func, this);
}

void Thread::join(){
    pthread_join(_pid, NULL);
}

void Thread::detach(){
    pthread_detach(_pid);
}

void *Thread::thread_func(void *arg){
    Thread *pThread = static_cast<Thread*>(arg);
    //dynamic binding
    pThread->run();
    return NULL;
}

pthread_t Thread::getPid() const{
    return _pid;
}
