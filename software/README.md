Software section:
====

Before started:
===
  
  * Get the latest demo Linux Image:
    http://releases.rocketboards.org/release/2015.10/atlas/atlas_sdcard_v1.1.img.tgz

  * Burn the Image to your micro SD card using dd (Linux) or Win32DiskImage
    (Windows)
  
  * Make sure you have connected the UART cable (J13) before powering up the board

  * Put the MSEL jumper like this : ON / OFF / ON / OFF / ON / XX

  * Power the board and log in with root

Required Software:
===

  * SoC EDS from Altera : can be dowloaded here : http://dl.altera.com/soceds/

Test the HelloWorld example program:
===

  * On Windows, start the Embedded command shell from altera
(C:/Altera/Version/Embedded/command_shell.bat)
  
  * Go to the project folder using cd and run make

  * Use SCP to upload the binary on the board

  * Use chmod +x and then run

  * You are done !!!!


