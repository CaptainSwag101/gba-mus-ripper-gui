#-------------------------------------------------
#
# Project created by QtCreator 2016-04-04T16:42:51
#
#-------------------------------------------------

QT       += core gui
VERSION = 0.1.2

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += -std=c++11
TARGET = gba_mus_ripper_gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    progressdialog.cpp \
    aboutdialog.cpp \
    gba_mus_ripper/gba_mus_ripper.cpp \
    song_ripper/midi.cpp \
    song_ripper/song_ripper.cpp \
    sound_font_ripper/gba_instr.cpp \
    sound_font_ripper/gba_samples.cpp \
    sound_font_ripper/sf2.cpp \
    sound_font_ripper/sound_font_ripper.cpp \
    sappy_detector/sappy_detector.cpp

HEADERS  += mainwindow.h \
    progressdialog.h \
    aboutdialog.h \
    sappy_detector/sappy_detector.h \
    song_ripper/midi.h \
    sound_font_ripper/gba_instr.h \
    sound_font_ripper/gba_samples.h \
    sound_font_ripper/sf2.h \
    sound_font_ripper/sf2_chunks.h \
    sound_font_ripper/sf2_types.h \
    hex_string.h \
    gba_mus_ripper/gba_mus_ripper.h \
    song_ripper/song_ripper.h \
    sound_font_ripper/sound_font_ripper.h

FORMS    += mainwindow.ui \
    progressdialog.ui \
    aboutdialog.ui

DISTFILES += \
    goldensun_synth.raw \
    psg_data.raw \
    GBA_Mus_Ripper.ico \
    sound_font_ripper/goldensun_synth.raw \
    sound_font_ripper/psg_data.raw

win32:RC_ICONS += GBA_Mus_Ripper.ico
