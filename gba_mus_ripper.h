#include <string>

#ifndef GBA_MUS_RIPPER_H
#define GBA_MUS_RIPPER_H

using namespace std;

string inGBA_path;
std::string out_path;
size_t inGBA_size;
std::string name;
std::string path;
bool gm = false;
bool xg = false;
bool rc = false;
bool sb = false;
bool raw = false;
uint32_t song_tbl_ptr = 0;

static const int sample_rates[] = {-1, 5734, 7884, 10512, 13379, 15768, 18157, 21024, 26758, 31536, 36314, 40137, 42048};

int musRip(int argc, std::string args[]);
#endif // GBA_MUS_RIPPER_H
