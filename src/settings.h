#pragma once

#include <stdint.h>

namespace midier
{
namespace settings
{

// compile-time library settings

// the MIDI channel all notes are sent on (0-15, i.e. channels 1-16)
constexpr uint8_t MidiChannel = 0;

// the # of bars in the logical loop (see `Time::Bars`)
constexpr char MaxBars = 48;

} // settings
} // midier
