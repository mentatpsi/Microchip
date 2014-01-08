@echo off
REM set the project name
@set PROJECTNAME=%S1D13517
REM Set the path to the Graphics Resource Converter (grc.jar)
@set GRCPATH=%../../../Microchip/Graphics/bin/grc
java -jar %GRCPATH%/grc.jar -XML "InternalResource-XC16-%PROJECTNAME%.xml" -B XC16 -O ../InternalResourceXC16.c
java -jar %GRCPATH%/grc.jar -XML "InternalResource-XC32-%PROJECTNAME%.xml" -B XC32 -O ../InternalResourceXC32.c
java -jar %GRCPATH%/grc.jar -XML "ExternalResource%PROJECTNAME%.xml"  -O ../ExternalResource.hex
move ..\ExternalResource.hex ExternalResource.hex
del ExternalResource-%PROJECTNAME%.hex
rename ExternalResource.hex ExternalResource-%PROJECTNAME%.hex

