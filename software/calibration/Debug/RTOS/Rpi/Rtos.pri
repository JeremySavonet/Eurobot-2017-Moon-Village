###################################################################################################
### \file		Rtos.pri
### \brief		Librairie rtos
###
### \author		Etienne BOILLEY
### \date		20/09/2015
### \copyright	IOCENE
###################################################################################################

win32 {
    INCLUDEPATH += $$quote($$system(cd)/../)
    VPATH += $$quote($$system(cd)/../)
}
else {
    INCLUDEPATH += $$quote($$system(pwd)/../)
    VPATH += $$quote($$system(pwd)/../)
}


QT += serialport
QT += network


HEADERS = \
	Configuration.h \
	Common/Macro.h \
	Common/Types.h \
	Rpi/Types.rpi.h \
	Common/Fifo.h \
	Rpi/Console.rpi.h \
	Common/Debug.h \
	Common/DebugLevel.h \
	Rpi/Time.rpi.h \
	Common/Task.h \
    Rpi/Task.rpi.hpp \
     Common/Uart.h \
    Rpi/Uart.rpi.h \
	Common/Protocol.h \
	Rtos.h \
	Rpi/Rtos.rpi.h \

SOURCES = \
	Common/Tests/Types.test.c \
	Common/Fifo.c \
	Rpi/Console.rpi.cpp \
	Common/Debug.c \
	Rpi/Time.rpi.cpp \
    Rpi/Task.rpi.cpp \
   # Rpi/Uart.rpi.cpp \
	Common/Protocol.c \
	Rpi/Rtos.rpi.cpp \


