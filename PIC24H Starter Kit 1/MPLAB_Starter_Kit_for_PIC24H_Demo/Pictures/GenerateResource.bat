@set GRCPATH=%../../../Microchip/Graphics/bin/grc
java -jar %GRCPATH%/grc.jar -XML "PIC24HStarterKitFontResources.xml" -B XC16 -O ../src/Gentium8.c
java -jar %GRCPATH%/grc.jar -XML "PIC24HStarterKitResources.xml" -B XC16 -O ../src/Pictures.c
move ../src/Gentium8.h ../h/Gentium8.h
move ../src/Pictures.h ../h/Pictures.h

