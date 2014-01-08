@echo off
REM set the project name
@set PROJECTNAME=%ObjectLayer
REM Set the path to the Graphics Resource Converter (grc.jar)
@set GRCPATH=%../../../Microchip/Graphics/bin/grc

if not exist "../../../Microchip/Graphics/Fonts/firefly/fireflysung.ttf" (
echo ERROR: fireflysung.ttf font file missing please look at "FontFilter/Loading Fonts ReadMe.txt" on how to obtain the file. 
echo GRC will not generate the files of the XML project that has the missing file.
)
if not exist "../../../Microchip/Graphics/Fonts/vera/VeraMono.ttf" (
echo ERROR: VeraMono.ttf font file missing please look at "FontFilter/Loading Fonts ReadMe.txt" on how to obtain the file. 
echo GRC will not generate the files of the XML project that has the missing file.
)

java -jar %GRCPATH%/grc.jar -XML "InternalResource%PROJECTNAME%.xml" -B XC16 -O ../InternalResourceXC16.c
java -jar %GRCPATH%/grc.jar -XML "InternalResource%PROJECTNAME%.xml" -B XC32 -O ../InternalResourceXC32.c
