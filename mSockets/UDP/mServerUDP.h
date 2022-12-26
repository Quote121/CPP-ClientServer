#pragma once

#ifndef MSERVERUDP_H
#define MSERVERUDP_H

#include "../common.h"
#include "../networkEntity.h"

class mServerUDP : NetworkEntity{
private:
    int val;
public:
    bool mCreate() override;

    bool mRecv() override;

    bool mSend() override;

    bool mClose() override;

};

#endif // MSERVERUDP_H