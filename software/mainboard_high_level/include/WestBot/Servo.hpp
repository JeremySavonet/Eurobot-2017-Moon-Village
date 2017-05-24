// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_SERVO_HPP_
#define WESTBOT_SERVO_HPP_

#include <QString>

#include "Hal.hpp"

#define SERV0_DISABLE_CONSIGN (9000)

#define SERVO_0_ARM_R_CLOSED (30100) // forcage du cylindre pour la detection couleur
#define SERVO_0_ARM_R_0 (29800) // angle à 0
#define SERVO_0_ARM_R_DROP (28500) // position ejection
#define SERVO_0_ARM_R_OPEN45 (25000) // ouvert à 45° (utile après ejection car 90° trop large)
#define SERVO_0_ARM_R_OPEN90 (22400) // ouvert à 90°
#define SERVO_0_ARM_R_OPEN180 (18000) // /!ATTENTION!\ Il faut désactiver le servo ensuite car il ne va pas à 180°


#define SERVO_6_ARM_L_CLOSED (12800) // forcage du cylindre pour la detection couleur
#define SERVO_6_ARM_L_0 (13800) // angle à 0
#define SERVO_6_ARM_L_DROP (15000) // position d'ejection (entre 0 & 45 °)
#define SERVO_6_ARM_L_OPEN45 (20200) // ouvert à 45° (utile après ejection car 90° trop large)
#define SERVO_6_ARM_L_OPEN90 (22900) // ouvert à 90°
#define SERVO_6_ARM_L_OPEN180 (30500) // FULL rentré en sécurité

#define SERVO_7_EJECTOR_STANDBY (31517)
#define SERVO_7_EJECTOR_EJECT (11533)

#define SERVO_1_UNBLOCK_STANDBY (26100)
#define SERVO_1_UNBLOCK_ACTION (18300)

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
