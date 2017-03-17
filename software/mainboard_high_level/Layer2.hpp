// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_LAYER2_HPP_
#define WESTBOT_LAYER2_HPP_

namespace WestBot {

typedef struct Layer2
{
    uint32_t registers[ 64 ];
} __attribute__((packed)) Layer2_t;

}

#endif // WESTBOT_LAYER2_HPP_
