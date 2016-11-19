HPS_FPGA_CUSTOM_LED:
====

This is a demo program to get started with the DEO nano SoC board and test that your development environment is set up right.

Before started:
===
  * You'll need to install the Altera suite which is composed of:
  - Quartus Prime Edition: http://dl.altera.com/16.1/?edition=lite
  - Quartus SoC EDS: http://dl.altera.com/soceds/

  * Create a Debian Image for the board (see: https://github.com/JeremySavonet/Eurobot-2017-Moon-Village/blob/master/README.md)

  * Burn the Image to your micro SD card

  * Generate the rbf file from the sof file

  * Make sure you have connected the UART cable (J13) before powering up the board

  * Put the MSEL jumper like this : ON / OFF / ON / OFF / ON / XX

  * Power the board
  * Open the UART with scree for example:
<code>screen /dev/ttyUSB0 115200</code>
  * Log in with root when needed (login: root, mdp: admin)

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

Test the HPS_FPGA_CUSTOM_LED example program:
===
  * On Linux/Windows, start the Embedded command shell from altera
  * Go to the fpga folder using cd and file the sof file.
  * Run the following command to generate your rbf file:
```
quartus_cpf -c -o bitstream_compression=on your_sof_file.sof DE0_NANO_SOC.rbf
```
  * Change the rbf file on your SD card
  * Go to the fpga folder using cd and run generate.sh script. It will create the hps_0.h header files to make your program and interact with the FPGA.
  * Go to the software folder using cd and type make. It will create a binary file HPS_FPGA_CUSTOM_LED.
  * You can use SCP or place the binary file directly into your sd card at: /home/fpga/
  * Disconnect your SD card and reboot the board.
  * You should see Linux booting and end to a command prompt.
  * cd to /home/fpga and run the HPS_FPGA_CUSTOM_LED binary
  * You are done !
