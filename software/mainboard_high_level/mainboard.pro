QT += core
QT -= gui

CONFIG += c++11

TARGET = mainboard
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += include

SOURCES += src/main.cpp \
           src/MemoryManager.cpp \
           src/Action.cpp \
           src/GameManager.cpp \
           src/FunnyAction.cpp \
           src/Memory.cpp \
           src/Input.cpp \
           src/ItemRegister.cpp \
           src/Output.cpp

HEADERS += \
    include/WestBot/MemoryManager.hpp \
    include/WestBot/Action.hpp \
    include/WestBot/GameManager.hpp \
    include/WestBot/FunnyAction.hpp \
    include/WestBot/ItemRegister.hpp \
    include/WestBot/Memory.hpp \
    include/WestBot/Input.hpp \
    include/WestBot/Layer.hpp \
    include/WestBot/Output.hpp

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
