QT += core gui widgets

TARGET = AStarDemo
TEMPLATE = app

SOURCES += main.cpp\
           mainwindow.cpp \
           mapscene.cpp

HEADERS += mainwindow.h \
           mapscene.h

FORMS += mainwindow.ui

RESOURCES += images.qrc

INCLUDEPATH += ../astar/include/
LIBS += -L../astar/lib -lAStar
