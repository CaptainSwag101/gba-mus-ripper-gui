#-------------------------------------------------
#
# Project created by QtCreator 2016-04-04T16:42:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gba_mus_ripper_gui
TEMPLATE = app
CONFIG += -static
QMAKE_LFLAGS += -static


SOURCES += main.cpp\
        mainwindow.cpp \
    gba_instr.cpp \
    gba_mus_ripper.cpp \
    gba_samples.cpp \
    midi.cpp \
    sf2.cpp \
    song_ripper.cpp \
    sound_font_ripper.cpp \
    sappy_detector.cpp

HEADERS  += mainwindow.h \
    gba_instr.hpp \
    gba_samples.hpp \
    hex_string.hpp \
    midi.hpp \
    sf2.hpp \
    sf2_chunks.hpp \
    sf2_types.hpp \
    gba_mus_ripper.h \
    sound_font_ripper.h \
    song_ripper.h \
    sappy_detector.h

FORMS    += mainwindow.ui

DISTFILES += \
    goldensun_synth.raw \
    psg_data.raw

unix|win32: LIBS += -lQt5Cored -lQt5Widgetsd
