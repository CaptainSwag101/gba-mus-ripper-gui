#include <forward_list>
#include <string>
#include "midi.h"
#include "note.h"

#ifndef SONG_RIPPER_H
#define SONG_RIPPER_H

// Global variables for this program

using namespace std;

uint32_t track_ptr[16];
uint8_t last_cmd[16];
char last_key[16];
char last_vel[16];
int counter[16];
uint32_t return_ptr[16];
int key_shift[16];
bool return_flag[16];
bool track_completed[16];
bool end_flag = false;
bool loop_flag = false;
uint32_t loop_adr;

int lfo_delay_ctr[16];
int lfo_delay[16];
int lfo_depth[16];
int lfo_type[16];
bool lfo_flag[16];
bool lfo_hack[16];

unsigned int simultaneous_notes_ctr = 0;
unsigned int simultaneous_notes_max = 0;

static forward_list<Note> notes_playing;

int bank_number;
bool bank_used = false;

MIDI midi(24);

void process_event(int track);

int ripSong(int argc, string args[]);

#endif // SONG_RIPPER_H
