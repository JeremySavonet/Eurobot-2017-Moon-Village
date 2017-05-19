// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_SERVO_HPP_
#define WESTBOT_SERVO_HPP_

#include <QString>

#include "Hal.hpp"

#define SERV0_DISABLE_CONSIGN 9000

#define SERVO_0_ARM_R_CLOSED 31217
#define SERVO_0_ARM_R_OPEN 17590
#define SERVO_0_ARM_R_OPEN90 20000
#define SERVO_0_ARM_R_DROP 30275

#define SERVO_6_ARM_L_CLOSED 12713
#define SERVO_6_ARM_L_OPEN 29217
#define SERVO_6_ARM_L_OPEN90 23000
#define SERVO_6_ARM_L_DROP 14155

#define SERVO_7_EJECTOR_STANDBY 31217
#define SERVO_7_EJECTOR_EJECT 11533
#define SERVO_7_EJECTOR_FUSEE 14000

namespace WestBot {

class Servo
{
public:
    Servo( const QString& name );

    bool attach( Hal& hal,
                 uint8_t ioNumber,
                 uint16_t min,
                 uint16_t max );

    const QString& name() const;

    uint16_t read();
    void write( uint16_t position );

    void enable();
    void disable();

    bool isAttached() const;

private:
    const QString _name;

    ItemRegister::Ptr _servo;
    ItemRegister::Ptr _servoEnable;
    ItemRegister::Ptr _servoOverride;

    uint16_t _minAngle;
    uint16_t _maxAngle;
};

}

#endif // WESTBOT_SERVO_HPP_
