#-------------------------------------------------
#
# Project created by QtCreator 2016-04-04T16:42:51
#
#-------------------------------------------------

QT       += core gui
VERSION = 0.1.3

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gba_mus_ripper_gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    aboutdialog.ui

DISTFILES += \
    goldensun_synth.raw \
    psg_data.raw \
    Resources/GBA_Mus_Ripper.ico

win32:RC_ICONS += Resources/GBA_Mus_Ripper.ico
