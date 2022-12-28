#include "mSockets/TCP/mClientTCP.h"

#include <iostream>



int main(){

    mClientTCP* mcTCP = new mClientTCP();

    mcTCP->mCreate("127.0.0.1", "8080");
    mcTCP->mConnect();

    mcTCP->mSend("Hello");
    mcTCP->mRecv();
    mcTCP->mClose();

    return 0;
}