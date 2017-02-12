# Copyright (c) 2016-2017 All Rights Reserved WestBot

TARGET = MainBoard

INCLDIR = include

CROSS_COMPILE = arm-linux-gnueabihf-
ALT_DEVICE_FAMILY ?= soc_cv_av
CXXFLAGS = -g -Wall -std=c++11 \
           -I${SOCEDS_DEST_ROOT}/ip/altera/hps/altera_hps/hwlib/include \
           -I${SOCEDS_DEST_ROOT}/ip/altera/hps/altera_hps/hwlib/include/$(ALT_DEVICE_FAMILY) \
           -D$(ALT_DEVICE_FAMILY) \

LDFLAGS = -g -Wall -std=c++11
CXX = $(CROSS_COMPILE)g++
ARCH = arm

SRCDIR = src
CPPSRCS = $(SRCDIR)/ControlSystem/Filters/Pid.cpp \
          $(SRCDIR)/Memory/MemoryManager.cpp \
          $(SRCDIR)/Motor/BrushlessMotorManager.cpp \
          $(SRCDIR)/Motor/DcMotorManager.cpp \
          $(SRCDIR)/Motor/QeiManager.cpp \
          $(SRCDIR)/Motor/MotorManager.cpp \
          $(SRCDIR)/Robot/AngleDistance.cpp \
          main.cpp
OBJS = $(CPPSRCS:.cpp=.o )

build: $(TARGET)
$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(CPPSRCS) -o $@
%.o : %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) *.a *.o *~

