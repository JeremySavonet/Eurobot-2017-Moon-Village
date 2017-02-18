#
TARGET = HPS_FPGA_CUSTOM_LED

#
CROSS_COMPILE = arm-linux-gnueabihf-
ALT_DEVICE_FAMILY ?= soc_cv_av
CXXFLAGS = -g -Wall -I${SOCEDS_DEST_ROOT}/ip/altera/hps/altera_hps/hwlib/include -I${SOCEDS_DEST_ROOT}/ip/altera/hps/altera_hps/hwlib/include/$(ALT_DEVICE_FAMILY) -D$(ALT_DEVICE_FAMILY)
LDFLAGS =  -g -Wall
CXX = $(CROSS_COMPILE)g++
ARCH= arm

SRCS=Led.cpp \
     main.cpp
OBJS=$(SRCS:.cpp=.o )

build: $(TARGET)
$(TARGET): main.o
	$(CXX) $(LDFLAGS) $(SRCS) -o $@
%.o : %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) *.a *.o *~
