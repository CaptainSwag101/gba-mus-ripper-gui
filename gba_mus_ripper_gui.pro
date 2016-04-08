#-------------------------------------------------
#
# Project created by QtCreator 2016-04-04T16:42:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gba_mus_ripper_gui
TEMPLATE = app
QMAKE_LFLAGS += -static


SOURCES += main.cpp\
        mainwindow.cpp \
    gba_mus_ripper.cpp \
    sappy_detector/sappy_detector.cpp \
    song_ripper/midi.cpp \
    song_ripper/song_ripper.cpp \
    sound_font_ripper/gba_instr.cpp \
    sound_font_ripper/gba_samples.cpp \
    sound_font_ripper/sf2.cpp \
    sound_font_ripper/sound_font_ripper.cpp \
    song_ripper/note.cpp

HEADERS  += mainwindow.h \
    gba_mus_ripper.h \
    sappy_detector.h \
    hex_string.h \
    sappy_detector/sappy_detector.h \
    song_ripper/midi.h \
    sound_font_ripper/gba_instr.h \
    sound_font_ripper/gba_samples.h \
    sound_font_ripper/sf2.h \
    sound_font_ripper/sf2_chunks.h \
    sound_font_ripper/sf2_types.h \
    song_ripper/song_ripper.h \
    song_ripper/note.h

FORMS    += mainwindow.ui

DISTFILES += \
    goldensun_synth.raw \
    sound_font_ripper/goldensun_synth.raw

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../Qt/5.6/mingw49_32/lib/ -lQt5Core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../Qt/5.6/mingw49_32/lib/ -lQt5Cored
else:unix: LIBS += -L$$PWD/../../../../Qt/5.6/mingw49_32/lib/ -lQt5Core

INCLUDEPATH += $$PWD/../../../../Qt/5.6/mingw49_32/include
DEPENDPATH += $$PWD/../../../../Qt/5.6/mingw49_32/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Qt/5.6/mingw49_32/lib/libQt5Core.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Qt/5.6/mingw49_32/lib/libQt5Cored.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Qt/5.6/mingw49_32/lib/Qt5Core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Qt/5.6/mingw49_32/lib/Qt5Cored.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../Qt/5.6/mingw49_32/lib/libQt5Core.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../Qt/5.6/mingw49_32/lib/ -lQt5Widgets
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../Qt/5.6/mingw49_32/lib/ -lQt5Widgetsd
else:unix: LIBS += -L$$PWD/../../../../Qt/5.6/mingw49_32/lib/ -lQt5Widgets

INCLUDEPATH += $$PWD/../../../../Qt/5.6/mingw49_32/include
DEPENDPATH += $$PWD/../../../../Qt/5.6/mingw49_32/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Qt/5.6/mingw49_32/lib/libQt5Widgets.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Qt/5.6/mingw49_32/lib/libQt5Widgetsd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Qt/5.6/mingw49_32/lib/Qt5Widgets.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Qt/5.6/mingw49_32/lib/Qt5Widgetsd.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../Qt/5.6/mingw49_32/lib/libQt5Widgets.a
