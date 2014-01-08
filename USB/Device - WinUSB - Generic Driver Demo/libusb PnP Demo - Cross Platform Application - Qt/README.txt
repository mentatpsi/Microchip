This is a cross platform project that is similar in functionality as the
"WinUSB PnP Demo - PC Application - MS VC++ 2008 Express" project.  However, 
this version is developed in the Qt development environment (from qt.nokia.com), 
and uses the "libusb v1.0" APIs.

The libusb v1.0 "driver" provides top level libusb API functions for accessing 
the USB device, but uses the Microsoft WinUSB driver when the application is 
used on the Windows platform.  Therefore, when this project is built and run 
on a Windows based machine, install and use the standard WinUSB driver package 
that comes with this demo, even when using this libusb application project.

The libusb-1.0.dll file and respective .lib file provided here were built 
from the v1.0.9 source.  See www.libusb.org for details on the libusb APIs 
and links to the libusb source code and header download.

If using this project to build a Linux or Mac OS X executable, make sure
that the libusb v1.0.9 driver is available and installed correctly.