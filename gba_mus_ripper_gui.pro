#-------------------------------------------------
#
# Project created by QtCreator 2016-04-04T16:42:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gba_mus_ripper_gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    progressdialog.cpp

HEADERS  += mainwindow.h \
    progressdialog.h

FORMS    += mainwindow.ui \
    progressdialog.ui

DISTFILES += \
    goldensun_synth.raw \
    psg_data.raw \
    GBA_Mus_Ripper.ico

win32:RC_ICONS += GBA_Mus_Ripper.ico
