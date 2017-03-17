// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_LAYER3_HPP_
#define WESTBOT_LAYER3_HPP_

namespace WestBot {

typedef struct Layer3
{
    uint32_t registers[ 64 ];
} __attribute__((packed)) Layer3_t;

}

#endif // WESTBOT_LAYER3_HPP_
