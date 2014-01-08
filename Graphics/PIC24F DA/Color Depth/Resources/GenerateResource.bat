@echo OFF
REM set the project name
@set PROJECTNAME=%ColorDepth
REM Set the path to the Graphics Resource Converter (grc.jar)
@set GRCPATH=%../../../../Microchip/Graphics/bin/grc
java -jar %GRCPATH%/grc.jar -XML 1BPP/"InternalResource%PROJECTNAME%_1BPPMode.xml" -B XC16 -O ../InternalResourceXC16_1BPPMode.c
java -jar %GRCPATH%/grc.jar -XML 4BPP/"InternalResource%PROJECTNAME%_4BPPMode.xml" -B XC16 -O ../InternalResourceXC16_4BPPMode.c
java -jar %GRCPATH%/grc.jar -XML 8BPP/"InternalResource%PROJECTNAME%_8BPPMode.xml" -B XC16 -O ../InternalResourceXC16_8BPPMode.c
java -jar %GRCPATH%/grc.jar -XML 1BPP/"InternalResource%PROJECTNAME%_1BPPModeCompressed.xml" -B XC16 -O ../InternalResourceXC16_1BPPModeIPU.c
java -jar %GRCPATH%/grc.jar -XML 4BPP/"InternalResource%PROJECTNAME%_4BPPModeCompressed.xml" -B XC16 -O ../InternalResourceXC16_4BPPModeIPU.c
java -jar %GRCPATH%/grc.jar -XML 8BPP/"InternalResource%PROJECTNAME%_8BPPModeCompressed.xml" -B XC16 -O ../InternalResourceXC16_8BPPModeIPU.c
