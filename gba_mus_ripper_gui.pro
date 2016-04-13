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
    ripper_core.cpp \
    sappy_detector.cpp

HEADERS  += mainwindow.h \
    ripper_core.h \
    sappy_detector.h

FORMS    += mainwindow.ui

DISTFILES += \
    goldensun_synth.raw \
    psg_data.raw
