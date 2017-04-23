// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <QDebug>
#include <QThread>

#include <WestBot/TrajectoryManager.hpp>

using namespace WestBot;

TrajectoryManager::TrajectoryManager( Hal& hal )
    : _hal( hal )
{
}

TrajectoryManager::~TrajectoryManager()
{
}

void TrajectoryManager::init()
{
    uint8_t commandId = _hal._trajOutAck.read< uint8_t >();

    _hal._trajCmdValid.write( 0x0 );
    _hal._trajCmdId.write( commandId++ );
    _hal._trajCmdType.write( CMD_TYPE_TRAJ );
    _hal._trajCmdOrderType.write( TRAJ_DISABLE );
    _hal._trajCmdValid.write( 0x1 );

    while( _hal._trajOutAck.read< uint8_t >() != commandId )
    {
       QThread::msleep( 10 );
       qDebug() << "wait cmd ack";
    }

    _hal._trajCmdValid.write( 0x0 );
    _hal._trajCmdId.write( commandId++ );
    _hal._trajCmdType.write( CMD_TYPE_CFG_WND );
    _hal._trajCmdWndDistance.write( ( float ) 10.0 );
    _hal._trajCmdWndAngleDeg.write( ( float ) 2.0 );
    _hal._trajCmdWndAngleStartDeg.write( ( float ) 10.0 );
    _hal._trajCmdValid.write( 0x1 );

    while( _hal._trajOutAck.read< uint8_t >() != commandId )
    {
       QThread::msleep( 2000 );
       qDebug() << "wait cmd ack " << _hal._trajOutAck.read< uint8_t >();
    }

    _hal._trajCmdValid.write( 0x0 );
    _hal._trajCmdId.write( commandId++ );
    _hal._trajCmdType.write( CMD_TYPE_CFG_DISTANCE );
    _hal._trajCmdCfgSpeed.write( ( float ) 0.15 );
    _hal._trajCmdCfgAcc.write( ( float ) 0.00008 );
    _hal._trajCmdValid.write( 0x1 );

    while( _hal._trajOutAck.read< uint8_t >() != commandId )
    {
       QThread::msleep( 2000 );
       qDebug() << "wait cmd ack " << _hal._trajOutAck.read< uint8_t >();
    }

    _hal._trajCmdValid.write( 0x0 );
    _hal._trajCmdId.write( commandId++ );
    _hal._trajCmdType.write( CMD_TYPE_CFG_ANGLE );
    _hal._trajCmdCfgSpeed.write( ( float ) 0.0008 );
    _hal._trajCmdCfgAcc.write( ( float) 0.0000004 );
    _hal._trajCmdValid.write( 0x1 );

    while( _hal._trajOutAck.read< uint8_t >() != commandId )
    {
       QThread::msleep( 10 );
       qDebug() << "wait cmd ack";
    }

    _hal._trajCmdValid.write( 0x0 );
    _hal._trajCmdId.write( commandId++ );
    _hal._trajCmdType.write( CMD_TYPE_TRAJ );
    _hal._trajCmdOrderType.write( TRAJ_ENABLE );
    _hal._trajCmdValid.write( 0x1 );

    while( _hal._trajOutAck.read< uint8_t >() != commandId )
    {
       QThread::msleep( 10 );
       qDebug() << "wait cmd ack";
    }

    qDebug() << "Trajectory manager successfully initialized";
}
