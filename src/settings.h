#pragma once

#include <stdint.h>

namespace midier
{
namespace settings
{

constexpr float GateDutyCycle = 0.5f;
constexpr uint8_t MidiChannel = 0;
constexpr char MaxBars = 48;

// Set to true to enable MIDI input (clock sync, transport).
// Requires a dedicated Serial1 port for MIDI input on boards with
// only one UART, since MIDI output already uses Serial.
constexpr bool MidiInputEnabled = false;

} // settings
} // midier
