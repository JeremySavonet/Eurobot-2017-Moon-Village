// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include "../../include/WestBot/Robot/AngleDistance.hpp"

using namespace WestBot;

void AngleDistance::rsGetPolarFromWheels( struct RsPolar* p_dst,
                                          struct RsWheels* w_src )
{
    p_dst->distance = ( w_src->right + w_src->left ) / 2;
    p_dst->angle = ( w_src->right - w_src->left ) / 2;
}

void AngleDistance::rsGetWheelsFromPolar( struct RsWheels* w_dst,
                                          struct RsPolar* p_src )
{
    w_dst->left = p_src->distance - p_src->angle;
    w_dst->right = p_src->distance + p_src->angle;
}
