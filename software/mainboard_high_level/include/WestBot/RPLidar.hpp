// Copyright (c) 2017 All Rights Reserved WestBot

#ifndef WESTBOT_RPLIDAR_HPP_
#define WESTBOT_RPLIDAR_HPP_

#define MAX_LEN_SCAN_DATA 10000

class QSerialPort;
class QString;

namespace WestBot
{
class RPLidar
{
public:
    RPLidar( const QString& port );

    bool connect(); // connect to tty
    void disconnect();

    bool startScan( int timeoutMs = 1000 ); // send start command (continuous)

    void flushScanData();

    bool grabScanData( double* distance, double* angle, int* len );

private:
    QSerialPort* _tty;
};

}

#endif // WESTBOT_RPLIDAR_HPP_
