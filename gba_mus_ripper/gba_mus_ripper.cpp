/**
 * GBAMusRipper (c) 2012 by Bregalad
 * This is free and open source software
 *
 * This program analyzes a Game Boy Advance ROM and searches for a sound engine
 * named "Sappy" which is used in ~90% of commercial GBA games.
 *
 * If the engine is found it rips all songs to MIDI (.mid) format and all
 * instruments to SoundFont 2.0 (.sf2) format.
 */

#include <QApplication>
#include <QDir>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <cmath>
#include <string>
#include <vector>
#include <set>
#include <cstdint>
#include "../hex_string.h"
#include "../sappy_detector/sappy_detector.h"
#include "../song_ripper/song_ripper.h"
#include "../sound_font_ripper/sound_font_ripper.h"

using namespace std;

static FILE *inGBA;
static string inGBA_path;
static size_t inGBA_size;
static string name;
static string outPath;
static bool gm = false;
static bool xg = false;
static bool rc = false;
static bool sb = false;
static bool raw = false;
static uint32_t song_tbl_ptr = 0;

static const int32_t sample_rates[] = {-1, 5734, 7884, 10512, 13379, 15768, 18157, 21024, 26758, 31536, 36314, 40137, 42048};

static void print_instructions()
{
    cout <<
                "  /========================================================\\\n"
                "-<   GBA Mus Ripper 3.5 (c) 2017 Bregalad, CaptainSwag101   >-\n"
                "  \\========================================================/\n\n"
                "Usage : gba_mus_ripper game.gba [-o output path] [flags] [address]\n\n"
                "-o   : Output path. All MIDIs and soundfonts will be ripped to a subfolder inside this directory.\n"
                "-gm  : Give General MIDI names to presets. Note that this will only change the names and will NOT\n"
                "       magically turn the soundfont into a General MIDI compliant soundfont.\n"
                "-rc  : Rearrange channels in output MIDIs so channel 10 is avoided. Needed by sound\n"
                "       cards where it's impossible to disable \"drums\" on channel 10 even with GS or XG commands.\n"
                "-xg  : Output MIDI will be compliant to XG standard (instead of default GS standard)\n"
                "-sb  : Separate banks. Every sound bank is ripped to a different .sf2 file and placed\n"
                "       into different sub-folders (instead of doing it in a single .sf2 file and a single folder)\n"
                "-raw : Output MIDIs exactly as they're encoded in ROM, without linearizing volumes/velocities\n"
                "       and without simulating vibratos.\n"
                "-adr : Force adress of the song table manually. This is required for manually dumping music data\n"
                "       from ROMs where the location can't be detected automatically.\n"
                ;
    exit(-1);
}

static uint32_t get_GBA_pointer()
{
    uint32_t p;
    fread(&p, 4, 1, inGBA);
    return p - 0x8000000;
}

static void mkdir(string name)
{
    system(("mkdir -p \"" + name + '"').c_str());
}

//  Convert number to string with always 3 digits (even if leading zeros)
static string dec3(uint32_t n)
{
    string s;
    s += "0123456789"[n/100];
    s += "0123456789"[n/10%10];
    s += "0123456789"[n%10];
    return s;
}

static int32_t parse_args(int32_t argc, string argv[])
{
    const char *args[argc];
    for (int32_t c = 0; c < argc; c++)
    {
        args[c] = argv[c].c_str();
    }

    if (argc < 1) print_instructions();

    bool path_found = false, song_tbl_found = false;
    for(int32_t i = 0; i < argc; i++)
    {
        if (args[i][0] == '-')
        {
            if (!strcmp(args[i], "-help"))
                print_instructions();
            else if (!strcmp(args[i], "-o") && path_found && (i + 1) <= argc)
            {
                outPath = args[i + 1];
                i++;
            }
            else if (!strcmp(args[i], "-gm"))
                gm = true;
            else if (!strcmp(args[i], "-xg"))
                xg = true;
            else if (!strcmp(args[i], "-rc"))
                rc = true;
            else if (!strcmp(args[i], "-sb"))
                sb = true;
            else if (!strcmp(args[i], "-raw"))
                raw = true;
            else
            {
                cout << stderr << "Error: Unknown command line option: " << args[i] << ". Try with -help to get information.\n";
                return -1;
            }
        }
        // Convert given address to binary, use it instead of automatically detected one
        else if (!path_found)
        {
            // Get GBA file
            inGBA = fopen(args[i], "rb");
            if (!inGBA)
            {
                cout << stderr << "Error: Can't open file " << args[i] << " for reading.\n";
                return -2;
            }

            // Name is filename without the extention and without path
            inGBA_path = args[i];
            size_t separator_index = inGBA_path.find_last_of("/\\") + 1;
            name = inGBA_path.substr(separator_index, inGBA_path.find_last_of('.') - separator_index);

            // Path where the input GBA file is located
            outPath = inGBA_path.substr(0, separator_index);
            path_found = true;
        }
        else if (!song_tbl_found)
        {
            errno = 0;
            song_tbl_ptr = strtoul(args[i], 0, 0);
            if (errno)
            {
                cout << stderr << "Error: " << args[i] << " is not a valid song table address.\n";
                return -3;
            }
            song_tbl_found = true;
        }
        else
        {
            cout << stderr << "Error: Don't know what to do with " << args[i] << ". Try with -help to get more information.\n";
            return -4;
        }
    }
    if (!path_found)
    {
        cout << stderr << "Error: No input GBA file. Try with -help to get more information.\n";
        return -1;
    }

    return 0;
}

