// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_LAYER1_HPP_
#define WESTBOT_LAYER1_HPP_

namespace WestBot {

typedef struct Layer1
{
    uint32_t registers[ 64 ];
} __attribute__((packed)) Layer1_t;

}

#endif // WESTBOT_LAYER1_HPP_
