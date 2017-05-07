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
    _hal._trajFreqHz.write( 10 );

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

void TrajectoryManager::disable()
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
       qDebug() << "wait disable ack";
    }
}

void TrajectoryManager::enable()
{
    uint8_t commandId = _hal._trajOutAck.read< uint8_t >();

    _hal._trajCmdValid.write( 0x0 );
    _hal._trajCmdId.write( commandId++ );
    _hal._trajCmdType.write( CMD_TYPE_TRAJ );
    _hal._trajCmdOrderType.write( TRAJ_ENABLE );
    _hal._trajCmdValid.write( 0x1 );

    while( _hal._trajOutAck.read< uint8_t >() != commandId )
    {
       QThread::msleep( 10 );
       qDebug() << "wait enable ack";
    }
}

void TrajectoryManager::stop()
{
    uint8_t commandId = _hal._trajOutAck.read< uint8_t >();

    _hal._trajCmdValid.write( 0x0 );
    _hal._trajCmdId.write( commandId++ );
    _hal._trajCmdType.write( CMD_TYPE_TRAJ );
    _hal._trajCmdOrderType.write( TRAJ_STOP );
    _hal._trajCmdValid.write( 0x1 );

    while( _hal._trajOutAck.read< uint8_t >() != commandId )
    {
       QThread::msleep( 10 );
       qDebug() << "wait stop ack";
    }
}

void TrajectoryManager::hardStop()
{
    uint8_t commandId = _hal._trajOutAck.read< uint8_t >();

    _hal._trajCmdValid.write( 0x0 );
    _hal._trajCmdId.write( commandId++ );
    _hal._trajCmdType.write( CMD_TYPE_TRAJ );
    _hal._trajCmdOrderType.write( TRAJ_HARDSTOP );
    _hal._trajCmdValid.write( 0x1 );

    while( _hal._trajOutAck.read< uint8_t >() != commandId )
    {
       QThread::msleep( 10 );
       qDebug() << "wait hardstop ack";
    }
}

// Trajectories: all this method are blocking
void TrajectoryManager::moveDRel( float distance )
{
    uint8_t inWindow;
    uint8_t commandId = hal._trajOutAck.read< uint8_t >();

    hal._trajCmdValid.write( 0x0 );
    hal._trajCmdId.write( commandId++ );
    hal._trajCmdType.write( CMD_TYPE_TRAJ );
    hal._trajCmdOrderType.write( TRAJ_D_REL );
    hal._trajCmdDDistance.write( distance );

    hal._trajCmdValid.write( 0x1 ) ;

    while( hal._trajOutAck.read< uint8_t >() != commandId )
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }
    do
    {
        QThread::msleep( 100 );
        TrajectoryManager::State state =
            static_cast< TrajectoryManager::TrajectoryState >(
                hal._trajOutState.read< uint8_t >() );
        inWindow = hal._trajOutInWindow.read< uint8_t >();
        qDebug()
            << "Wait traj ready: State:" << state << "in windows:" << inWindow
            << "x/y:" << hal._odometryX.read<int16_t>() << "/"
            << hal._odometryY.read<int16_t>();
    } while( inWindow == 0 );
}

void TrajectoryManager::moveOnlyDRel( float distance )
{
    uint8_t inWindow;
    uint8_t commandId = hal._trajOutAck.read< uint8_t >();

    hal._trajCmdValid.write( 0x0 );
    hal._trajCmdId.write( commandId++ );
    hal._trajCmdType.write( CMD_TYPE_TRAJ );
    hal._trajCmdOrderType.write( TRAJ_ONLY_D_REL );
    hal._trajCmdDDistance.write( distance );

    hal._trajCmdValid.write( 0x1 ) ;

    while( hal._trajOutAck.read< uint8_t >() != commandId )
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }
    do
    {
        QThread::msleep( 100 );
        TrajectoryManager::State state =
            static_cast< TrajectoryManager::TrajectoryState >(
                hal._trajOutState.read< uint8_t >() );
        inWindow = hal._trajOutInWindow.read< uint8_t >();
        qDebug()
            << "Wait traj ready: State:" << state << "in windows:" << inWindow
            << "x/y:" << hal._odometryX.read<int16_t>() << "/"
            << hal._odometryY.read<int16_t>();
    } while( inWindow == 0 );
}

