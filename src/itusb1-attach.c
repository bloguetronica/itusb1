/* ITUSB1 Attach Command - Version 1.2 for Debian Linux
   Copyright (c) 2019-2021 Samuel Lourenço

   This program is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation, either version 3 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   You should have received a copy of the GNU General Public License along
   with this program.  If not, see <https://www.gnu.org/licenses/>.


   Please feel free to contact me via e-mail: samuel.fmlourenco@gmail.com */


// Includes
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libusb-1.0/libusb.h>
#include "itusb1-core.h"
#include "libusb-extra.h"

// Function prototypes
void attach(libusb_device_handle *devhandle);

int main(int argc, char **argv)
{
    err_level = EXIT_SUCCESS;  // Note that this variable is declared externally!
    libusb_context *context;
    if (libusb_init(&context) != 0) {  // Initialize libusb. In case of failure
        fprintf(stderr, "Error: Could not initialize libusb.\n");
        err_level = EXIT_FAILURE;
    } else {  // If libusb is initialized
        libusb_device_handle *devhandle;
        if (argc < 2) {  // If the program was called without arguments
            devhandle = libusb_open_device_with_vid_pid(context, VID, PID);  // Open a device and get the device handle
        } else {  // Serial number was specified as argument
            devhandle = libusb_open_device_with_vid_pid_serial(context, VID, PID, (unsigned char *)argv[1]);  // Open the device having the specified serial number, and get the device handle
        }
        if (devhandle == NULL) {  // If the previous operation fails to get a device handle
            fprintf(stderr, "Error: Could not find device.\n");
            err_level = EXIT_FAILURE;
        } else {  // If the device is successfully opened and a handle obtained
            bool kernel_attached = false;
            if (libusb_kernel_driver_active(devhandle, 0) != 0) {  // If a kernel driver is active on the interface
                libusb_detach_kernel_driver(devhandle, 0);  // Detach the kernel driver
                kernel_attached = true;  // Flag that the kernel driver was attached
            }
            if (libusb_claim_interface(devhandle, 0) != 0) {  // Claim the interface. In case of failure
                fprintf(stderr, "Error: Device is currently unavailable.\n");
                err_level = EXIT_FAILURE;
            } else {  // If the interface is successfully claimed
                attach(devhandle);  // Attach DUT to HUT
                if (err_level == EXIT_SUCCESS) {  // If all goes well
                    printf("USB device attached.\n");
                }
                libusb_release_interface(devhandle, 0);  // Release the interface
            }
            if (kernel_attached) {  // If a kernel driver was attached to the interface before
                libusb_attach_kernel_driver(devhandle, 0);  // Reattach the kernel driver
            }
            libusb_close(devhandle);  // Close the device
        }
        libusb_exit(context);  // Deinitialize libusb
    }
    return err_level;
}

// Attaches the DUT (device under test) to the HUT (host under test)
void attach(libusb_device_handle *devhandle)
{
    if (get_gpio1(devhandle) != get_gpio2(devhandle)) {  // If GPIO.1 and GPIO.2 pins do not match in value, indicating an unusual state
        set_gpio1(devhandle, true);  // Set GPIO.1 to a logical high to switch off VBUS first
        set_gpio2(devhandle, true);  // Set GPIO.2 to a logical high to disconnect the data lines
        usleep(100000);  // Wait 100ms to allow for device shutdown
    }
    if (get_gpio1(devhandle) && get_gpio2(devhandle)) {  // If GPIO.1 and GPIO.2 are both set to a logical high
        set_gpio1(devhandle, false);  // Set GPIO.1 to a logical low to switch VBUS on
        usleep(100000);  // Wait 100ms in order to emulate a manual attachment of the device
        set_gpio2(devhandle, false);  // Set GPIO.2 to a logical low to connect the data lines
        usleep(100000);  // Wait 100ms so that device enumeration process can, at least, start (this is not enough to guarantee enumeration, though)
    }
}
