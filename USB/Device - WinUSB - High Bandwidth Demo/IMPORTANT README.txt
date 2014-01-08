The HighBandwidthWinUSB.exe program was written in Microsoft Visual C++ 2005 Express Edition.

Therefore, in order to use this program, you will need to have the .NET framework version 2.0
(or later) installed on your computer.  If you do not have the .NET framework 2.0 installed, 
a non-descript error message will occur when trying to launch the executable, and the program 
will not open.  You may also need to install the Microsoft Visual C++ 2005 Redistributeable Package.

You may already have the .NET framework installed on your PC, especially if you have already installed
other applications which were built with one of the Visual Studio 2005 .NET languages.  If you do not
yet have it, the .NET framework can be freely downloaded from Microsoft's website.  Users of Windows Vista
do not need to install the .NET framework, as it comes pre-installed as part of the OS.

The redistributables are currently (22 May 2008) available at:
http://www.microsoft.com/downloads/details.aspx?FamilyID=0856EACB-4362-4B0D-8EDD-AAB15C5E04F5&displaylang=en
http://www.microsoft.com/downloads/details.aspx?familyid=32bc1bee-a3f9-4c13-9c99-220b62a191ee&displaylang=en



In order to build the PC application source code, you will need to install:
1. Microsoft Visual C++ 2005 Express Edition (other editions or later versions are believed to work
	however, this is not tested, and once the project is converted over to a newer version or
	more advanced version, the project will probably not be openable in 2005 Express Edition anymore). 
2. Windows Server 2003 R2 Platform SDK (and follow Microsoft's recommendations for integrating it with the
	VC++ 2005 IDE, which invovles updating the include path among other things)
3. Windows Driver Development Kit (WDK) build 7600.16385.0. (also update VC++ 2005 IDE include paths)

All of these tools are currently distributed by Microsoft free of charge.  Currently (22 May 2008), they 
can be downloaded from:

1. http://www.microsoft.com/express/2005/download/default.aspx
2. http://www.microsoft.com/downloads/details.aspx?familyid=484269E2-3B89-47E3-8EB7-1F2BE6D7123A&displaylang=en
3. http://connect.microsoft.com (must get passport login and join the WDK "connection")


It may also be useful to watch the tutorial videos provided here (shows how to integrate Platform SDK with VC++ IDE):

http://msdn.microsoft.com/en-us/visualc/aa336415.aspx

