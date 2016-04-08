#include "midi.h"

#ifndef NOTE_H
#define NOTE_H


class Note
{
    MIDI& midi;
    int counter;
    int key;
    int vel;
    int chn;
    bool event_made;

    friend void make_note_on_event(Note& n);

public:
    static bool countdown_is_over(Note& n);
    Note(MIDI& midi, int chn, int len, int key, int vel);
};

#endif // NOTE_H
