@echo OFF
REM set the project name
@set PROJECTNAME=%MultiApp
REM Set the path to the Graphics Resource Converter (grc.jar)
@set GRCPATH=%../../../Microchip/Graphics/bin/grc
REM Check for presence of files.
if not exist "../../../Microchip/Graphics/Fonts/firefly/fireflysung.ttf" (
echo ERROR: fireflysung.ttf font file missing please look at "GMAP Demo ReadMe.txt" on how to obtain the file. 
echo GRC will not generate the files of the XML project that has the missing file.
)
if not exist "../../../Microchip/Graphics/Fonts/unbatang/UnBatang_0613.ttf" (
echo ERROR: UnBatang_0613.ttf font file missing please look at "GMAP Demo ReadMe.txt" on how to obtain the file. 
echo GRC will not generate the files of the XML project that has the missing file.
)
java -jar %GRCPATH%/grc.jar -XML "InternalResource%PROJECTNAME%.xml" -B XC16 -O ../InternalResourceXC16.c
java -jar %GRCPATH%/grc.jar -XML "InternalResource%PROJECTNAME%Set2.xml" -B XC16 -O ../InternalResourceSet2XC16.c
java -jar %GRCPATH%/grc.jar -XML "InternalResource%PROJECTNAME%.xml" -B XC32 -O ../InternalResourceXC32.c
java -jar %GRCPATH%/grc.jar -XML "InternalResource%PROJECTNAME%Set2.xml" -B XC32 -O ../InternalResourceSet2XC32.c
java -jar %GRCPATH%/grc.jar -XML "ExternalResource%PROJECTNAME%.xml" -CS 2 -O ../ExternalResource.hex
move ..\ExternalResource.hex ExternalResource.hex
del ExternalResource-%PROJECTNAME%.hex
rename ExternalResource.hex ExternalResource-%PROJECTNAME%.hex

