# -------------------------------------------------
# Project created by QtCreator 2010-10-29T15:54:07
# -------------------------------------------------
QT -= gui
TARGET = HIDAPI
TEMPLATE = lib
CONFIG += staticlib
HEADERS += hidapi.h

# -------------------------------------------------
# Add appropriate source file depending on OS
# -------------------------------------------------
macx:  SOURCES += mac/hid.c
unix: !macx:  SOURCES += linux/hid-libusb.c
win32: SOURCES += windows/hid.c

# -------------------------------------------------
# Make sure output directory for object file and
# library is in the correct subdirectory
# -------------------------------------------------
macx {
    DESTDIR = mac
    OBJECTS_DIR = mac
    MOC_DIR = mac
    UI_DIR = mac
    RCC_DIR = mac
}
unix: !macx {
    DESTDIR = linux
    OBJECTS_DIR = linux
    MOC_DIR = linux
    UI_DIR = linux
    RCC_DIR = linux
}
win32 {
    DESTDIR = windows
    OBJECTS_DIR = windows
    MOC_DIR = windows
    UI_DIR = windows
    RCC_DIR = windows
}
