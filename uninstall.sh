#!/bin/sh

echo Rolling back configurations...
rm -f /etc/udev/rules.d/70-bgtn-itusb1.rules
service udev restart
echo Removing binaries and man pages...
make -C /usr/local/src/itusb1 uninstall
echo Removing source code files...
rm -rf /usr/local/src/itusb1
echo Done!
