/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */

#ifndef _MUTEXLOCK_H_
#define _MUTEXLOCK_H_

#include <pthread.h>
#include "NonCopyable.h"

class Condition;

class MutexLock : public NonCopyable{
    public:
        MutexLock();
        ~MutexLock();

        void lock();
        void unlock();

    private:
        pthread_mutex_t _mutex;
        friend class Condition;
};

#endif /*_MUTEXLOCK_H_*/
