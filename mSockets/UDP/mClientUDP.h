#pragma once

#ifndef MCLIENTUDP_H
#define MCLIENTUDP_H

#include "../common.h"
#include "../networkEntity.h"

class mClientUDP : NetworkEntity{
private:
public:
    bool mCreate() override;

    bool mRecv() override;

    bool mSend() override;

    bool mClose() override;

};


#endif // MCLIENTUDP_H