@set GRCPATH=%../../Microchip/Graphics/bin/grc
java -jar %GRCPATH%/grc.jar -XML "PIC24FStarterKitFontImages.xml" -B XC16 -O ../PicturesFont.c
