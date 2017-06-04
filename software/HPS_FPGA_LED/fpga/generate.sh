#!/bin/sh
sopc-create-header-files \
"$PWD/hps_fpga.sopcinfo" \
--single hps_arm.h \
--module hps_arm
