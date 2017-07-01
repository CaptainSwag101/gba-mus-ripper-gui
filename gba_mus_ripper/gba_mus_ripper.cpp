/*
 * GBAMusRipper (c) 2012 by Bregalad
 * This is free and open source software
 *
 * This program analyzes a Game Boy Advance ROM and searches for a sound engine
 * named "Sappy" which is used in ~90% of commercial GBA games.
 *
 * If the engine is found it rips all songs to MIDI (.mid) format and all
 * instruments to SoundFont 2.0 (.sf2) format.
 */

#include "gba_mus_ripper.h"


void musRipper::printHelp()
{
    cout << "  /========================================================\\" << endl;
    cout << "-<   GBA Mus Ripper 3.5 (c) 2017 Bregalad, CaptainSwag101   >-" << endl;
    cout << "  \\========================================================/" << endl;
    cout << endl;
    cout << "Usage : gba_mus_ripper game.gba [-o output path] [flags] [address]" << endl;
    cout << endl;
    cout << "-o   : Output path. All MIDIs and soundfonts will be ripped to a subfolder inside this directory." << endl;
    cout << "-gm  : Give General MIDI names to presets. Note that this will only change the names and will NOT" << endl;
    cout << "       magically turn the soundfont into a General MIDI compliant soundfont." << endl;
    cout << "-rc  : Rearrange channels in output MIDIs so channel 10 is avoided. Needed by sound" << endl;
    cout << "       cards where it's impossible to disable \"drums\" on channel 10 even with GS or XG commands." << endl;
    cout << "-xg  : Output MIDI will be compliant to XG standard (instead of default GS standard)" << endl;
    cout << "-sb  : Separate banks. Every sound bank is ripped to a different .sf2 file and placed" << endl;
    cout << "       into different sub-folders (instead of doing it in a single .sf2 file and a single folder)" << endl;
    cout << "-raw : Output MIDIs exactly as they're encoded in ROM, without linearizing volumes/velocities" << endl;
    cout << "       and without simulating vibratos." << endl;
    cout << "-adr : Force adress of the song table manually. This is required for manually dumping music data" << endl;
    cout << "       from ROMs where the location can't be detected automatically." << endl;
}


uint32_t musRipper::getGbaPointer()
{
    uint32_t p;
    ifstream ifs;
    ifs.open(romPath);
    ifs >> p;

    ifs.close();

    return p - 0x8000000;
}


//  Convert number to string with always 3 digits (even if leading zeros)
string musRipper::dec3(uint n)
{
    string s;
    s += "0123456789"[n/100];
    s += "0123456789"[n/10%10];
    s += "0123456789"[n%10];
    return s;
}


int musRipper::parseArgs(string rom, string out,
                         int _address, bool addr,
                         bool gm, bool xg, bool rc, bool sb, bool raw)
{
    bool path_found = false, song_tbl_found = false;

    romPath = rom;
    outPath = out;
    address = _address;
    addrFlag = addr;
    gmFlag = gm;
    xgFlag = xg;
    rcFlag = rc;
    sbFlag = sb;
    rawFlag = raw;

    size_t separator_index = romPath.find_last_of("/") + 1;
    romName = romPath.substr(separator_index, romPath.find_last_of('.') - separator_index);

    if (!song_tbl_found)
    {
        if (address < 0)
        {
            cout << stderr << "Error: " << address << " is not a valid song table address.\n";
            return -3;
        }

        song_tbl_ptr = address;

        song_tbl_found = true;
    }

    if (romPath == "")
    {
        cout << "Error: No input GBA file. Try with -help to get more information." << endl;
        return -1;
    }

    return 0;
}


