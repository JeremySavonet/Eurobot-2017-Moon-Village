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

    const int COLOR_INIT_OK = 0;
    const int COLOR_INIT_NOK = 1;
    const int VALUE_COUNT = 100;

    const int AVG_CNT = 10;
}

ColorSensor::ColorSensor( const QString& name )
    : _name( name )
    , _sensorValid( nullptr )
    , _sensorRed( nullptr )
    , _sensorGreen( nullptr )
    , _sensorBlue( nullptr )
    , _sensorClear( nullptr )
    , _vcAvg( 0 )
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
    _motorOverride->write( 0x01 );

    for( int i = 0; i < VALUE_COUNT; i++ )
    {
        uint16_t value;
        value = _sensorClear->read< uint16_t >();
        _vcAvg += value;
        QThread::msleep( 10 );
    }

    _vcAvg = _vcAvg / VALUE_COUNT;

    qDebug() << "AVG Clear is " << _vcAvg;

    if( _vcAvg > 400 || _vcAvg == 0 )
    {
        qDebug() << "ERROR = The carrousel seems to be not empty, init is KO";
        _isAttached = false;
        return false;
    }

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

bool ColorSensor::isAttached() const
{
    return _isAttached;
}

int ColorSensor::sensorCheck()
{
    uint8_t ret;
    uint16_t v_min = 65535;
    uint16_t v_max = 0;
    uint16_t j;
    uint16_t value;

    _motorValue->write( MOTOR_COLOR_1T_PER_SECOND );

    for( j = 0; j < 1000; j++ )
    {
        value = _sensorClear->read< uint16_t >();
        if( value < v_min )
        {
            v_min = value;
        }

        if( value > v_max )
        {
            v_max = value;
        }

        QThread::msleep( 1 );
    }

    _motorValue->write( 0 );

    qDebug() << "lowest is " << v_min;
    qDebug() << "higuest is " << v_max;

    if( abs( v_min - v_max ) < v_max / 2 )
    {
        uint16_t blue = _sensorBlue->read< uint16_t >();
        qDebug() << "blue is " << blue;

        if( ( _colorTarget == Color::Blue && blue >= v_min / 3 ) ||
            ( _colorTarget == Color::Yellow && blue < v_min / 3 ) )
        {
            ret = RET_OK;
            qDebug() << "Single color: OK!!";
        }
        else
        {
            ret = RET_ERROR_COLOR_SINGLE_OPPOSITE;
            qDebug() << "Single color opposite: NOK!!";
        }

        return ret;
    }

    QThread::msleep( 50 );

    for( uint16_t retry = 0; retry < 2 * 1000; retry++ )
    {
        _motorValue->write( MOTOR_COLOR_1T_PER_SECOND );
        {
            value = _sensorClear->read< uint16_t >();

            if( abs( value - v_max ) <= v_max / 5 )
            {
                qDebug() << "MAX FOUND" << value << v_max;

                _motorValue->write( -MOTOR_COLOR_1T_PER_SECOND );
                QThread::msleep( 250 );
                _motorValue->write( 0 );
                QThread::msleep( 100 );

                uint16_t blue_value = _sensorBlue->read< uint16_t >();
                uint16_t clear_value = _sensorClear->read< uint16_t >();

                qDebug() << "COLOR TO CHECK" << blue_value << clear_value << v_min << v_max;

                if( ( _colorTarget == Color::Yellow ) &&
                    ( blue_value < clear_value / 3 ) &&
                    ( abs( clear_value - v_max ) > v_max / 5 ) )
                {
                    qDebug() << "YELLOW found!";
                    ret = RET_OK;
                    return ret;
                }
                else if( ( _colorTarget == Color::Blue ) &&
                         ( blue_value >= clear_value / 3 ) &&
                         ( abs( clear_value - v_max ) > v_max / 5 ) )
                {
                    qDebug() << "BLUE found!";
                    ret = RET_OK;
                    return ret;
                }
                else
                {
                    qDebug() << "Not Found, Retry (or stop)" << value << v_min;
                    _motorValue->write( MOTOR_COLOR_1T_PER_SECOND );
                    QThread::msleep( 500 );
                }
            }
            else
            {
                QThread::msleep( 1 );
            }
        }
    }

    ret = RET_ERROR_UNKNOWN;
    _motorValue->write( 0 );
    return ret;
}

bool ColorSensor::checkIsEmpty()
{
    uint32_t avg = 0;
    for( int i = 0; i < AVG_CNT; i++ )
    {
        uint16_t value;
        value = _sensorClear->read< uint16_t >();
        avg += value;
        QThread::msleep( 10 );
    }

    avg = avg / AVG_CNT;
    qDebug() << "AVG is " << avg;

    if( avg > 3 * _vcAvg )
    {
        qDebug() << "Slot is not empty;";
        return false;
    }
    else
    {
        qDebug() << "Slot is empty;";
        return true;
    }
}
