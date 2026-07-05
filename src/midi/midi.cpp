#include "midi.h"

#include "../settings.h"
#include "../debug/debug.h"

#include <Arduino.h>

namespace midier
{
namespace midi
{

namespace
{

void send(byte command, byte data1, byte data2)
{
    constexpr auto channel = settings::MidiChannel;

#ifndef DEBUG
    Serial.write((command & 0xF0) | (channel & 0x0F));
    Serial.write(data1 & 0x7F);
    Serial.write(data2 & 0x7F);
#else
    // TRACE_4(F("Sending MIDI command NOTE-"), command == 0x90 ? F("ON") : F("OFF"), " #", (int)data1);
#endif
}

} //

Number number(Note note, Octave octave)
{
    return 24 + (12 * (octave - 1)) + (char)note;
}

void (*on_note_on)(Number) = nullptr;

void on(Number number, Velocity velocity)
{
    send(0x90, number, (char)velocity);

    if (on_note_on != nullptr)
    {
        on_note_on(number);
    }
}

void off(Number number)
{
    send(0x80, number, 0);
}

void play(Note note, unsigned duration)
{
    play(note, 3, duration); // playing notes in octave 3 by default
}

void play(Note note, Octave octave, unsigned duration)
{
    const auto no = number(note, octave);

    on(no);
    delay(duration);
    off(no);
}

// ---------------------------------------------------------------------------
// MIDI Input
// ---------------------------------------------------------------------------

Event poll()
{
    if (!settings::MidiInputEnabled)
    {
        return Event::None;
    }

    Event event = Event::None;

    // Drain the receive buffer so we don't fall behind.
    // Keep only the last real-time message found in this pass.
    while (Serial1.available() > 0)
    {
        const auto byte = Serial1.read();

        switch (byte)
        {
            case 0xF8: event = Event::Clock; break;
            case 0xFA: event = Event::Start; break;
            case 0xFC: event = Event::Stop;  break;
            default: break; // ignore everything else
        }
    }

    return event;
}

} // midi
} // midier
