QT += core
QT -= gui

TARGET = RPLidar

TEMPLATE = lib

INCLUDEPATH += include \
               3rdparty/rplidar_sdk/include

SOURCES += src/RPLidar.cpp

HEADERS += include/WestBot/RPLidar/RPLidar.hpp

LIBS += -L./3rdparty/rplidar_sdk/lib -lSlamtecRPLidar
R
DEFINES += WESTBOT_RPLIDARSHAREDLIB_LIBRARY

DEFINES += QT_DEPRECATED_WARNINGS
