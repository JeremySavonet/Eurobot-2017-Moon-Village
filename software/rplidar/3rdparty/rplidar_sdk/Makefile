#/*
# * Copyright (C) 2017  WestBot
# */
#

HOME_TREE := .

MODULE_NAME := $(notdir $(CURDIR))

CPPFLAGS= -g
LDFLAGS= -shared

.PHONY : clean

include $(HOME_TREE)/mak_def.inc

SOURCES += src/rplidar_driver.cpp \
           src/hal/thread.cpp

C_INCLUDES += -I$(CURDIR)/include -I$(CURDIR)/src

ifeq ($(BUILD_TARGET_PLATFORM),Linux)
SOURCES += src/arch/linux/net_serial.cpp \
           src/arch/linux/timer.cpp
endif

ifeq ($(BUILD_TARGET_PLATFORM),Darwin)
SOURCES += src/arch/macOS/net_serial.cpp \
           src/arch/macOS/timer.cpp
endif

OBJECTS=$(SOURCES:.cpp=.o)

TARGET=lib/libSlamtecRPLidar.so

all: $(TARGET)

include $(HOME_TREE)/mak_common.inc

clean: 
	rm -rf $(OBJECTS)
$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)
