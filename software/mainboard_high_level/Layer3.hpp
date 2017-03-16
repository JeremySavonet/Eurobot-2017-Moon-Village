// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_LAYER3_HPP_
#define WESTBOT_LAYER3_HPP_

namespace WestBot {

typedef struct
{
    uint32_t registers[ 64 ];
} __attribute__((packed)) Layer3;

}

#endif // WESTBOT_LAYER3_HPP_
