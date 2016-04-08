#include "note.h"
#include "song_ripper.h"
#include "midi.h"

// Note class
// this was needed to properly handle polyphony on all channels...

// Create note and key on event
Note::Note(MIDI& midi, int chn, int len, int key, int vel)
{
    event_made = false;

    start_lfo(chn);
    add_simultaneous_note();
}

// Tick counter, if it becomes zero
// then create key off event
// this function returns "true" when the note should be freed from memory
bool Note::tick()
{
    if(counter > 0 && --counter == 0)
    {
        midi.add_note_off(chn, key, vel);
        stop_lfo(chn);
        simultaneous_notes_ctr--;
        return true;
    }
    else
        return false;
}

static bool Note::countdown_is_over(Note& n)
{
    return n.tick() || counter < 0;
}
