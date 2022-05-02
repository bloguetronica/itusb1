This directory contains all source code files required for compiling the
commands for ITUSB1 USB Test Switch. A list of relevant files follows:
– cp2130.cpp;
– cp2130.h;
– error.cpp;
– error.h;
– itusb1-attach.cpp;
– itusb1-detach.cpp;
– itusb1device.cpp;
– itusb1device.h;
– itusb1-info.cpp;
– itusb1-list.cpp;
– itusb1-lockotp.cpp;
– itusb1-reset.cpp;
– itusb1-status.cpp;
– itusb1-udoff.cpp;
– itusb1-udon.cpp;
– itusb1-upoff.cpp;
– itusb1-upon.cpp;
– libusb-extra.c;
– libusb-extra.h;
– man/itusb1-attach.1;
– man/itusb1-detach.1;
– man/itusb1-info.1;
– man/itusb1-list.1;
– man/itusb1-lockotp.1;
– man/itusb1-reset.1;
– man/itusb1-status.1;
– man/itusb1-udoff.1;
– man/itusb1-udon.1;
– man/itusb1-upoff.1;
– man/itusb1-upon.1;
– Makefile.

In order to compile successfully all commands, you must have the packages
"build-essential" and "libusb-1.0-0-dev" installed. Given that, if you wish to
simply compile, change your working directory to the current one on a terminal
window, and simply invoke "make" or "make all". If you wish to install besides
compiling, run "sudo make install". Alternatively, if you wish to force a
rebuild, you should invoke "make clean all", or "sudo make clean install" if
you prefer to install after rebuilding.

It may be necessary to undo any previous operations. Invoking "make clean"
will delete all object code generated (binaries included) during earlier
compilations. You can also invoke "sudo make uninstall" to unistall the
binaries.

P.S.:
Notice that any make operation containing the targets "install" or "uninstall"
(e.g. "make all install" or "make uninstall") requires root permissions, or in
other words, must be run with sudo.
