HPS_FPGA_LED:
====

This is a demo program to get started with the DEO nano SoC board and test that your development environment is set up right.

Before started:
===
  * You'll need to install the Altera suite which is composed of:
  - Quartus Prime Edition: http://dl.altera.com/16.1/?edition=lite
  - Quartus SoC EDS: http://dl.altera.com/soceds/

  * Get the latest demo Linux Image:
    http://releases.rocketboards.org/release/2015.10/atlas/atlas_sdcard_v1.1.img.tgz

  * Burn the Image to your micro SD card using dd (Linux) or Win32DiskImage
    (Windows)

  * Make sure you have connected the UART cable (J13) before powering up the board

  * Put the MSEL jumper like this : ON / OFF / ON / OFF / ON / XX

  * Power the board
  * Open the UART with scree for example:
<code>screen /dev/ttyUSB0 115200</code>
  * Log in with root when needed

Troubleshooting:
===
  * On Debian Jessie (8), you'll probably need to execute the following commands to end setup the cross compilation toolchains:
<code>
sudo dpkg --add-architecture i386
sudo apt-get update
sudo apt-get install libc6:i386 libstdc++6:i386 libfontconfig1:i386 libfreetype6:i386 libice6:i386 lib32ncurses5 zlib1g:i386 libusb-0.1-4:i386
</code>

Required Software:
===
  * Quartus Prime Edition from Altera: http://dl.altera.com/16.1/?edition=lite
  * SoC EDS from Altera : can be dowloaded here : http://dl.altera.com/soceds/

Note:
===
  * To run generate.sh script and Make, you'll need to start the embedded_command_shell.sh. This script will set up correctly all the environment variable for you. It can be found in the EDS folder. For me: /home/user/intelFPGA/embedded/embedded_command_shell.sh.

Test the HPS_FPGA_LED example program:
===
  * On Linux/Windows, start the Embedded command shell from altera
  * Go to the fpga folder using cd and run generate.sh script. It will create the hps_0.h header files to make your program and interact with the FPGA.
  * Go to the software folder using cd and type make. It will create a binary file HPS_FPGA_LED.
  * You can use SCP or place the binary file directly into your sd card at: /home/root/
  * Use chmod +x and then run
  * You will need to program the FPGA to:
  - Start Quartus programmer.
  - Select the hardware and click autodetect.
  - You'll see the SOCVHPS and FPGA.
  - Click on the FPGA and select 'change file'.
  - Go to fpga folder -> output and find the *.sof file
  - Select write and start.
  * You are done !

Note that if you reboot the board, you'll need to program the FPGA again since it's not write in the flash.
