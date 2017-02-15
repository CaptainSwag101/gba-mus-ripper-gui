#include "mainwindow.h"
#include <QApplication>
#include "gba_mus_ripper/gba_mus_ripper.h"
#include "song_ripper/song_ripper.h"
#include "sound_font_ripper/sound_font_ripper.h"

int32_t main(int32_t argc, char *argv[])
{

    if (argc > 1)
    {
        if (argv[1] == (std::string)"song_ripper")
        {
            return song_ripper(argc - 1, argv + 1);
        }
        else if (argv[1] == (std::string)"sound_font_ripper")
        {
            return sound_font_ripper(argc - 1, argv + 1);
        }
        else
        {

            return -1;
        }
    }
    else
    {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        return a.exec();
    }
}
