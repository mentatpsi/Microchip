@set GRCPATH=%../../../Microchip/Graphics/bin/grc
java -jar %GRCPATH%/grc.jar -XML "font_project.xml" -B XC16 -O ../gfx_font_resources_XC16.c
java -jar %GRCPATH%/grc.jar -XML "font_project.xml" -B XC32 -O ../gfx_font_resources_XC32.c
java -jar %GRCPATH%/grc.jar -XML "external_images.xml"  -O ../external_images.hex
move ..\external_images.hex external_images.hex
del ExternalResource-E-Paper.hex
rename external_images.hex ExternalResource-E-Paper.hex
