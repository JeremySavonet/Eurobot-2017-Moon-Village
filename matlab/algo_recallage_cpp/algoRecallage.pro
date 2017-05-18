QT += core
QT -= gui

CONFIG += c++11

TARGET = algoRecallage
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += include \
               3rdparty/Eigen

SOURCES += src/main.cpp