void TrajectoryManager::turnARel( float theta )
{
    uint8_t inWindow;
    uint8_t commandId = hal._trajOutAck.read< uint8_t >();

    hal._trajCmdValid.write( 0x0 );
    hal._trajCmdId.write( commandId++ );
    hal._trajCmdType.write( CMD_TYPE_TRAJ );
    hal._trajCmdOrderType.write( TRAJ_A_REL );
    hal._trajCmdPosTheta.write( theta );

    hal._trajCmdValid.write( 0x1 ) ;

    while( hal._trajOutAck.read< uint8_t >() != commandId )
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }
    do
    {
        QThread::msleep( 100 );
        TrajectoryManager::State state =
            static_cast< TrajectoryManager::TrajectoryState >(
                hal._trajOutState.read< uint8_t >() );
        inWindow = hal._trajOutInWindow.read< uint8_t >();
        qDebug()
            << "Wait traj ready: State:" << state << "in windows:" << inWindow
            << "theta:" << hal._odometryTheta.read<int16_t>();
    } while( inWindow == 0 );
}

void TrajectoryManager::turnAAbs( float theta )
{
    uint8_t inWindow;
    uint8_t commandId = hal._trajOutAck.read< uint8_t >();

    hal._trajCmdValid.write( 0x0 );
    hal._trajCmdId.write( commandId++ );
    hal._trajCmdType.write( CMD_TYPE_TRAJ );
    hal._trajCmdOrderType.write( TRAJ_A_ABS );
    hal._trajCmdPosTheta.write( theta );

    hal._trajCmdValid.write( 0x1 ) ;

    while( hal._trajOutAck.read< uint8_t >() != commandId )
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }
    do
    {
        QThread::msleep( 100 );
        TrajectoryManager::State state =
            static_cast< TrajectoryManager::TrajectoryState >(
                hal._trajOutState.read< uint8_t >() );
        inWindow = hal._trajOutInWindow.read< uint8_t >();
        qDebug()
            << "Wait traj ready: State:" << state << "in windows:" << inWindow
            << "theta:" << hal._odometryTheta.read<int16_t>();
    } while( inWindow == 0 );
}

void TrajectoryManager::turnOnlyARel( float theta )
{
    uint8_t inWindow;
    uint8_t commandId = hal._trajOutAck.read< uint8_t >();

    hal._trajCmdValid.write( 0x0 );
    hal._trajCmdId.write( commandId++ );
    hal._trajCmdType.write( CMD_TYPE_TRAJ );
    hal._trajCmdOrderType.write( TRAJ_ONLY_A_REL );
    hal._trajCmdPosTheta.write( theta );

    hal._trajCmdValid.write( 0x1 ) ;

    while( hal._trajOutAck.read< uint8_t >() != commandId )
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }
    do
    {
        QThread::msleep( 100 );
        TrajectoryManager::State state =
            static_cast< TrajectoryManager::TrajectoryState >(
                hal._trajOutState.read< uint8_t >() );
        inWindow = hal._trajOutInWindow.read< uint8_t >();
        qDebug()
            << "Wait traj ready: State:" << state << "in windows:" << inWindow
            << "theta:" << hal._odometryTheta.read<int16_t>();
    } while( inWindow == 0 );
}

void TrajectoryManager::turnOnlyAAbs( float theta )
{
    uint8_t inWindow;
    uint8_t commandId = hal._trajOutAck.read< uint8_t >();

    hal._trajCmdValid.write( 0x0 );
    hal._trajCmdId.write( commandId++ );
    hal._trajCmdType.write( CMD_TYPE_TRAJ );
    hal._trajCmdOrderType.write( TRAJ_ONLY_A_ABS );
    hal._trajCmdPosTheta.write( theta );

    hal._trajCmdValid.write( 0x1 ) ;

    while( hal._trajOutAck.read< uint8_t >() != commandId )
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }
    do
    {
        QThread::msleep( 100 );
        TrajectoryManager::State state =
            static_cast< TrajectoryManager::TrajectoryState >(
                hal._trajOutState.read< uint8_t >() );
        inWindow = hal._trajOutInWindow.read< uint8_t >();
        qDebug()
            << "Wait traj ready: State:" << state << "in windows:" << inWindow
            << "theta:" << hal._odometryTheta.read<int16_t>();
    } while( inWindow == 0 );
}

