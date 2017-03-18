// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_LAYER_HPP_
#define WESTBOT_LAYER_HPP_

namespace WestBot {

typedef struct Layer
{
    uint32_t registers[ 64 ];
} __attribute__((packed)) Layer_t;

}

#endif // WESTBOT_LAYER_HPP_
