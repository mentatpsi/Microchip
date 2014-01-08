MiWi DE 4.2.5
Add support for XC16 and XC32 compilers. Fix bug that associated with symbol timer for PIC32. 

MiWi DE 4.2.4
Fix the bug in corner cases. User for previous versions generally do not need to update.
Further simplified demos and focus on MiApp interfaces in demo main function.

MiWi DE 4.2.3
Fix the bug in MiWi P2P that two MRF24J40 nodes cannot connect if Network Freezer is not enabled.

MiWi DE 4.2.2
Add out-of-box support for MRF24J40MC module

MiWi DE 4.2
MiWi DE 4.2 is latest release with Microchip Applicaiton Libraries. There is no difference between version 4.2 and version 4.1.2Beta release directly from Microchip MiWi docking page, except some minor bug fixes.

MiWi DE 4.1.2Beta
MiWi DE 4.1.2Beta is a major release after MiWi DE 3.x. The major changes are:
	1. Add support for MiWi PRO protocol that supports large networks up to 65 hops
	2. Add initial support for MPLAB X Beta 6
	3. Provide demo source code for Microchip Wireless Development Kit 
	4. Provide testing interface for MiWi PRO protocol 
	5. Bug fixes.

MiWi DE 3.1
MiWi DE v3.1 is latest update to P2P stack v2.1.1 with MiMAC and MiApp interfaces. The major changes are:
	1. Add support for MRF89XA
	2. Add support for MiWi protocol
	3. Add new features, such as Network Freezer, Enhanced Data Request and Time Synchronization.

Please read "Getting Started with MiWi DE.chm" for latest information regarding v3.1.x release of 
MiWi DE stack.


MiWi DE 2.1.1
MiWi P2P stack v2.1.1 is a minor bug fix for v2.1.0. The only two changes are:
	1. Fix a typo in MRF49XA driver in security mode
	2. Fix the project file for PIC24/dsPIC33 so no missing files will be shown


MiWi DE 2.1
MiWi P2P stack v2.1 is the first stack to use Microchip MiMAC and MiApp interfaces. Major changes are:
	1. Use MiMAC and MiApp interfaces.
	2. Add support for MRF49XA
	3. Support MiWi P2P stack


MiWi P2P 1.0.x
MiWi P2P stack support one hop communication between devices in star or peer-to-peer topology. Major 
functionalities are:
	1. Support MiWi P2P
	2. Support MRF24J40
	3. Support rich feature lists such as active scan, energy scan and frequency agility.




In case you encountered problem to open Microsoft Compiled HTML Help file with file extension "chm", 
the registration of Microsoft Compiled HTML Help file reader may be broken. To fix this problem, 
please follow instructions below:

1. open a CMD window.
2. Type following on the command line:
regsvr32 %systemroot%\system32\hhctrl.ocx
regsvr32 %systemroot%\system32\itss.dll 

You should be able to see window pop up to indicate operation status.

3. Try to open the Microsoft Compiled HTML Help file with file extension "chm" again.

In case you still cannot open the chm file, contact your system administrator for help.
