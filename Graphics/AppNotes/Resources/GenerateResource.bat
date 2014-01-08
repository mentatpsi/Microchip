@echo off
REM set the project name
@set PROJECTNAME=%ObjectLayer
REM Set the path to the Graphics Resource Converter (grc.jar)
REM Check for presence of files.
if not exist "../../../Microchip/Graphics/Fonts/firefly/fireflysung.ttf" (
echo ERROR: fireflysung.ttf font file missing please look at "\AN1182 Resources\FontFilter\Loading Fonts ReadMe.txt" on how to obtain the file. 
echo GRC will not generate the files of the XML project that has the missing file.
)
if not exist "../../../Microchip/Graphics/Fonts/unbatang/UnBatang_0613.ttf" (
echo ERROR: UnBatang_0613.ttf font file missing please look at "\AN1182 Resources\FontFilter\Loading Fonts ReadMe.txt" on how to obtain the file. 
echo GRC will not generate the files of the XML project that has the missing file.
)
@set GRCPATH=%../../../Microchip/Graphics/bin/grc
java -jar %GRCPATH%/grc.jar -XML "AN1136 Resources"/"AN1136_graphics_resources.xml" -B XC16 -O "../AN1136 Pictures XC16.c"
java -jar %GRCPATH%/grc.jar -XML "AN1136 Resources"/"AN1136_graphics_resources.xml" -B XC32 -O "../AN1136 Pictures XC32.c"
java -jar %GRCPATH%/grc.jar -XML "AN1182 Resources"/"AN1182_graphics_resources.xml" -B XC16 -O "../AN1182 Pictures XC16.c"
java -jar %GRCPATH%/grc.jar -XML "AN1182 Resources"/"AN1182_graphics_resources.xml" -B XC32 -O "../AN1182 Pictures XC32.c"
java -jar %GRCPATH%/grc.jar -XML "AN1182 Resources"/FontFilter/"AN1182Fonts.xml" -B XC16 -O "../HelloWorldFonts XC16.c"
java -jar %GRCPATH%/grc.jar -XML "AN1182 Resources"/FontFilter/"AN1182Fonts.xml" -B XC32 -O "../HelloWorldFonts XC32.c"
java -jar %GRCPATH%/grc.jar -XML "Collated AppNotes.xml" -B XC16 -O "../AppNotesDemoResources XC16.c"
java -jar %GRCPATH%/grc.jar -XML "Collated AppNotes.xml" -B XC32 -O "../AppNotesDemoResources XC32.c"
