// "Sequencer/Advanced/ExternalClock" - Syncs the sequencer to an external MIDI clock
//
// This example shows how to synchronise Midier's sequencer to an external
// MIDI clock source (e.g. a DAW, drum machine, or another hardware sequencer).
//
// The sequencer is set to external clock mode. Each call to click() polls for
// incoming MIDI clock ticks and fires the correct number of internal
// subdivisions automatically. Start and Stop transport messages are handled
// as well — both silence all layers and return to wander state.
//
// Hardware requirements:
//   - A board with at least two hardware UARTs (e.g. Arduino Mega, Due,
//     Teensy, ESP32). Serial is used for MIDI output and Serial1 for MIDI
//     input.
//   - A MIDI input circuit connected to Serial1 (RX1).
//   - An external device sending MIDI clock on the same cable.
//
// Software requirements:
//   - In src/settings.h, set MidiInputEnabled to true.
//
// Setup needed to run the examples: https://github.com/razrotenberg/Midier#setup
//
#include <Midier.h>

midier::Layers<8> layers;
midier::Sequencer sequencer(layers);

void setup()
{
    // MIDI output on Serial — use 31250 for a hardware MIDI connection
    Serial.begin(31250);

    // MIDI input on Serial1 — must also be 31250 for standard MIDI
    Serial1.begin(31250);

    // use external clock: timing comes from incoming MIDI clock ticks
    sequencer.clock = midier::Sequencer::Clock::External;

    // assistance is not supported in external clock mode
    sequencer.assist = midier::Sequencer::Assist::No;

    // start a layer so there is something to hear when the clock arrives
    sequencer.start(1);
}

void loop()
{
    // call click() in every iteration — in external clock mode it polls
    // for MIDI input internally and only advances when a clock tick arrives
    sequencer.click(midier::Sequencer::Run::Async);

    // you can do other things here (read buttons, update LEDs, etc.)
}
