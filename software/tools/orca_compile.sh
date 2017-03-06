#!/bin/sh

#/opt/altera16.1_lite/nios2eds/nios2_command_shell.sh

export LM_LICENSE_FILE=

export PATH=$PATH:/workspace/nano_soc/orca/orca/rv/bin/
export PATH=$PATH:/opt/altera16.1_lite/quartus/bin/



cd ../orca/software/de2-test
make

cd ../../tools/
rm de2-test.elf.qex
./elf2hex.sh ../software/de2-test/de2-test.elf
cp de2-test.elf.qex ../../HPS_FPGA_LED/fpga/test.hex
quartus_cdb --update_mif ../..//HPS_FPGA_LED/fpga/HPSFPGA.qpf
quartus_asm ../..//HPS_FPGA_LED/fpga/HPSFPGA.qpf

