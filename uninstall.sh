#!/bin/sh

echo Rolling back configurations...
rm -f /etc/udev/rules.d/70-bgtn-itusb1.rules
service udev restart
echo Removing man pages...
rm -f /usr/local/share/man/man1/itusb1-attach.1.gz
rm -f /usr/local/share/man/man1/itusb1-detach.1.gz
rm -f /usr/local/share/man/man1/itusb1-list.1.gz
rm -f /usr/local/share/man/man1/itusb1-lockotp.1.gz
rm -f /usr/local/share/man/man1/itusb1-reset.1.gz
rm -f /usr/local/share/man/man1/itusb1-status.1.gz
rm -f /usr/local/share/man/man1/itusb1-udoff.1.gz
rm -f /usr/local/share/man/man1/itusb1-udon.1.gz
rm -f /usr/local/share/man/man1/itusb1-upoff.1.gz
rm -f /usr/local/share/man/man1/itusb1-upon.1.gz
rmdir --ignore-fail-on-non-empty /usr/local/share/man/man1
echo Removing binaries...
make -C /usr/local/src/itusb1 uninstall
echo Removing source code files...
rm -rf /usr/local/src/itusb1
echo Done!