int musRipper::startRip()
{
    int sample_rate = 0, main_volume = 0;		// Use default values when those are '0'

    ifstream ifs;
    ifs.open(romPath);

    // If the user hasn't provided an address manually, we'll try to automatically detect it
    if (!song_tbl_ptr)
    {
        QCoreApplication::processEvents();
        // Auto-detect address of sappy engine
        string sappy_detector_cmd = romPath;
#ifdef QT_DEBUG
        cout << "DEBUG: Going to call system(" << sappy_detector_cmd << ")\n";
#endif
        int32_t sound_engine_adr = sappy_detector(1, sappy_detector_cmd);

        // Exit if no sappy engine was found
        if (!sound_engine_adr)
        {
            return -4;
        }

        ifs.seekg(sound_engine_adr);
        /*
        if (fseek(inGBA, sound_engine_adr, SEEK_SET))
        {
            cout << stderr << "Error: Invalid offset within input GBA file: 0x" <<  sound_engine_adr << ".\n";
            return -5;
        }
        */

        // Engine parameter's word
        uint32_t parameter_word;
        ifs >> parameter_word;
        //fread(&parameter_word, 4, 1, inGBA);

        // Get sampling rate
        sample_rate = sample_rates[(parameter_word>>16) & 0xf];
        main_volume = (parameter_word>>12) & 0xf;

        // Compute address of song table
        uint32_t song_levels;			// Read # of song levels
        //fread(&song_levels, 4, 1, inGBA);
        ifs >> song_levels;
        cout << "# of song levels: " << song_levels  << "\n";
        song_tbl_ptr = getGbaPointer() + 12 * song_levels;
    }

    // Create a directory named like the input ROM, without the .gba extention
    QDir dir;
    dir.mkdir(QString::fromStdString(outPath + "/" + romName));

    //  Get the size of the input GBA file
    //fseek(inGBA, 0L, SEEK_END);
    romSize = ifs.tellg();
    //romSize = ftell(inGBA);

    if (song_tbl_ptr >= romSize)
    {
        cout << "Fatal error: Song table at 0x" <<  song_tbl_ptr << " is past the end of the file.\n";
        return -6;
    }

    cout << "Parsing song table...";
    // New list of songs
    vector<uint32_t> song_list;
    // New list of sound banks
    set<uint32_t> sound_bank_list;

    ifs.seekg(song_tbl_ptr);
    /*
    if (fseek(inGBA, song_tbl_ptr, SEEK_SET))
    {
        cout << "Fatal error: Can't seek to song table at: 0x" <<  song_tbl_ptr << ".\n";
        return -7;
    }
    */

    // Ignores entries which are made of 0s at the start of the song table
    // this fix was necessarily for the game Fire Emblem
    uint32_t song_pointer;
    while(true)
    {
        //fread(&song_pointer, 4, 1, inGBA);
        ifs >> song_pointer;
        if (song_pointer != 0) break;
        song_tbl_ptr += 4;
    }

    uint32_t i = 0;
    while(true)
    {
        song_pointer -= 0x8000000;		// Adjust pointer

        // Stop as soon as we met with an invalid pointer
        if (song_pointer == 0 || song_pointer >= romSize) break;

        for (int32_t j = 4; j != 0; --j) ifs.seekg(4);		// Discard 4 bytes (sound group)
        song_list.push_back(song_pointer);			// Add pointer to list
        i++;
        //fread(&song_pointer, 4, 1, inGBA);
        ifs >> song_pointer;
    };
    // As soon as data that is not a valid pointer is found, the song table is terminated

    // End of song table
    uint32_t song_tbl_end_ptr = 8 * i + song_tbl_ptr;

    cout << "Collecting sound bank list...";

    typedef set<uint32_t>::iterator bank_t;
    bank_t *sound_bank_index_list = new bank_t[song_list.size()];

    for (i = 0; i < song_list.size(); i++)
    {
        // Ignore unused song, which points to the end of the song table (for some reason)
        if (song_list[i] != song_tbl_end_ptr)
        {
            // Seek to song data
            ifs.seekg(song_list[i] + 4);
            //if (fseek(inGBA, song_list[i] + 4, SEEK_SET)) continue;
            //uint32_t sound_bank_ptr = get_GBA_pointer();

            // Add sound bank to list if not already in the list
            //sound_bank_index_list[i] = sound_bank_list.insert(sound_bank_ptr).first;
        }
    }

    // Close GBA file so that Songripper can access it
    //fclose(inGBA);
    ifs.close();

    // Create directories for each sound bank if separate banks is enabled
    if (sbFlag)
    {
        for (bank_t j = sound_bank_list.begin(); j != sound_bank_list.end(); ++j)
        {
            uint32_t d = distance(sound_bank_list.begin(), j);
            string subdir = outPath + '/' + romName + '/' + "soundbank_" + dec3(d);
            QDir dir;
            dir.mkdir(QString::fromStdString(subdir));
        }
    }

    for (i = 0; i < song_list.size(); i++)
    {
        if (song_list[i] != song_tbl_end_ptr)
        {
            QCoreApplication::processEvents();
            uint32_t bank_index = distance(sound_bank_list.begin(), sound_bank_index_list[i]);
            string seq_rip_cmd = "song_ripper\n" + romPath + "\n" + outPath + "/" + romName;

            // Add leading zeroes to file name
            if (sbFlag)
                seq_rip_cmd += "/soundbank_" + dec3(bank_index);

            seq_rip_cmd += "/song" + dec3(i) + ".mid";

            seq_rip_cmd += "\n0x" + hex(song_list[i]);
            seq_rip_cmd += rcFlag ? "\n-rc" : (xgFlag ? "\n-xg" : "\n-gs");
            if (!rawFlag)
            {
                seq_rip_cmd += "\n-sv";
                seq_rip_cmd += "\n-lv";
            }
            // Bank number, if banks are not separated
            if (!sbFlag)
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
                cout << "Unable to find the " << QDir::toNativeSeparators(QDir::currentPath() + "/gba_mus_ripper_gui.exe").toStdString() << " executable!" << endl;
                return -8;
            }
            songripper->setProgram(QDir::currentPath() + "/gba_mus_ripper_gui.exe");
#else
            if (!QFile::exists(QDir::toNativeSeparators(QDir::currentPath() + "/gba_mus_ripper_gui")))
            {
                cout << "Unable to find the " << QDir::toNativeSeparators(QDir::currentPath() + "/gba_mus_ripper_gui").toStdString() << " executable!" << endl;
                return -8;
            }
            songripper->setProgram(QDir::currentPath() + "/gba_mus_ripper_gui");
#endif

            songripper->setArguments(argList);
            songripper->start();
            songripper->waitForFinished();
            if (songripper->exitCode() < 0)
                cout << "An error has occurred." << endl;
        }
    }
    delete[] sound_bank_index_list;

    if (sbFlag)
    {
        // Rips each sound bank in a different file/folder
        for (bank_t j = sound_bank_list.begin(); j != sound_bank_list.end(); ++j)
        {
            QCoreApplication::processEvents();
            uint32_t bank_index = distance(sound_bank_list.begin(), j);

            string sbnumber = dec3(bank_index);
            string foldername = "soundbank_" + sbnumber;
            string sf_rip_args = "sound_font_ripper\n" + romPath + "\n" + outPath + "/" + romName + '/';
            sf_rip_args += foldername + '/' + foldername /* + "_@" + hex(*j) */ + ".sf2\"";

            if (sample_rate) sf_rip_args += "\n-s" + to_string(sample_rate);
            if (main_volume)	sf_rip_args += "\n-mv" + to_string(main_volume);
            if (gmFlag) sf_rip_args += "\n-gm";
            sf_rip_args += "\n0x" + hex(*j);

            QStringList argList = QString::fromStdString(sf_rip_args).split("\n");

#ifdef QT_DEBUG
            cout << "DEBUG: Going to call system(" << sf_rip_args << ")" << endl;
#endif
            //system(sf_rip_args.c_str());
            QProcess *sf2ripper = new QProcess();

#ifdef Q_OS_WIN32
            if (!QFile::exists(QDir::toNativeSeparators(QDir::currentPath() + "/gba_mus_ripper_gui.exe")))
            {
                cout << "Unable to find the " << QDir::toNativeSeparators(QDir::currentPath() + "/gba_mus_ripper_gui.exe").toStdString() << " executable!" << endl;
                return -8;
            }
            sf2ripper->setProgram(QDir::currentPath() + "/gba_mus_ripper_gui.exe");
#else
            if (!QFile::exists(QDir::toNativeSeparators(QDir::currentPath() + "/gba_mus_ripper_gui")))
            {
                cout << "Unable to find the " << QDir::toNativeSeparators(QDir::currentPath() + "/gba_mus_ripper_gui").toStdString() << " executable!" << endl;
                return -8;
            }
            sf2ripper->setProgram(QDir::currentPath() + "/gba_mus_ripper_gui");
#endif

            sf2ripper->setArguments(argList);
            sf2ripper->start();
            sf2ripper->waitForFinished();
            cout << sf2ripper->readAllStandardOutput().toStdString();
            if (sf2ripper->exitCode() != 0)
                cout << "An error has occured." << endl;
        }
    }
    else
    {
        // Rips each sound bank in a single soundfont file
        // Build argument list to call sound_font_ripper
        // Output sound font named after the input ROM
        QCoreApplication::processEvents();
        string sf_rip_args = "sound_font_ripper\n" + romPath + "\n" + outPath + "/" + romName + '/' + romName + ".sf2";
        if (sample_rate) sf_rip_args += "\n-s" + to_string(sample_rate);
        if (main_volume) sf_rip_args += "\n-mv" + to_string(main_volume);
        // Pass -gm argument if necessary
        if (gmFlag) sf_rip_args += "\n-gm";

        // Make sound banks addresses list.
        for (bank_t j = sound_bank_list.begin(); j != sound_bank_list.end(); ++j)
            sf_rip_args += "\n0x" + hex(*j);

        QStringList argList = QString::fromStdString(sf_rip_args).split("\n");

        // Call sound font ripper
#ifdef QT_DEBUG
        cout << "DEBUG: Going to call system(" << sf_rip_args << ")" << endl;;
#endif
        //system(sf_rip_args.c_str());
        QProcess *sf2ripper = new QProcess();

#ifdef Q_OS_WIN32
            if (!QFile::exists(QDir::toNativeSeparators(QDir::currentPath() + "/gba_mus_ripper_gui.exe")))
            {
                cout << "Unable to find the " << QDir::toNativeSeparators(QDir::currentPath() + "/gba_mus_ripper_gui.exe").toStdString() << " executable!" << endl;
                return -8;
            }
            sf2ripper->setProgram(QDir::currentPath() + "/gba_mus_ripper_gui.exe");
#else
            if (!QFile::exists(QDir::toNativeSeparators(QDir::currentPath() + "/gba_mus_ripper_gui")))
            {
                cout << "Unable to find the " << QDir::toNativeSeparators(QDir::currentPath() + "/gba_mus_ripper_gui").toStdString() << " executable!" << endl;
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
            cout << "An error has occured." << endl;
    }
    cout << "Rip completed!" << endl;
    return 0;
}
