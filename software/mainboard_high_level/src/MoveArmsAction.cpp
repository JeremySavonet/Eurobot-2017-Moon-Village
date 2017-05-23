// Copyright (c) 2017 All Rights Reserved WestBot

#include <QThread>

#include <WestBot/MoveArmsAction.hpp>

using namespace WestBot;

MoveArmsAction::MoveArmsAction(
     Servo& armRight,
     Servo& armLeft,
     Servo& ejector,
     Servo& unblock,
     Position position )
    : Action()
    , _armRight( armRight )
    , _armLeft( armLeft )
    , _ejector( ejector )
    , _unblock( unblock )
    , _position( position )
{
}

void MoveArmsAction::execute()
{
    switch( _position )
    {
    case Position::CLOSED:
    {
        _armRight.write( SERVO_0_ARM_R_CLOSED );
        _armLeft.write( SERVO_6_ARM_L_CLOSED );
    }
        break;

    case Position::ZERO_POS:
    {
        _armRight.write( SERVO_0_ARM_R_0 );
        _armLeft.write( SERVO_6_ARM_L_0 );
    }
        break;

    case Position::DROP:
    {
        _armRight.write( SERVO_0_ARM_R_DROP );
        _armLeft.write( SERVO_6_ARM_L_DROP );
    }
        break;

    case Position::OPEN_90:
    {
        _armRight.write( SERVO_0_ARM_R_OPEN90 );
        _armLeft.write( SERVO_6_ARM_L_OPEN90 );
    }
        break;

    case Position::OPEN_45:
    {
        _armRight.write( SERVO_0_ARM_R_OPEN45 );
        _armLeft.write( SERVO_6_ARM_L_OPEN45 );
    }
        break;

    case Position::OPEN_180:
    {
        _armRight.write( SERVO_0_ARM_R_OPEN180 );
        _armLeft.write( SERVO_6_ARM_L_OPEN180 );
    }
        break;

    case Position::OPEN_FUSEE:
    {
        _ejector.write( SERVO_7_EJECTOR_STANDBY );
        QThread::msleep( 250 );

        // Open arms full
        _armRight.write( SERVO_0_ARM_R_OPEN180 );
        _armLeft.write( SERVO_6_ARM_L_OPEN180 );

        QThread::msleep( 250 );

        // Disable arms
        _armLeft.write( SERV0_DISABLE_CONSIGN );
        _armRight.write( SERV0_DISABLE_CONSIGN );
    }
        break;

    case Position::DISABLE:
    {
        _armLeft.write( SERV0_DISABLE_CONSIGN );
        _armRight.write( SERV0_DISABLE_CONSIGN );
        _ejector.write( SERV0_DISABLE_CONSIGN );
    }
        break;

    case Position::EJECT:
    {
        _armRight.write( SERVO_0_ARM_R_DROP );
        _armLeft.write( SERVO_6_ARM_L_DROP );

        QThread::msleep( 200 );

        _ejector.write( SERVO_7_EJECTOR_EJECT );

        QThread::msleep( 250 );

        _armRight.write( SERVO_0_ARM_R_OPEN45 );
        _armLeft.write( SERVO_6_ARM_L_OPEN45 );
        QThread::msleep( 500 );

        _ejector.write( SERVO_7_EJECTOR_STANDBY );
        QThread::msleep( 500 );

        _armLeft.write( SERV0_DISABLE_CONSIGN );
        _armRight.write( SERV0_DISABLE_CONSIGN );
        _ejector.write( SERV0_DISABLE_CONSIGN );
    }
        break;

    case Position::UNBLOCK:
    {
        _unblock.write( SERVO_1_UNBLOCK_ACTION );
    }
        break;

    case Position::STANDBY_UNBLOCK:
    {
        _unblock.write( SERVO_1_UNBLOCK_STANDBY );
    }
        break;

    }

    emit complete();
}
