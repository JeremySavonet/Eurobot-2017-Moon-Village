// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_SYSTEM_HPP_
#define WESTBOT_SYSTEM_HPP_

namespace WestBot {

typedef struct Layer1Config
{
    bool simulationMode;
} Layer1Config_t;

typedef struct Layer2Config
{
   uint16_t odometryConfiguration;

   float wheelAxe;
   float wheelDiameterLeft;
   float wheelDiameterRight;

   float codingWheelAxe;
   float codingWheelDiameterLeft;
   float codingWheelDiameterRight;

    uint16_t distancePeriodPid;
    uint16_t distanceEnable;
    float pidDistanceKp;
    float pidDistanceKi;
    float pidDistanceKd;
    float distanceSpeed;
    float distanceAcc;
    uint32_t distanceOutputSaturation;
    float distanceCurrentTarget;

    uint16_t anglePeriodPid;
    uint16_t angleEnable;
    float pidAngleKp;
    float pidAngleKi;
    float pidAngleKd;
    float angleSpeed;
    float angleAcc;
    uint32_t angleOutputSaturation;
    float angleCurrentTarget;

    uint16_t carrouselPeriodPid;
    uint16_t carrouselEnable;
    float pidCarrouselKp;
    float pidCarrouselKi;
    float pidCarrouselKd;
    float carrouselSpeed;
    float carrouselAcc;
    uint32_t carrouselOutputSaturation;
    float carrouselCurrentTarget;
} Layer2Config_t;

typedef struct Layer3Config
{
    uint8_t systemMode;
    uint8_t positionManagerConfiguration;
    uint8_t positionManagerEnable;
} Layer3Config_t;

}

#endif // WESTBOT_SYSTEM_HPP_
