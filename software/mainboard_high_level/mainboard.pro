QT += core network
QT -= gui

CONFIG += c++11

TARGET = mainboard
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += include \
               ../rplidar/include

LIBS += -L../rplidar/lib -lRPLidar

SOURCES += src/Carrousel.cpp \
           src/Configuration.cpp \
           src/ConfigurationTcpServer.cpp \
           src/Hal.cpp \
           src/Input.cpp \
           src/ItemRegister.cpp \
           src/Memory.cpp \
           src/MemoryManager.cpp \
           src/Output.cpp \
           src/StrategyManager.cpp \
           src/SystemManager.cpp \
           src/main.cpp \
           src/Servo.cpp \
           src/TrajectoryManager.cpp \
           src/ColorSensor.cpp \
           src/DetectionManager.cpp \
           src/PositionManager.cpp \
           src/Turbine.cpp \
           src/Action.cpp \
           src/MoveAction.cpp \
           src/TurnCarrouselAction.cpp \
           src/WaitAction.cpp \
    src/MoveArmsAction.cpp \
    src/TurbineAction.cpp \
    src/FunnyAction.cpp

HEADERS += \
    include/Defines.hpp \
    include/WestBot/Carrousel.hpp \
    include/WestBot/Common.hpp \
    include/WestBot/Configuration.hpp \
    include/WestBot/ConfigurationTcpServer.hpp \
    include/WestBot/Hal.hpp \
    include/WestBot/Input.hpp \
    include/WestBot/ItemRegister.hpp \
    include/WestBot/Layer.hpp \
    include/WestBot/Memory.hpp \
    include/WestBot/MemoryManager.hpp \
    include/WestBot/Output.hpp \
    include/WestBot/StrategyManager.hpp \
    include/WestBot/SystemManager.hpp \
    include/WestBot/Servo.hpp \
    include/WestBot/TrajectoryManager.hpp \
    include/WestBot/ColorSensor.hpp \
    include/WestBot/DetectionManager.hpp \
    include/WestBot/PositionManager.hpp \
    include/WestBot/Turbine.hpp \
    include/WestBot/Action.hpp \
    include/WestBot/MoveAction.hpp \
    include/WestBot/TurnCarrouselAction.hpp \
    include/WestBot/WaitAction.hpp \
    include/WestBot/MoveArmsAction.hpp \
    include/WestBot/TurbineAction.hpp \
    include/WestBot/FunnyAction.hpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

target.path = /root
INSTALLS += target
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
