del "Compile All.bat" /f /q
del "*.$$$" /f /q
del "*.bkx" /f /q
del "*.cod" /f /q
del "*.cof" /f /q
del "*.elf" /f /q
del "*.i" /f /q
del "*.sym" /f /q
del "*.wat" /f /q
del "*.lde" /f /q
del "*.hxl" /f /q
del "*.mcs" /f /q
del "*.tagsrc" /f /q
del "*.mptags" /f /q
del "untitled.mcw" /f /q
del "*.map" /f /q
del "*.err" /f /q
del "*.o" /f /q
del "*.obj" /f /q
del "*.rlf" /f /q
del "*.sdb" /f /q
del "*.lst" /f /q
del "*.cce" /f /q
del "*.p1" /f /q
del "*.pre" /f /q
del funclist
del "RIUSBLogFile.txt" /f /q
del "RIStreaming.bin" /f /q
del "TCPIPConfig.h.bak"
del ".\Linker\RIUSBLogFile.txt" /f /q
del "..\Microchip\TCPIP Stack\*.err" /f /q
del "..\Microchip\TCPIP Stack\*.obj" /f /q
del "..\Microchip\TCPIP Stack\*.o" /f /q
del "..\Microchip\TCPIP Stack\*.rlf" /f /q
del "..\Microchip\TCPIP Stack\*.sdb" /f /q
del "..\Microchip\TCPIP Stack\*.lst" /f /q
del "..\Microchip\TCPIP Stack\*.cce" /f /q
del "..\Microchip\TCPIP Stack\*.i" /f /q
del "..\Microchip\Include\RIUSBLogFile.txt" /f /q
del "..\Microchip\TCPIP Stack\RIUSBLogFile.txt" /f /q
del "..\Microchip\TCPIP Stack\RIStreaming.bin" /f /q
del "..\Microchip\TCPIP Stack\*.p1" /f /q
del "Makefile" /f /q
del "a.out" /f /q
rmdir "TempOBJ" /s /q
rmdir "Obj-EM PICtail-PIC24F ENC28" /s /q
rmdir "Obj-EM PICtail-PIC24F ENC624" /s /q
rmdir "..\Microchip\TCPIP Stack\Utilities\Source\Microchip Ethernet Discoverer\publish" /s /q
rmdir "..\Microchip\TCPIP Stack\Utilities\Source\Microchip Ethernet Discoverer\obj" /s /q
rmdir "..\Microchip\TCPIP Stack\Utilities\Source\Microchip Ethernet Discoverer\bin\Debug" /s /q


IF %1==deletesvn (
FOR /R "..\." %%s IN (.) DO rmdir "%%s\.svn" /s /q
)