int32_t mus_ripper(int32_t argc, string argv[])
{
    // Parse arguments (without program name)
    int32_t parse_result = parse_args(argc, argv);
    if (parse_result < 0)
    {
        return parse_result;
    }

    // Compute program prefix (should be "", "./", "../" or whathever)
    string prg_name = argv[0];

    int32_t sample_rate = 0, main_volume = 0;		// Use default values when those are '0'

    // If the user hasn't provided an address manually, we'll try to automatically detect it
    if (!song_tbl_ptr)
    {
        QCoreApplication::processEvents();
        // Auto-detect address of sappy engine
        string sappy_detector_cmd = inGBA_path;
#ifdef QT_DEBUG
        cout << "DEBUG: Going to call system(" << sappy_detector_cmd << ")\n";
#endif
        int32_t sound_engine_adr = sappy_detector(1, sappy_detector_cmd);

        // Exit if no sappy engine was found
        if (!sound_engine_adr)
        {
            return -4;
        }

        if (fseek(inGBA, sound_engine_adr, SEEK_SET))
        {
            cout << stderr << "Error: Invalid offset within input GBA file: 0x" <<  sound_engine_adr << ".\n";
            return -5;
        }

        // Engine parameter's word
        uint32_t parameter_word;
        fread(&parameter_word, 4, 1, inGBA);

        // Get sampling rate
        sample_rate = sample_rates[(parameter_word>>16) & 0xf];
        main_volume = (parameter_word>>12) & 0xf;

        // Compute address of song table
        uint32_t song_levels;			// Read # of song levels
        fread(&song_levels, 4, 1, inGBA);
        cout << "# of song levels: " << song_levels  << "\n";
        song_tbl_ptr = get_GBA_pointer() + 12 * song_levels;
    }

    // Create a directory named like the input ROM, without the .gba extention
    mkdir(outPath + "/" + name);

    //  Get the size of the input GBA file
    fseek(inGBA, 0L, SEEK_END);
    inGBA_size = ftell(inGBA);

    if (song_tbl_ptr >= inGBA_size)
    {
        cout << stderr << "Fatal error: Song table at 0x" <<  song_tbl_ptr << " is past the end of the file.\n";
        return -6;
    }

    cout << "Parsing song table...";
    // New list of songs
    vector<uint32_t> song_list;
    // New list of sound banks
    set<uint32_t> sound_bank_list;

    if (fseek(inGBA, song_tbl_ptr, SEEK_SET))
    {
        cout << stderr << "Fatal error: Can't seek to song table at: 0x" <<  song_tbl_ptr << ".\n";
        return -7;
    }

    // Ignores entries which are made of 0s at the start of the song table
    // this fix was necessarily for the game Fire Emblem
    uint32_t song_pointer;
    while(true)
    {
        fread(&song_pointer, 4, 1, inGBA);
        if (song_pointer != 0) break;
        song_tbl_ptr += 4;
    }

    uint32_t i = 0;
    while(true)
    {
        song_pointer -= 0x8000000;		// Adjust pointer

        // Stop as soon as we met with an invalid pointer
        if (song_pointer == 0 || song_pointer >= inGBA_size) break;

        for(int32_t j = 4; j != 0; --j) fgetc(inGBA);		// Discard 4 bytes (sound group)
        song_list.push_back(song_pointer);			// Add pointer to list
        i++;
        fread(&song_pointer, 4, 1, inGBA);
    };
    // As soon as data that is not a valid pointer is found, the song table is terminated

    // End of song table
    uint32_t song_tbl_end_ptr = 8 * i + song_tbl_ptr;

    cout << "Collecting sound bank list...";

    typedef set<uint32_t>::iterator bank_t;
    bank_t *sound_bank_index_list = new bank_t[song_list.size()];

    for(i = 0; i < song_list.size(); i++)
    {
        // Ignore unused song, which points to the end of the song table (for some reason)
        if (song_list[i] != song_tbl_end_ptr)
        {
            // Seek to song data
            if (fseek(inGBA, song_list[i] + 4, SEEK_SET)) continue;
            uint32_t sound_bank_ptr = get_GBA_pointer();

            // Add sound bank to list if not already in the list
            sound_bank_index_list[i] = sound_bank_list.insert(sound_bank_ptr).first;
        }
    }

    // Close GBA file so that Songripper can access it
    fclose(inGBA);

    // Create directories for each sound bank if separate banks is enabled
    if (sb)
    {
        for(bank_t j = sound_bank_list.begin(); j != sound_bank_list.end(); ++j)
        {
            uint32_t d = distance(sound_bank_list.begin(), j);
            string subdir = outPath + '/' + name + '/' + "soundbank_" + dec3(d);
            mkdir(subdir);
        }
    }

    for(i = 0; i < song_list.size(); i++)
    {
        if (song_list[i] != song_tbl_end_ptr)
        {
            QCoreApplication::processEvents();
            uint32_t bank_index = distance(sound_bank_list.begin(), sound_bank_index_list[i]);
            string seq_rip_cmd = "song_ripper\n" + inGBA_path + "\n" + outPath + "/" + name;

            // Add leading zeroes to file name
            if (sb)
                seq_rip_cmd += "/soundbank_" + dec3(bank_index);

            seq_rip_cmd += "/song" + dec3(i) + ".mid";

            seq_rip_cmd += "\n0x" + hex(song_list[i]);
            seq_rip_cmd += rc ? "\n-rc" : (xg ? "\n-xg" : "\n-gs");
            if (!raw)
            {
                seq_rip_cmd += "\n-sv";
                seq_rip_cmd += "\n-lv";
            }
            // Bank number, if banks are not separated
            if (!sb)
                seq_rip_cmd += "\n-b" + to_string(bank_index);

            QStringList argList = QString::fromStdString(seq_rip_cmd).split("\n");

            cout << "Song " << i << "\n";
#ifdef QT_DEBUG
            cout << "DEBUG: Going to call system(" << seq_rip_cmd << ")\n";
#endif
            //if (!system(seq_rip_cmd.c_str()))
            QProcess *songripper = new QProcess();

#ifdef Q_OS_WIN32
            if (!QFile::exists(QDir::toNativeSeparators(QDir::currentPath() + "/gba_mus_ripper_gui.exe")))
            {
                cout << "Unable to find the " << QDir::toNativeSeparators(QDir::currentPath() + "/gba_mus_ripper_gui.exe").toStdString() << " executable!\n";
                return -8;
            }
            songripper->setProgram(QDir::currentPath() + "/gba_mus_ripper_gui.exe");
#else
            if (!QFile::exists(QDir::toNativeSeparators(QDir::currentPath() + "/gba_mus_ripper_gui")))
            {
                cout << "Unable to find the " << QDir::toNativeSeparators(QDir::currentPath() + "/gba_mus_ripper_gui").toStdString() << " executable!\n";
                return -8;
            }
            songripper->setProgram(QDir::currentPath() + "/gba_mus_ripper_gui");
#endif

            songripper->setArguments(argList);
            songripper->start();
            songripper->waitForFinished();
            if (songripper->exitCode() < 0)
                cout << "An error has occurred.";
        }
    }
    delete[] sound_bank_index_list;

    if (sb)
    {
        // Rips each sound bank in a different file/folder
        for(bank_t j = sound_bank_list.begin(); j != sound_bank_list.end(); ++j)
        {
            QCoreApplication::processEvents();
            uint32_t bank_index = distance(sound_bank_list.begin(), j);

            string sbnumber = dec3(bank_index);
            string foldername = "soundbank_" + sbnumber;
            string sf_rip_args = "sound_font_ripper\n" + inGBA_path + "\n" + outPath + "/" + name + '/';
            sf_rip_args += foldername + '/' + foldername /* + "_@" + hex(*j) */ + ".sf2\"";

            if (sample_rate) sf_rip_args += "\n-s" + to_string(sample_rate);
            if (main_volume)	sf_rip_args += "\n-mv" + to_string(main_volume);
            if (gm) sf_rip_args += "\n-gm";
            sf_rip_args += "\n0x" + hex(*j);

            QStringList argList = QString::fromStdString(sf_rip_args).split("\n");

#ifdef QT_DEBUG
            cout << "DEBUG: Going to call system(" << sf_rip_args << ")\n";
#endif
            //system(sf_rip_args.c_str());
            QProcess *sf2ripper = new QProcess();

#ifdef Q_OS_WIN32
            if (!QFile::exists(QDir::toNativeSeparators(QDir::currentPath() + "/gba_mus_ripper_gui.exe")))
            {
                cout << "Unable to find the " << QDir::toNativeSeparators(QDir::currentPath() + "/gba_mus_ripper_gui.exe").toStdString() << " executable!\n";
                return -8;
            }
            sf2ripper->setProgram(QDir::currentPath() + "/gba_mus_ripper_gui.exe");
#else
            if (!QFile::exists(QDir::toNativeSeparators(QDir::currentPath() + "/gba_mus_ripper_gui")))
            {
                cout << "Unable to find the " << QDir::toNativeSeparators(QDir::currentPath() + "/gba_mus_ripper_gui").toStdString() << " executable!\n";
                return -8;
            }
            sf2ripper->setProgram(QDir::currentPath() + "/gba_mus_ripper_gui");
#endif

            sf2ripper->setArguments(argList);
            sf2ripper->start();
            sf2ripper->waitForFinished();
            cout << sf2ripper->readAllStandardOutput().toStdString();
            if (sf2ripper->exitCode() != 0)
                cout << "An error has occured.";
        }
    }
    else
    {
        // Rips each sound bank in a single soundfont file
        // Build argument list to call sound_font_ripper
        // Output sound font named after the input ROM
        QCoreApplication::processEvents();
        string sf_rip_args = "sound_font_ripper\n" + inGBA_path + "\n" + outPath + "/" + name + '/' + name + ".sf2";
        if (sample_rate) sf_rip_args += "\n-s" + to_string(sample_rate);
        if (main_volume) sf_rip_args += "\n-mv" + to_string(main_volume);
        // Pass -gm argument if necessary
        if (gm) sf_rip_args += "\n-gm";

        // Make sound banks addresses list.
        for(bank_t j = sound_bank_list.begin(); j != sound_bank_list.end(); ++j)
            sf_rip_args += "\n0x" + hex(*j);

        QStringList argList = QString::fromStdString(sf_rip_args).split("\n");

        // Call sound font ripper
#ifdef QT_DEBUG
        cout << "DEBUG: Going to call system(" << sf_rip_args << ")\n";
#endif
        //system(sf_rip_args.c_str());
        QProcess *sf2ripper = new QProcess();

#ifdef Q_OS_WIN32
            if (!QFile::exists(QDir::toNativeSeparators(QDir::currentPath() + "/gba_mus_ripper_gui.exe")))
            {
                cout << "Unable to find the " << QDir::toNativeSeparators(QDir::currentPath() + "/gba_mus_ripper_gui.exe").toStdString() << " executable!\n";
                return -8;
            }
            sf2ripper->setProgram(QDir::currentPath() + "/gba_mus_ripper_gui.exe");
#else
            if (!QFile::exists(QDir::toNativeSeparators(QDir::currentPath() + "/gba_mus_ripper_gui")))
            {
                cout << "Unable to find the " << QDir::toNativeSeparators(QDir::currentPath() + "/gba_mus_ripper_gui").toStdString() << " executable!\n";
                return -8;
            }
            sf2ripper->setProgram(QDir::currentPath() + "/gba_mus_ripper_gui");
#endif

        sf2ripper->setProgram(QDir::currentPath() + "/gba_mus_ripper_gui");
        sf2ripper->setArguments(argList);
        sf2ripper->start();
        sf2ripper->waitForFinished();
        cout << sf2ripper->readAllStandardOutput().toStdString();
        if (sf2ripper->exitCode() != 0)
            cout << "An error has occured.";
    }
    cout << "Rip completed!\n";
    return 0;
}
