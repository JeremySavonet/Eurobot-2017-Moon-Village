// Copyright (c) 2017 All Rights Reserved WestBot

#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QTimer>

#include <WestBot/ColorSensor.hpp>

using namespace WestBot;

namespace
{
    const uint16_t COLOR_RED_THRESHOLD = 100;
    const uint16_t COLOR_GREEN_THRESHOLD = 100;
    const uint16_t COLOR_BLUE_THRESHOLD = 100;
    const uint16_t COLOR_CLEAR_THRESHOLD = 100;

    const int MOTOR_COLOR_1T_PER_SECOND = 16500;
    const int RET_OK = 0;
    const int RET_ERROR_UNKNOWN = 1;
    const int RET_ERROR_COLOR_SINGLE_OPPOSITE = 2;
}

ColorSensor::ColorSensor( const QString& name )
    : _name( name )
    , _sensorValid( nullptr )
    , _sensorRed( nullptr )
    , _sensorGreen( nullptr )
    , _sensorBlue( nullptr )
    , _sensorClear( nullptr )
    , _colorTarget( Color::Unknown )
    , _isAttached( false )
{

}

bool ColorSensor::attach( Hal& hal )
{    
    bool isTimeout = false;

    QTimer timeout;
    timeout.setSingleShot( true );
    timeout.start( 1000 );
    QObject::connect(
        & timeout,
        & QTimer::timeout,
        [ &isTimeout ]()
        {
            isTimeout = true;
        } );

    if( ! _isAttached )
    {
        _motorOverride = std::make_shared< ItemRegister >( hal._motor3Override );
        _motorInverted = std::make_shared< ItemRegister >( hal._motor3Inverted );
        _motorValue = std::make_shared< ItemRegister >( hal._motor3Value );

        _sensorValid = std::make_shared< ItemRegister >( hal._colorSensorValid );
        _sensorRed = std::make_shared< ItemRegister >( hal._colorSensorRed );
        _sensorGreen = std::make_shared< ItemRegister >( hal._colorSensorGreen );
        _sensorBlue = std::make_shared< ItemRegister >( hal._colorSensorBlue );
        _sensorClear = std::make_shared< ItemRegister >( hal._colorSensorClear );

        while( ! isTimeout && _sensorValid->read< uint16_t >() != 0x01 )
        {
            if( isTimeout )
            {
                _isAttached = false;
                return false;
            }

            QThread::msleep( 10 );
            QCoreApplication::processEvents();
        }
    }

    timeout.stop();

    _motorValue->write( 0 );
    _motorInverted->write( 1 );
    _motorOverride->write( 0x00 );

    qDebug() << "Sensor module is attached";

    _isAttached = true;

    return true;
}

void ColorSensor::changeTarget( Color colorTarget )
{
    _colorTarget = colorTarget;
}

const QString& ColorSensor::name() const
{
    return _name;
}

bool ColorSensor::isInRange()
{
    if( Color::Unknown == _colorTarget )
    {
        qWarning() << "No color target defined";
        return false;
    }

    const uint16_t red = _sensorRed->read< uint16_t >();
    const uint16_t green = _sensorGreen->read< uint16_t >();
    const uint16_t blue = _sensorBlue->read< uint16_t >();
    const uint16_t clear = _sensorClear->read< uint16_t >();

    qDebug() << "Red:" << red << " Green:" << green << " Blue:" << blue << " Clear:" << clear;

    // TODO: read register, compare to threashold and check with target color
    return true;
}

Color ColorSensor::sensorCheck()
{
    const uint16_t red = _sensorRed->read< uint16_t >();
    const uint16_t green = _sensorGreen->read< uint16_t >();
    const uint16_t blue = _sensorBlue->read< uint16_t >();
    const uint16_t clear = _sensorClear->read< uint16_t >();

    qDebug() << "Red:" << red << " Green:" << green << " Blue:" << blue << " Clear:" << clear;

    // TODO: read register and compare to threshold
    return Color::Blue;
}

