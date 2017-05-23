QT += core
QT -= gui

CONFIG += c++11

TARGET = algoRecallage
target.path = /root
INSTALLS += target

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += include \
               3rdparty/Eigen

HEADERS += 3rdparty/Eigen/src/* \
    src/Recalage.hpp

SOURCES += src/main.cpp \
    src/Recalage.cpp

