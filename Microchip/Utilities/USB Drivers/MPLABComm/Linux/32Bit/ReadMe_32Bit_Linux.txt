ReadMe_32Bit_Linux.txt
_____________________________________________________________________________

Most of the file placement below corresponds to MPLABComm use within MPLAB X.
The libusb and libUSBAccessLink libraries should be accessible from outside
of MPLAB X if placed in the /usr/lib/ subdirectory.

Other files listed below deal with tool hotplug support.

-----------------------------------------------------------------------------
File Placement (outside of MPLAB X)
-----------------------------------------------------------------------------
Dir:               File(s):                Permissions:
-----------------  ----------------------  -------------------
/usr/lib/          libUSBAccessLink.so     -rwxr-xr-x

/usr/lib/          libusb-1.0.so.0.0.0     -rwxr-xr-x
                   libusb-1.0.a            -rwxr-xr-x
                   libusb-1.0.la           -rwxr-xr-x

/etc/.mplab_ide    mchpdefport             -rw-r--r--
                   mchplinusbdevice        -rwxr-xr-x

/etc/udev/rules.d  z010_mchp_tools.rules   -rw-r--r-- (requires reboot)

-----------------------------------------------------------------------------
Create Soft Link(s):
-----------------------------------------------------------------------------
1) Link to libusb:

   $ cd /usr/lib
   $ sudo ln -s /usr/lib/libusb-1.0.so.0.0.0 libusb-1.0.so

