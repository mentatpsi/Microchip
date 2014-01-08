@echo off
REM set the project name
@set PROJECTNAME=%PrimitiveLayer
REM Set the path to the Graphics Resource Converter (grc.jar)
@set GRCPATH=%../../../Microchip/Graphics/bin/grc
java -jar %GRCPATH%/grc.jar -XML "InternalResource%PROJECTNAME%.xml" -B XC16 -O ../InternalResourceXC16.c
java -jar %GRCPATH%/grc.jar -XML "InternalResource%PROJECTNAME%.xml" -B XC32 -O ../InternalResourceXC32.c
