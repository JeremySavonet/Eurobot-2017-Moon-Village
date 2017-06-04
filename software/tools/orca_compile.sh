#!/bin/sh

#/opt/altera16.1_lite/nios2eds/nios2_command_shell.sh

export LM_LICENSE_FILE=

export PATH=$PATH:/workspace/nano_soc/orca/orca/rv/bin/
export PATH=$PATH:/opt/altera16.1_lite/quartus/bin/



cd ../orca/software/generic
make clean
make

cd ../low_level
make clean
make


cd ../../tools/
rm orca.elf.qex
./elf2hex.sh ../software/generic/orca.elf
cp orca.elf.qex ../../HPS_FPGA_LED/fpga/test.hex

./elf2hex.sh ../software/low_level/orca.elf
cp orca.elf.qex ../../HPS_FPGA_LED/fpga/system_ll.hex

quartus_cdb --update_mif ../..//HPS_FPGA_LED/fpga/HPSFPGA.qpf
quartus_asm ../..//HPS_FPGA_LED/fpga/HPSFPGA.qpf
quartus_cpf -c -o bitstream_compression=on ../..//HPS_FPGA_LED/fpga/output_files/HPSFPGA.sof ../..//HPS_FPGA_LED/fpga/output_files/soc_system.rbf
