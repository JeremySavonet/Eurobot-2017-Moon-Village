QT += core
QT -= gui

TARGET = RPLidarDemo
TEMPLATE = app

SOURCES += main.cpp

CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle

INCLUDEPATH += /home/westbot/Documents/Robotics/Eurobot-2017-Moon-Village/software/rplidar/include/ \
               /home/westbot/Documents/Robotics/Eurobot-2017-Moon-Village/software/rplidar/3rdparty/rplidar_sdk/include

LIBS += -L/home/westbot/Documents/Robotics/Eurobot-2017-Moon-Village/software/rplidar/lib -lRPLidar
LIBS += -L/home/westbot/Documents/Robotics/Eurobot-2017-Moon-Village/software/rplidar/rplidar/3rdparty/rplidar_sdk/lib -lSlamtecRPLidar

INCLUDEPATH += include \
               3rdparty/rplidar_sdk/include

SOURCES += src/RPLidar.cpp

HEADERS += include/WestBot/RPLidar/RPLidar.hpp
