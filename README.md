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

When you are done,
