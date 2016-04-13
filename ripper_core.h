#include <QString>
#ifndef RIPPER_CORE_H
#define RIPPER_CORE_H

static int startRip(QString romPath, QString outPath, bool gm, bool rc, bool xg, bool sb, bool raw, int adr);

static QString RomPath;
static QString OutPath;
static int Address;
static bool gmFlag, rcFlag, xgFlag, sbFlag, rawFlag;
static uint32_t song_tbl_ptr;

#endif // RIPPER_CORE_H
