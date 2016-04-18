# gba-mus-ripper-gui
A graphical frontend for my fork of Bregalad's 'gba-mus-ripper' program

To build this program from source using Qt Creator, you will need Qt version 5.6 with the MinGW32 compiler v4.9.2.

If you want to run this program as a standalone executable after building from source, you will need to follow the above step,
and also copy the 4 executables from the source directory into the program's irectory, as well as copy the files
"Qt5Core.dll", "Qt5Widgets.dll", and "Qt5Gui.dll" from "(Qt installation directory)\5.6\mingw49_32\bin\" into the program's directory.

Or, if you don't want to go through all that trouble, simply download the latest release from the "Releases" tab of this page.

NOTE: Currently this program only works properly under Windows, I do not know if Linux support will be possible to add due to the way
this program works.
