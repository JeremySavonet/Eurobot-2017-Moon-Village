// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_RPLIDAR_RPLIDAR_HPP_
#define WESTBOT_RPLIDAR_RPLIDAR_HPP_

#include <QString>

#include <3rdparty/rplidar_sdk/include/rplidar.h>

#include "Export.hpp"

namespace
{
    const uint32_t DEFAULT_TIMEOUT =  2000;
}

namespace WestBot {
namespace RPLidar {

class WESTBOT_RPLIDAR_EXPORT RPLidar
{
public:
    RPLidar( const QString port = QString(), uint32_t baudrate = 115200 );
    ~RPLidar();

    bool connect();
    void disconnect();

    bool isConnected();

    bool reset( uint32_t timeout = DEFAULT_TIMEOUT );

    bool setMotorPwm( uint16_t pwm );
    bool startMotor();
    bool stopMotor();
    bool checkMotorCtrlSupport( uint32_t timeout = DEFAULT_TIMEOUT );

    bool startScan( bool force = false, bool autoExpressMode = true );
    bool startScanNormal( bool force, uint32_t timeout = DEFAULT_TIMEOUT );
    bool startScanExpress( bool fixedAngle, uint32_t timeout = DEFAULT_TIMEOUT );
    bool stopScan( uint32_t timeout = DEFAULT_TIMEOUT );
    bool checkExpressScanSupported( uint32_t timeout = DEFAULT_TIMEOUT );

    // Wait and grab a complete 0-360 degree scan data previously received.
    // The grabbed scan data returned by this interface always has the following charactistics.
    bool grabScanData( rplidar_response_measurement_node_t* nodebuffer, size_t& count, uint32_t timeout = DEFAULT_TIMEOUT );

    // Ascending the scan data according to the angle value in the scan.
    bool ascendScanData( rplidar_response_measurement_node_t* nodebuffer, size_t count );

    bool checkHealth( uint32_t timeout = DEFAULT_TIMEOUT );
    QString getDeviceInfo( uint32_t timeout = DEFAULT_TIMEOUT );
    QString getSampleDurationUs( uint32_t timeout = DEFAULT_TIMEOUT );

    bool getFrequency( bool inExpressMode, size_t count, float& frequency, bool& is4kmode );

private:
    QString _port;
    uint32_t _baudrate;
    rp::standalone::rplidar::RPlidarDriver* _lidarDriver;
};

}
}

#endif // WESTBOT_RPLIDAR_RPLIDAR_HPP_