void TrajectoryManager::turnToXY( float x, float y )
{
    uint8_t inWindow;
    uint8_t commandId = hal._trajOutAck.read< uint8_t >();

    hal._trajCmdValid.write( 0x0 );
    hal._trajCmdId.write( commandId++ );
    hal._trajCmdType.write( CMD_TYPE_TRAJ );
    hal._trajCmdOrderType.write( TRAJ_TURNTO_XY );
    hal._trajCmdPosX.write( x );
    hal._trajCmdPosY.write( y );

    hal._trajCmdValid.write( 0x1 ) ;

    while( hal._trajOutAck.read< uint8_t >() != commandId )
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }
    do
    {
        QThread::msleep( 100 );
        TrajectoryManager::State state =
            static_cast< TrajectoryManager::TrajectoryState >(
                hal._trajOutState.read< uint8_t >() );
        inWindow = hal._trajOutInWindow.read< uint8_t >();
        qDebug()
            << "Wait traj ready: State:" << state << "in windows:" << inWindow
            << "x/y:" << hal._odometryX.read<int16_t>() << "/"
            << hal._odometryY.read<int16_t>();
    } while( inWindow == 0 );
}

void TrajectoryManager::turnToXYBehind( float x, float y )
{
    uint8_t inWindow;
    uint8_t commandId = hal._trajOutAck.read< uint8_t >();

    hal._trajCmdValid.write( 0x0 );
    hal._trajCmdId.write( commandId++ );
    hal._trajCmdType.write( CMD_TYPE_TRAJ );
    hal._trajCmdOrderType.write( TRAJ_TURNTO_XY_BEHIND );
    hal._trajCmdPosX.write( x );
    hal._trajCmdPosY.write( y );

    hal._trajCmdValid.write( 0x1 ) ;

    while( hal._trajOutAck.read< uint8_t >() != commandId )
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }
    do
    {
        QThread::msleep( 100 );
        TrajectoryManager::State state =
            static_cast< TrajectoryManager::TrajectoryState >(
                hal._trajOutState.read< uint8_t >() );
        inWindow = hal._trajOutInWindow.read< uint8_t >();
        qDebug()
            << "Wait traj ready: State:" << state << "in windows:" << inWindow
            << "x/y:" << hal._odometryX.read<int16_t>() << "/"
            << hal._odometryY.read<int16_t>();
    } while( inWindow == 0 );
}

void TrajectoryManager::moveToXYAbs( float theta, float x, float y )
{
    uint8_t inWindow;
    uint8_t commandId = hal._trajOutAck.read< uint8_t >();

    hal._trajCmdValid.write( 0x0 );
    hal._trajCmdId.write( commandId++ );
    hal._trajCmdType.write( CMD_TYPE_TRAJ );
    hal._trajCmdOrderType.write( TRAJ_GOTO_XY_ABS );
    hal._trajCmdPosTheta.write( theta );
    hal._trajCmdPosX.write( x );
    hal._trajCmdPosY.write( y );

    hal._trajCmdValid.write( 0x1 ) ;

    while( hal._trajOutAck.read< uint8_t >() != commandId )
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }
    do
    {
        QThread::msleep( 100 );
        TrajectoryManager::State state =
            static_cast< TrajectoryManager::TrajectoryState >(
                hal._trajOutState.read< uint8_t >() );
        inWindow = hal._trajOutInWindow.read< uint8_t >();
        qDebug()
            << "Wait traj ready: State:" << state << "in windows:" << inWindow
            << "x/y/theta:" << hal._odometryX.read<int16_t>() << "/"
            << hal._odometryY.read<int16_t>() << "/"
            << hal._odometryTheta.read<int16_t>();
    } while( inWindow == 0 );
}

void TrajectoryManager::moveForwardToXYAbs( float theta, float x, float y )
{
    uint8_t inWindow;
    uint8_t commandId = hal._trajOutAck.read< uint8_t >();

    hal._trajCmdValid.write( 0x0 );
    hal._trajCmdId.write( commandId++ );
    hal._trajCmdType.write( CMD_TYPE_TRAJ );
    hal._trajCmdOrderType.write( TRAJ_GOTO_FORWARD_XY_ABS );
    hal._trajCmdPosTheta.write( theta );
    hal._trajCmdPosX.write( x );
    hal._trajCmdPosY.write( y );

    hal._trajCmdValid.write( 0x1 ) ;

    while( hal._trajOutAck.read< uint8_t >() != commandId )
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }
    do
    {
        QThread::msleep( 100 );
        TrajectoryManager::State state =
            static_cast< TrajectoryManager::TrajectoryState >(
                hal._trajOutState.read< uint8_t >() );
        inWindow = hal._trajOutInWindow.read< uint8_t >();
        qDebug()
            << "Wait traj ready: State:" << state << "in windows:" << inWindow
            << "x/y/theta:" << hal._odometryX.read<int16_t>() << "/"
            << hal._odometryY.read<int16_t>() << "/"
            << hal._odometryTheta.read<int16_t>();
    } while( inWindow == 0 );
}

