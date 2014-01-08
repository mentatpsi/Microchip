
The Multi-app Demo shows an example on how to use multiple font tables in one application.
Due to the size of the font (*.ttf), the files are not distributed with the demo.
The fonts used are the following:

For French, Italian, German, Dutch, English : used Gentium taken from
http://scripts.sil.org/cms/scripts/page.php?site_id=nrsi&cat_id=FontDownloads 

For Chinese, Japanese : used  FireFlySung taken from
http://www.unifont.org/fontguide
http://www.study-area.org/apt/firefly-font/

For Russian, Korean : used  UnBatang taken from
http://www.unifont.org/fontguide
http://kldp.net/projects/unfonts/download

Gentium Font table is generated for character ID range of 28 - 127.
The default font table in the Graphics Library is using this font.
For French, Italian, German, Dutch, English the default font is used.
This is the file: GOLFontDefault.c

For the Chinese, Japanese, Korean, Russian and Thai the characters are derived from the font filter files:
1. HWChinese.txt
2. HWJapanese.txt
3. HWKorean.txt
4. HWRussian.txt

All these font filter text files are saved as Unicode with 16 bit encoding. This can be done by
specifying the encoding when saving the text file on a text file editor such as BabelPad.
http://www.babelstone.co.uk/Software/BabelPad.html

To properly view these files in an editor (e.g. BabelPad.exe) the respective fonts must be installed in the system.

Refer to the "Graphics Resource Converter Tool" (grc.java) help file or user's manual 
for instructions on how to create filtered font tables.

IMPORTANT:
The GRC project InternalResourceMultiApp.xml is included in this directory to show how the fonts
are generated. 

Before opening the project in the grc.java download the required (*.ttf) font
files first and place the font files in their respective directories in the 
<install dir>/Microchip/Graphics/Fonts directory. Each font will have its own sub-directory.

1. <install dir>/Microchip/Graphics/Fonts/firefly/fireflysung.ttf
2. <install dir>/Microchip/Graphics/Fonts/unbatang/UnBatang_0613.ttf
3. <install dir>/Microchip/Graphics/Fonts/gentium/Gentium.ttf
where: install_dir is the location where the Microchip Application Libraries (MLA) are installed.

It is also important that the file names of the *.ttf are the same. If they are not
the grc.java will generate an error when you load the project files.
The file names used are:
1. fireflysung.ttf
2. UnBatang_0613.ttf


