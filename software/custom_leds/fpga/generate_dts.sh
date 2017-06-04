#!/bin/sh

sopc2dts -v \
         --input soc_system.sopcinfo\
         --output soc_system.dts\
         --type dts\
         --board soc_system_board_info.xml\
         --board hps_common_board_info.xml\
         --bridge-removal all\
         --clocks
