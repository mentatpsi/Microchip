@echo OFF
REM This batch file should be run inside the project directory.
REM set the project name
@set PROJECTNAME=%PIC32LCC
REM Set the path to the Graphics Resource Converter (grc.jar)
@set GRCPATH=%../../../Microchip/Graphics/bin/grc
java -jar %GRCPATH%/grc.jar -XML "InternalResource%PROJECTNAME%.xml" -B XC32 -O ../InternalResources.c