void TrajectoryManager::moveBackwardToXYAbs( float theta, float x, float y )
{
    uint8_t inWindow;
    uint8_t commandId = hal._trajOutAck.read< uint8_t >();

    hal._trajCmdValid.write( 0x0 );
    hal._trajCmdId.write( commandId++ );
    hal._trajCmdType.write( CMD_TYPE_TRAJ );
    hal._trajCmdOrderType.write( TRAJ_GOTO_BACKWARD_XY_ABS );
    hal._trajCmdPosTheta.write( theta );
    hal._trajCmdPosX.write( x );
    hal._trajCmdPosY.write( y );

    hal._trajCmdValid.write( 0x1 ) ;

    while( hal._trajOutAck.read< uint8_t >() != commandId )
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }
    do
    {
        QThread::msleep( 100 );
        TrajectoryManager::State state =
            static_cast< TrajectoryManager::TrajectoryState >(
                hal._trajOutState.read< uint8_t >() );
        inWindow = hal._trajOutInWindow.read< uint8_t >();
        qDebug()
            << "Wait traj ready: State:" << state << "in windows:" << inWindow
            << "x/y/theta:" << hal._odometryX.read<int16_t>() << "/"
            << hal._odometryY.read<int16_t>() << "/"
            << hal._odometryTheta.read<int16_t>();
    } while( inWindow == 0 );
}

void TrajectoryManager::moveToDARel( float theta, float distance )
{
    uint8_t inWindow;
    uint8_t commandId = hal._trajOutAck.read< uint8_t >();

    hal._trajCmdValid.write( 0x0 );
    hal._trajCmdId.write( commandId++ );
    hal._trajCmdType.write( CMD_TYPE_TRAJ );
    hal._trajCmdOrderType.write( TRAJ_D_A_REL );
    hal._trajCmdPosTheta.write( theta );
    hal._trajCmdDDistance.write( distance );

    hal._trajCmdValid.write( 0x1 ) ;

    while( hal._trajOutAck.read< uint8_t >() != commandId )
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }
    do
    {
        QThread::msleep( 100 );
        TrajectoryManager::State state =
            static_cast< TrajectoryManager::TrajectoryState >(
                hal._trajOutState.read< uint8_t >() );
        inWindow = hal._trajOutInWindow.read< uint8_t >();
        qDebug()
            << "Wait traj ready: State:" << state << "in windows:" << inWindow
            << "x/y/theta:" << hal._odometryX.read<int16_t>() << "/"
            << hal._odometryY.read<int16_t>() << "/"
            << hal._odometryTheta.read<int16_t>();
    } while( inWindow == 0 );
}

void TrajectoryManager::moveToXYRel( float x, float y )
{
    uint8_t inWindow;
    uint8_t commandId = hal._trajOutAck.read< uint8_t >();

    hal._trajCmdValid.write( 0x0 );
    hal._trajCmdId.write( commandId++ );
    hal._trajCmdType.write( CMD_TYPE_TRAJ );
    hal._trajCmdOrderType.write( TRAJ_GOTO_XY_REL );
    hal._trajCmdPosX.write( x );
    hal._trajCmdPosY.write( y );

    hal._trajCmdValid.write( 0x1 ) ;

    while( hal._trajOutAck.read< uint8_t >() != commandId )
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }
    do
    {
        QThread::msleep( 100 );
        TrajectoryManager::State state =
            static_cast< TrajectoryManager::TrajectoryState >(
                hal._trajOutState.read< uint8_t >() );
        inWindow = hal._trajOutInWindow.read< uint8_t >();
        qDebug()
            << "Wait traj ready: State:" << state << "in windows:" << inWindow
            << "x/y:" << hal._odometryX.read<int16_t>() << "/"
            << hal._odometryY.read<int16_t>();
    } while( inWindow == 0 );
}
