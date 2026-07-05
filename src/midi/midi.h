#pragma once

#include "../note/note.h"
#include "../octave/octave.h"

namespace midier
{
namespace midi
{

enum class Velocity : char
{
    // values are MIDI velocity

    High = 127, // maximum velocity
    Low  = 75,
};

// represents a MIDI note number
using Number = unsigned char;

// calculate MIDI note number from a musical note and an octave
Number number(Note note, Octave octave);

// send a 'NOTE_ON' MIDI command
void on(Number number, Velocity velocity = Velocity::High); // by default max velocity

// optional application hook: called after every NOTE_ON is sent
extern void (*on_note_on)(Number number);

// send a 'NOTE_OFF' MIDI command
void off(Number number);

// play a musical note for a specific duration of time (in ms)
void play(Note note,                unsigned duration = 200);
void play(Note note, Octave octave, unsigned duration = 200);

// ---------------------------------------------------------------------------
// MIDI Input (optional, guarded by settings::MidiInputEnabled)
// ---------------------------------------------------------------------------

// MIDI real-time events relevant to clock synchronisation
enum class Event : char
{
    None,   // no relevant message received
    Clock,  // 0xF8 — timing clock tick (24 PPQN)
    Start,  // 0xFA — start transport
    Stop,   // 0xFC — stop transport
};

// Non-blocking read from the MIDI input serial port.
// Returns the most recent real-time event found in the buffer,
// or Event::None if nothing relevant was available.
// Only functional when settings::MidiInputEnabled is true;
// otherwise always returns Event::None.
Event poll();

} // midi
} // midier
