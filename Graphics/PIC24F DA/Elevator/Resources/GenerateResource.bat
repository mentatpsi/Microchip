@echo OFF
REM set the project name
@set PROJECTNAME=%Elevator
REM Set the path to the Graphics Resource Converter (grc.jar)
@set GRCPATH=%../../../../Microchip/Graphics/bin/grc
java -jar %GRCPATH%/grc.jar -XML "InternalResource%PROJECTNAME%.xml" -B XC16 -O ../InternalResourceXC16.c
java -jar %GRCPATH%/grc.jar -XML "ExternalResource%PROJECTNAME%_ParallelFlash480x272.xml" -CS 2 -O ../ExternalResourceParallelFlash480x272.hex
java -jar %GRCPATH%/grc.jar -XML "ExternalResource%PROJECTNAME%_SPIFlash272x480.xml" -O ../ExternalResourceSPIFlash272x480.hex

move ..\ExternalResourceParallelFlash480x272.hex ExternalResource.hex
del ExternalResource-%PROJECTNAME%_ParallelFlash480x272.hex
rename ExternalResource.hex ExternalResource-%PROJECTNAME%_ParallelFlash480x272.hex

move ..\ExternalResourceSPIFlash272x480.hex ExternalResource.hex
del ExternalResource-%PROJECTNAME%_SPIFlash272x480.hex
rename ExternalResource.hex ExternalResource-%PROJECTNAME%_SPIFlash272x480.hex
