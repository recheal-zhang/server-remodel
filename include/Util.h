/*
 * Copyright (C) riozhang
 * Copyright (C) tencent, Inc.
 * */

#ifndef _UTIL_H_
#define _UTIL_H_

#include <fcntl.h>

class Util{
    public:
        //TODO: static xx yyy(){}
        //<<< Util::yyy();
        static void setNonblock(int fd){
            int flags = fcntl(fd, F_GETFL, 0);
            fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }

};

#endif /*_UTIL_H_*/
