/* ITUSB1 Status Command - Version 1.1 for Debian Linux
   Copyright (c) 2019-2020 Samuel Lourenço

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

int main(int argc, char **argv)
{
    err_level = EXIT_SUCCESS;  // Note that this variable is declared externally!
    libusb_context *context;
    if (libusb_init(&context) != 0)  // Initialize libusb. In case of failure
    {
        fprintf(stderr, "Error: Could not initialize libusb.\n");
        err_level = EXIT_FAILURE;
    }
    else  // If libusb is initialized
    {
        libusb_device_handle *devhandle;
        if (argc < 2)  // If the program was called without arguments
            devhandle = libusb_open_device_with_vid_pid(context, 0x10C4, 0x8C96);  // Open a device and get the device handle
        else  // Serial number was specified as argument
            devhandle = libusb_open_device_with_vid_pid_serial(context, 0x10C4, 0x8C96, (unsigned char *)argv[1]);  // Open the device having the specified serial number, and get the device handle
        if (devhandle == NULL)  // If the previous operation fails to get a device handle
        {
            fprintf(stderr, "Error: Could not find device.\n");
            err_level = EXIT_FAILURE;
        }
        else  // If the device is successfully opened and a handle obtained
        {
            bool kernel_attached = false;
            if (libusb_kernel_driver_active(devhandle, 0) != 0)  // If a kernel driver is active on the interface
            {
                libusb_detach_kernel_driver(devhandle, 0);  // Detach the kernel driver
                kernel_attached = true;  // Flag that the kernel driver was attached
            }
            if (libusb_claim_interface(devhandle, 0) != 0)  // Claim the interface. In case of failure
            {
                fprintf(stderr, "Error: Device is currently unavailable.\n");
                err_level = EXIT_FAILURE;
            }
            else  // If the interface is successfully claimed
            {
                configure_spi_mode(devhandle, 0, CSMODEPP, CFRQ1500K, CPOL0, CPHA0);  // Chip select pin mode regarding channel 0 is push-pull, the clock frequency is set to 1.5MHz, clock polarity is active high (CPOL = 0) and data is valid on each rising edge (CPHA = 0)
                disable_spi_delays(devhandle, 0);  // Disable all SPI delays for channel 0
                bool nup = get_gpio1(devhandle);  // Get the current value of the GPIO.1 pin, which corresponds to the !UPEN signal
                bool nud = get_gpio2(devhandle);  // Get the current value of the GPIO.2 pin, which corresponds to the !UDEN signal
                bool noc = get_gpio3(devhandle);  // Get the current value of the GPIO.3 pin, which corresponds to the !UPOC signal
                select_cs(devhandle, 0);  // Enable the chip select corresponding to channel 0, and disable any others
                if (get_current(devhandle) == 0)  // If first reading (always to be discarded) is zero
                {
                    usleep(1100);  // Wait 1.1ms to ensure that the LTC2312 is awake
                    get_current(devhandle);  // Do a second reading and discard it as well
                }
                uint16_t curr_code_sum = 0;
                for (int i = 0; i < 5; ++i)
                {
                    curr_code_sum += get_current(devhandle);  // Read the raw value (from the LTC2312 on channel 0) and add it to the sum
                }
                usleep(100);  // Wait 100us, in order to prevent possible errors while disabling the chip select (workaround)
                disable_cs(devhandle, 0);  // Disable the previously enabled chip select
                float current = curr_code_sum / 20.0;  // Calculate the average current out of five readings (current = curr_code / 4.0 for a single reading)
                if (err_level == EXIT_SUCCESS)  // If all goes well
                {
                    printf("Status: Connection %s\n", nup || nud ? "disabled" : "enabled");  // Print USB connection status
                    printf("USB power: %s\n", nup ? "Disabled" : "Enabled");  // Print USB power status
                    printf("USB data: %s\n", nud ? "Disabled" : "Enabled");  // Print USB data status
                    printf("Current: ");
                    if (current < 1000)  // If the current reading is lesser than 1000mA
                    {
                        printf("%.1fmA", current);  // Print the current reading
                        if (current > 500)
                            printf(" (OC)");  // Print "(OC)" next to the value, to indicate that the current exceeds the 500mA limit established by the USB 2.0 specification (and also may cause a trip)
                    }
                    else  // Otherwise
                        printf("OL");  // Print "OL" to indicate an out of limits reading
                    printf("\n");  // End printing with a newline
                    if (!noc)  // If GPIO.3 reads a logical low
                        printf("Warning: Fault detected!\n");  // Over-current, over-temperature ou under-voltage trip condition detected
                }
                libusb_release_interface(devhandle, 0);  // Release the interface
            }
            if (kernel_attached)  // If a kernel driver was attached to the interface before
                libusb_attach_kernel_driver(devhandle, 0);  // Reattach the kernel driver
            libusb_close(devhandle);  // Close the device
        }
        libusb_exit(context);  // Deinitialize libusb
    }
    return err_level;
}
