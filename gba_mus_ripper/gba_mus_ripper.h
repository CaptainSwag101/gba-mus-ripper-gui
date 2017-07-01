#ifndef GBA_MUS_RIPPER_H
#define GBA_MUS_RIPPER_H

#include <QApplication>
#include <QDir>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QtGlobal>

#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "../hex_string.h"
#include "../sappy_detector/sappy_detector.h"
#include "../song_ripper/song_ripper.h"
#include "../sound_font_ripper/sound_font_ripper.h"

using namespace std;


static const int32_t sample_rates[] = { -1, 5734, 7884, 10512, 13379, 15768, 18157, 21024, 26758, 31536, 36314, 40137, 42048 };


class musRipper
{
public:
    static int startRip();
    static int parseArgs(string rom, string out,
                             int _address, bool addr,
                             bool gm, bool xg, bool rc, bool sb, bool raw);

    static void printHelp();
    static uint32_t getGbaPointer();
    static string dec3(uint n);


    static bool gmFlag;
    static bool xgFlag;
    static bool rcFlag;
    static bool sbFlag;
    static bool rawFlag;
    static bool addrFlag;
    static int address;
    static uint32_t song_tbl_ptr;

    static string romPath;
    static size_t romSize;
    static string romName;
    static string outPath;
};

#endif // GBA_MUS_RIPPER_H
