@echo off
REM This batch file should be run inside the project directory.
REM set the project name
@set PROJECTNAME=%RemoteControl
REM Set the path to the Graphics Resource Converter (grc.jar)
@set GRCPATH=%../../../../Microchip/Graphics/bin/grc
java -jar %GRCPATH%/grc.jar -XML InternalResource%PROJECTNAME%.xml -B XC16 -O ../InternalResourceXC16.c
java -jar %GRCPATH%/grc.jar -XML ExternalResource%PROJECTNAME%.xml -O ../ExternalResource.hex
move ..\ExternalResource.hex ExternalResource.hex
del ExternalResource-%PROJECTNAME%.hex
rename ExternalResource.hex ExternalResource-%PROJECTNAME%.hex

