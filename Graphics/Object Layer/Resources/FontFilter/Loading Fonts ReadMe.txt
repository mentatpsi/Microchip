The Object Layer Demo shows an example on how to use multiple font tables in one application.
Due to the size of the font (*.ttf), the files are not distributed with the demo.
The fonts used are the following:

There are two font tables generated using the fireflysung.ttf.
FireFlySung obtained from
http://www.unifont.org/fontguide

Each font table is generated using the filter files:
1. MainDemoLargeChineseFontFilter.txt - assigned to GOLFontDefault (use pixel height = 17).
2. MainDemoSmallChineseFontFilter.txt - assigned to GOLSmallFont (use pixel height = 15).

Another font table is generated using the VeraMono.ttf.
VeraMono.ttf obtained from
http://www-old.gnome.org/fonts/

All these font filter text files are saved as Unicode with 16 bit encoding. This can be done by
specifying the encoding when saving the text file on a text file editor such as BabelPad.
http://www.babelstone.co.uk/Software/BabelPad.html

Refer to the "Graphics Resource Converter Tool" (grc.java) help file or user's manual 
for instructions on how to create filtered font tables.

IMPORTANT:
The GRC project GOLDemoFont.xml is included in this directory to show how the fonts
are generated. Before opening the project in the grc.java download the required (*.ttf) font
files first and place the font files in their respective directories in the 
<install dir>/Microchip/Graphics/Fonts directory. Each font will have its own sub-directory.

1. <install dir>/Microchip/Graphics/Fonts/firefly/fireflysung.ttf
2. <install dir>/Microchip/Graphics/Fonts/vera/VeraMono.ttf
where: install_dir is the location where the Microchip Application Libraries (MLA) are installed.

It is also important that the file names of the *.ttf are the same. If they are not
the grc.java will generate an error when you load the project files.
The file names used is:
1. fireflysung.ttf

