#include <iostream>

#include "Epoll.h"
#include "ThreadPool.h"
#include "DefineVal.h"
#include "INETAddr.h"
#include "SOCKAcceptor.h"
#include "LogThread.h"


using namespace std;

int main(){
    cout << "hello world" << endl;

    ThreadPool threadPool(MAXTHREADNUM);
    threadPool.start();

    LogThread logThread;
    logThread.registerLogPermission();
    logThread.start();

    INETAddr _inetAddr(IPADDRESS, PORT);
    SOCKAcceptor sockAcceptor;
    //sockAcceptor.bindListen(_inetAddr);
    sockAcceptor.sockBindListen();
    Epoll epollContrller;
    epollContrller.getSockAcceptorInfo(&sockAcceptor);
    epollContrller.getThreadPoolInfo(&threadPool);

    epollContrller.monitor();

    return 0;
}
