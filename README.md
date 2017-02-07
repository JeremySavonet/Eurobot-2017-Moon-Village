EUROBOT 2017 - Moon Village
====

Make the SD card with the Linux Image:
===

First checkout this repository by using:

```
shell$ git clone git://github.com/ikwzm/FPGA-SoC-Linux
shell$ cd FPGA-SoC-Linux
shell$ git lfs pull origin master
```

Make sure you have git lfs installed before running the third command.
If not installed, just type:
```
### On Debian:

echo 'deb http://http.debian.net/debian wheezy-backports main' > /etc/apt/sources.list.d/wheezy-backports-main.list
curl -s https://packagecloud.io/install/repositories/github/git-lfs/script.deb.sh | sudo bash
sudo apt-get install git-lfs
git lfs install

### On MacOs:
brew install git-lfs
git lfs install
```

When you are done, create the SD card. You will need to create two partition on it:
  * One around 524Mo in FAT format
  * The other around 2,7Go in Ext3 format

You can use the method of your choice to create those partition.
Then, it's time to write data on your SD card:

```
shell# mount /dev/sdc1 /mnt/usb1
shell# mount /dev/sdc2 /mnt/usb2
shell# cp target/de0-nano-soc/boot/* /mnt/usb1
shell# dd if=target/de0-nano-soc/u-boot/u-boot-spl.sfp of=/dev/sdc3 bs=64k seek=0
shell# dd if=target/de0-nano-soc/u-boot/u-boot.img of=/dev/sdc3 bs=64k seek=4
shell# tar xfz debian8-rootfs-vanilla.tgz -C /mnt/usb2
shell# cp fpga-soc-linux-drivers-4.4.7-armv7-fpga_0.0.2-1_armhf.deb /mnt/usb2/home/fpga
shell# umount mnt/usb1
shell# umount mnt/usb2
```

OR,
==

You can run the script robot.sh provided in robot_filesystem folder.

```
sudo ./robot.sh
```

Just provides data when asked (path to a valid rootfs and path to your sd card).

Note:
---
Maybe we can reduce the size of our image. For example:
  * 1Mo: Uboot
  * 256Mo: RBF + others stuffs
  * 256Mo: RootFs

and then use the command:
```
sudo resize2fs /dev/mmcblk0p2

```

to enlarge rootfs partition.

Now, you'r done with the SD card. You have a nice Debian distribution working on the DE0 nano SoC.

Change the RBF
===
You can change the default RBF file that come with the distribution above.
Just create your sof file that fit your needs with Quartus Tools.
Convert the sof file to a rbf file using the command:
```
quartus_cpf -c -o bitstream_compression=on DE0_NANO_SOC.sof DE0_NANO_SOC.rbf
```

Replace the rbf with yours on the first partition of your SD card.

Setup the CrossCompile Toolchain:
====

1- Get Arm Cross Compiler: GCC

Get the latest gcc compiler for ARM:

We will be using the standard Linaro GCC toolchain for the ARMv7 instruction set
to compile our projects.

```
wget -c https://releases.linaro.org/components/toolchain/binaries/6.2-2016.11/arm-linux-gnueabihf/gcc-linaro-6.2.1-2016.11-i686_arm-linux-gnueabihf.tar.xz
tar -xvf gcc-linaro-6.2.1-2016.11-i686_arm-linux-gnueabihf.tar.xz
export CROSS_COMPILE=$PWD/gcc-linaro-6.2.1-2016.11-i686_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
```

For others gcc version, go to:
https://releases.linaro.org/components/toolchain/binaries/6.2-2016.11/arm-linux-gnueabihf/

Or, simply run:

```
sudo apt-get install gcc-arm-linux-gnueabi
```

2- Get Linux Kernel from sources

Note:
---

We use Kernel 4.5.x

Tested:

  * Kernel sources

```
git clone https://github.com/altera-opensource/linux-socfpga.git
cd linux-socfpga
git tag
git checkout rel_socfpga-4.5_16.06.01_pr
```
  * Configuring the Kernel

```
make CROSS_COMPILE=arm-linux-gnueabi- ARCH=arm socfpga_defconfig
```

Maybe on some system you will need to install extra packages:

```
sudo apt-get install libncurses5-dev

or,

sudo dpkg --add-architecture i386 sudo apt-get update sudo apt-get install libc6:i386 libstdc++6:i386 libfontconfig1:i386 libfreetype6:i386 libice6:i386 lib32ncurses5 zlib1g:i386 libusb-0.1-4:i386
```

After that, Open the Kernel configuration tool:

```
make CROSS_COMPILE=arm-linux-gnueabi- ARCH=arm menuconfig
```


Note:
---

Go into the “General Setup” menu. Uncheck “Automatically append version information to the version string”. This will prevent the kernel from adding extra “version” information to the kernel.

And, enter the “Enable the block layer” menu option and enable the “Support for large (2TB+) block devices and files” option. Although the chances of you actually having 2TB+ files on your filesystem are small, if you look at the help for this option (press “?”) you’ll notice that this option is required to be enabled if you’re using the EXT4 filesystem (which we are). If you forget to enable this option, the kernel will mount your filesystem in read-only mode and print out a helpful message reminding you to come back and enable this if you want full read/write support.

When you’re done looking at the available options, hit the right arrow key to select the “Save” option at the bottom of the window and press enter. When asked for a filename, leave it at the default (“.config”) and hit enter. Hit enter again, then exit the configuration tool.

  * Compiling the Kernel and create zImage

```
make CROSS_COMPILE=arm-linux-gnueabi- ARCH=arm LOCALVERSION= zImage
```

Use the zImage created to make your SD card.

The part below is not tested and is here for information if we decide to try other linux kernel from others sources:
==

```
git clone https://github.com/RobertCNelson/socfpga-kernel-dev
cd socfpga-kernel-dev/

git checkout origin/v4.4.x -b LOCAL-BRANCH-NAME
```

You can checkout other version. For example:

For 4.6.x (Stable)

```
git checkout origin/v4.6.x -b LOCAL-BRANCH-NAME
```

For 4.7.x (Prepatch)

```
git checkout origin/v4.7.x -b LOCAL-BRANCH-NAME
```

3- Build the kernel

```
./build_kernel.sh
```
