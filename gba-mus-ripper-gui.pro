#-------------------------------------------------
#
# Project created by QtCreator 2016-09-12T12:45:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gba_mus_ripper_gui
TEMPLATE = app

HEADERS  += mainwindow.h \
    gba_mus_ripper/gba_mus_ripper.h \
    sappy_detector/sappy_detector.h \
    song_ripper/midi.h \
    song_ripper/song_ripper.h \
    sound_font_ripper/gba_instr.h \
    sound_font_ripper/gba_samples.h \
    sound_font_ripper/sf2.h \
    sound_font_ripper/sf2_chunks.h \
    sound_font_ripper/sf2_types.h \
    sound_font_ripper/sound_font_ripper.h \
    aboutdialog.h \
    hex_string.h \
    progressdialog.h

SOURCES += main.cpp\
        mainwindow.cpp \
    gba_mus_ripper/gba_mus_ripper.cpp \
    sappy_detector/sappy_detector.cpp \
    song_ripper/midi.cpp \
    song_ripper/song_ripper.cpp \
    sound_font_ripper/gba_instr.cpp \
    sound_font_ripper/gba_samples.cpp \
    sound_font_ripper/sf2.cpp \
    sound_font_ripper/sound_font_ripper.cpp \
    aboutdialog.cpp \
    progressdialog.cpp

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    progressdialog.ui

DISTFILES += GBA_Mus_Ripper_GUI.ico \
    goldensun_synth.raw \
    psg_data.raw

win32:RC_ICONS += GBA_Mus_Ripper_GUI.ico
