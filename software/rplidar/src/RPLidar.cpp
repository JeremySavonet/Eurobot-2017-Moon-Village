// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <QDebug>

#include <WestBot/RPLidar/RPLidar.hpp>

using namespace rp::standalone::rplidar;
using namespace WestBot::RPLidar;

namespace
{
    QString deviceInfoToString( rplidar_response_device_info_t deviceInfo )
    {
        Q_UNUSED( deviceInfo );
        return QString();
    }

    QString rateInfoToString( rplidar_response_sample_rate_t rateInfo )
    {
        Q_UNUSED( rateInfo );
        return QString();
    }
}

RPLidar::RPLidar( const QString port, uint32_t baudrate )
    : _port( port )
    , _baudrate( baudrate )
    , _lidarDriver( RPlidarDriver::CreateDriver( RPlidarDriver::DRIVER_TYPE_SERIALPORT ) )
{
}

RPLidar::~RPLidar()
{
    disconnect();
    RPlidarDriver::DisposeDriver( _lidarDriver );
}

bool RPLidar::connect()
{
    if( IS_FAIL( _lidarDriver->connect( _port.toStdString().c_str(), _baudrate ) ) )
    {
        qWarning() << "Error, cannot bind to the specified serial port:"
                   << _port;
        return false;
    }

    return true;
}

void RPLidar::disconnect()
{
    _lidarDriver->disconnect();
}

bool RPLidar::isConnected()
{
    return _lidarDriver->isConnected();
}

bool RPLidar::reset( uint32_t timeout )
{
    if( IS_FAIL( _lidarDriver->reset( timeout) ) )
    {
        return false;
    }

    return true;
}

bool RPLidar::setMotorPwm( uint16_t pwm )
{
    if( IS_FAIL( _lidarDriver->setMotorPWM( pwm ) ) )
    {
        return false;
    }

    return true;
}

bool RPLidar::startMotor()
{
    if( IS_FAIL( _lidarDriver->startMotor() ) )
    {
        return false;
    }

    return true;
}

bool RPLidar::stopMotor()
{
    if( IS_FAIL( _lidarDriver->stopMotor() ) )
    {
        return false;
    }

    return true;
}

bool RPLidar::checkMotorCtrlSupport( uint32_t timeout )
{
    bool motorCtrlSupport;
    if( IS_FAIL( _lidarDriver->checkMotorCtrlSupport( motorCtrlSupport, timeout ) ) )
    {
        qWarning() << "Error, cannot check motor control support.";
        return false;
    }

    return motorCtrlSupport;
}

bool RPLidar::startScan( bool force , bool autoExpressMode )
{
    if( IS_FAIL( _lidarDriver->startScan( force, autoExpressMode ) ) )
    {
        return false;
    }

    return true;
}

bool RPLidar::startScanNormal( bool force, uint32_t timeout )
{
    if( IS_FAIL( _lidarDriver->startScanNormal( force, timeout ) ) )
    {
        return false;
    }

    return true;
}

bool RPLidar::startScanExpress( bool fixedAngle, uint32_t timeout )
{
    if( IS_FAIL( _lidarDriver->startScanExpress( fixedAngle, timeout ) ) )
    {
        return false;
    }

    return true;
}

bool RPLidar::stopScan( uint32_t timeout )
{
    if( IS_FAIL( _lidarDriver->stop( timeout ) ) )
    {
        return false;
    }

    return true;
}

bool RPLidar::checkExpressScanSupported( uint32_t timeout )
{
    bool expressScanSupport;
    if( IS_FAIL( _lidarDriver->checkExpressScanSupported( expressScanSupport, timeout ) ) )
    {
        qWarning() << "Error, cannot check express scan support.";
        return false;
    }

    return expressScanSupport;
}

bool RPLidar::grabScanData( rplidar_response_measurement_node_t* nodebuffer, size_t& count, uint32_t timeout )
{
    Q_UNUSED( nodebuffer );
    Q_UNUSED( count );
    Q_UNUSED( timeout );

    // TODO: change the implem
    return false;
}

// Ascending the scan data according to the angle value in the scan.
bool RPLidar::ascendScanData( rplidar_response_measurement_node_t* nodebuffer, size_t count )
{
    Q_UNUSED( nodebuffer );
    Q_UNUSED( count );

    // TODO: change the implem
    return false;
}

bool RPLidar::checkHealth( uint32_t timeout )
{
    u_result operationResult;
    rplidar_response_device_health_t healthInfo;

    operationResult = _lidarDriver->getHealth( healthInfo, timeout );
    if( IS_FAIL( operationResult ) )
    {
        qWarning() << "Error, cannot retrieve the lidar health code";
        return false;
    }

    qDebug() << "RPLidar health status:" << healthInfo.status;
    if( RPLIDAR_STATUS_ERROR == healthInfo.status )
    {
        qWarning() << "Error, rplidar internal error detected. Please reboot the device to retry.";
        reset(); // automatically reboot de device
        return false;
    }

    return true;
}

QString RPLidar::getDeviceInfo( uint32_t timeout )
{
    u_result operationResult;
    rplidar_response_device_info_t deviceInfo;

    operationResult = _lidarDriver->getDeviceInfo( deviceInfo, timeout );

    if( IS_FAIL( operationResult ) )
    {
        qWarning() << "Error, cannot get device info.";
        return QString();
    }

    return deviceInfoToString( deviceInfo );
}

QString RPLidar::getSampleDurationUs( uint32_t timeout )
{
    u_result operationResult;
    rplidar_response_sample_rate_t rateInfo;

    operationResult = _lidarDriver->getSampleDuration_uS( rateInfo, timeout );

    if( IS_FAIL( operationResult ) )
    {
        qWarning() << "Error, cannot get rate info.";
        return QString();
    }

    return rateInfoToString( rateInfo );
}

bool RPLidar::getFrequency( bool inExpressMode, size_t count, float& frequency, bool& is4kmode )
{
    if( IS_FAIL( _lidarDriver->getFrequency( inExpressMode, count, frequency, is4kmode ) ) )
    {
        return false;
    }

    return true;
}

