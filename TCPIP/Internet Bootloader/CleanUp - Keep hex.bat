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
del ".\Linker\RIUSBLogFile.txt" /f /q
rmdir "Obj-Internet Bootloader" /s /q

IF %1==deletesvn (
	FOR /R "..\." %%s IN (.) DO rmdir "%%s\.svn" /s /q
)