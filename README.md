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