bool ColorSensor::isAttached() const
{
    return _isAttached;
}

// WIP ALGO BRICE: <===============
void ColorSensor::process()
{
    uint8_t search_blue = 0;
    uint8_t ret;

    redo_search:
    // equivalent to:
    //uint8_t set_color(uint8_t search_blue)
    {
        uint16_t vMin = 65535;
        uint16_t vMax = 0;

        uint16_t j;
        uint8_t ok = 0;

        while( ok == 0 )
        {
            uint16_t value;

            _motorValue->write( MOTOR_COLOR_1T_PER_SECOND );

            for( j = 0; j < 1000; j++ )
            {
                value = _sensorClear->read< uint16_t >();
                if( value < vMin )
                {
                    vMin = value;
                }
                if( value > vMax )
                {
                    vMax = value;
                }

                QThread::msleep( 1 );
            }

            _motorValue->write( 0 );

            qDebug() << "lowest is " << vMin;
            qDebug() << "higuest is " << vMax;

            if( abs( vMin - vMax ) < vMax / 2 )
            {
                uint16_t blue = _sensorBlue->read< uint16_t >();
                qDebug() << "Blue is " << blue;

                if( ( search_blue == 1 && blue >= vMin / 3 ) ||
                    ( search_blue == 0 && blue < vMin / 3 ) )
                {
                    ret = RET_OK;
                    qDebug() << "Single color: OK";
                }
                else
                {
                    ret = RET_ERROR_COLOR_SINGLE_OPPOSITE;
                    qDebug() << "Single color opposite: NOK";
                }

                ok = 1;
                break;
            }

            QThread::msleep( 500 );

            for( uint16_t retry = 0; retry < 2 * 1000; retry++ )
            {
                _motorValue->write( MOTOR_COLOR_1T_PER_SECOND );

                {
                    value = _sensorClear->read< uint16_t >();

                    if( abs( value - vMax ) <= vMax / 5 )
                    {
                        qDebug() << "MAX FOUND" << value << vMax;

                        _motorValue->write( -MOTOR_COLOR_1T_PER_SECOND );
                        QThread::msleep( 250 );
                        _motorValue->write( 0 );
                        QThread::msleep( 100 );

                        uint16_t blue_value = _sensorBlue->read< uint16_t >();
                        uint16_t clear_value = _sensorClear->read< uint16_t >();

                        qDebug() << "COLOR TO CHECK" << blue_value << clear_value << vMin << vMax;

                        if( ( search_blue == 0 ) &&
                            ( blue_value < clear_value / 3 ) &&
                            ( abs( clear_value - vMax ) > vMax / 5 ) )
                        {
                            qDebug() << "YELLOW found!";
                            ok = 1;
                            ret = RET_OK;
                            break;
                        }
                        else if( ( search_blue == 1 ) &&
                                 ( blue_value >= clear_value / 3 ) &&
                                 ( abs( clear_value - vMax ) > vMax / 5 ) )
                        {
                            qDebug() << "BLUE found!";
                            ok = 1;
                            ret = RET_OK;
                            break;
                        }
                        else
                        {
                            qDebug() << "Not Found, Retry (or stop)" << value << vMin;

                            _motorValue->write( MOTOR_COLOR_1T_PER_SECOND );
                            QThread::msleep( 500 );
                            _motorValue->write(0);
                        }
                    }
                    else
                    {
                        QThread::msleep( 1 );
                    }
                }
            }

            ok = 1;
            ret = RET_ERROR_UNKNOWN;
            _motorValue->write( 0 );
        }
    }

    qDebug() << "Ret=" << ret << "Search Blue=" << search_blue;
    QThread::msleep( 5000 );

    if( search_blue )
    {
        search_blue = 0;
    }
    else
    {
        search_blue = 1;
    }

    goto redo_search;
}
