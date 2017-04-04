#include "mainwindow.h"
#include <QApplication>
#include "gba_mus_ripper/gba_mus_ripper.h"
#include "song_ripper/song_ripper.h"
#include "sound_font_ripper/sound_font_ripper.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
