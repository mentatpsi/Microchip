The files in this directory may be either programmed onto the microcontroller
directly using an ICSP(tm) based programmer, or, through USB by using the 
HID Bootloader.

To program these files with the HID Bootloader, first program the microcontroller with
the "USB Device - HID - HID Bootloader - C18 - PIC18F4550.hex" file using a standard
ICSP based programmer.  The HID Bootloader PC application can then be used to self 
program the other files in this directory.