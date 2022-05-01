#!/bin/sh

echo Obtaining required packages...
apt-get -qq update
apt-get -qq install build-essential
apt-get -qq install libusb-1.0-0-dev
echo Copying source code files...
mkdir -p /usr/local/src/itusb1/man
cp -f src/cp2130.cpp /usr/local/src/itusb1/.
cp -f src/cp2130.h /usr/local/src/itusb1/.
cp -f src/error.cpp /usr/local/src/itusb1/.
cp -f src/error.h /usr/local/src/itusb1/.
cp -f src/GPL.txt /usr/local/src/itusb1/.
cp -f src/itusb1-attach.cpp /usr/local/src/itusb1/.
cp -f src/itusb1-detach.cpp /usr/local/src/itusb1/.
cp -f src/itusb1device.cpp /usr/local/src/itusb1/.
cp -f src/itusb1device.h /usr/local/src/itusb1/.
cp -f src/itusb1-info.cpp /usr/local/src/itusb1/.
cp -f src/itusb1-list.cpp /usr/local/src/itusb1/.
cp -f src/itusb1-lockotp.cpp /usr/local/src/itusb1/.
cp -f src/itusb1-reset.cpp /usr/local/src/itusb1/.
cp -f src/itusb1-status.cpp /usr/local/src/itusb1/.
cp -f src/itusb1-udoff.cpp /usr/local/src/itusb1/.
cp -f src/itusb1-udon.cpp /usr/local/src/itusb1/.
cp -f src/itusb1-upoff.cpp /usr/local/src/itusb1/.
cp -f src/itusb1-upon.cpp /usr/local/src/itusb1/.
cp -f src/LGPL.txt /usr/local/src/itusb1/.
cp -f src/libusb-extra.c /usr/local/src/itusb1/.
cp -f src/libusb-extra.h /usr/local/src/itusb1/.
cp -f src/Makefile /usr/local/src/itusb1/.
cp -f src/man/itusb1-attach.1 /usr/local/src/itusb1/man/.
cp -f src/man/itusb1-detach.1 /usr/local/src/itusb1/man/.
cp -f src/man/itusb1-info.1 /usr/local/src/itusb1/man/.
cp -f src/man/itusb1-list.1 /usr/local/src/itusb1/man/.
cp -f src/man/itusb1-lockotp.1 /usr/local/src/itusb1/man/.
cp -f src/man/itusb1-reset.1 /usr/local/src/itusb1/man/.
cp -f src/man/itusb1-status.1 /usr/local/src/itusb1/man/.
cp -f src/man/itusb1-udoff.1 /usr/local/src/itusb1/man/.
cp -f src/man/itusb1-udon.1 /usr/local/src/itusb1/man/.
cp -f src/man/itusb1-upoff.1 /usr/local/src/itusb1/man/.
cp -f src/man/itusb1-upon.1 /usr/local/src/itusb1/man/.
cp -f src/README.txt /usr/local/src/itusb1/.
echo Building and installing binaries and man pages...
make -C /usr/local/src/itusb1 install clean
echo Applying configurations...
cat > /etc/udev/rules.d/70-bgtn-itusb1.rules << EOF
SUBSYSTEM=="usb", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="8c96", MODE="0666"
SUBSYSTEM=="usb_device", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="8c96", MODE="0666"
EOF
service udev restart
echo Done!
