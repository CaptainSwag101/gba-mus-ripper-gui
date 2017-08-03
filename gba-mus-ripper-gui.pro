#-------------------------------------------------
#
# Project created by QtCreator 2016-09-12T12:45:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gba_mus_ripper_gui
TEMPLATE = app

FORMS += \
    source/aboutdialog.ui \
    source/mainwindow.ui \
    source/progressdialog.ui

HEADERS += \
    source/aboutdialog.h \
    source/gba_instr.h \
    source/gba_mus_ripper.h \
    source/gba_samples.h \
    source/hex_string.h \
    source/mainwindow.h \
    source/midi.h \
    source/moc_predefs.h \
    source/progressdialog.h \
    source/sappy_detector.h \
    source/sf2.h \
    source/sf2_chunks.h \
    source/sf2_types.h \
    source/song_ripper.h \
    source/sound_font_ripper.h

SOURCES += \
    source/aboutdialog.cpp \
    source/gba_instr.cpp \
    source/gba_mus_ripper.cpp \
    source/gba_samples.cpp \
    source/main.cpp \
    source/mainwindow.cpp \
    source/midi.cpp \
    source/progressdialog.cpp \
    source/sappy_detector.cpp \
    source/sf2.cpp \
    source/song_ripper.cpp \
    source/sound_font_ripper.cpp

DISTFILES += \
    resources/gba-mus-ripper-gui logo.pdn \
    resources/gba-mus-ripper-gui logo.png \
    resources/GBA_Mus_Ripper_GUI.ico \
    resources/goldensun_synth.raw \
    resources/psg_data.raw \
    resources/gba-mus-ripper-gui_resource.rc

win32:RC_ICONS += resources/GBA_Mus_Ripper_GUI.ico
