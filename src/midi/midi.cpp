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
    // single atomic write: some USB-MIDI bridges mis-packetize
    // messages whose bytes arrive with gaps in between
    const byte message[3] = {
        (byte)((command & 0xF0) | (channel & 0x0F)),
        (byte)(data1 & 0x7F),
        (byte)(data2 & 0x7F),
    };
    Serial.write(message, sizeof(message));
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

} // midi
} // midier
