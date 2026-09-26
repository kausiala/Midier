// "Sequencer/Advanced/ExternalClock" - Follows an external MIDI clock
//
// Instead of letting the sequencer keep time with click(), this sketch reads
// the MIDI real-time messages of a master device (a DAW, a drum machine,
// another sequencer) and advances the sequencer with tick():
//
//   Clock (0xF8)    = advance by Time::SubdivisionsPerMidiClock subdivisions
//                     (MIDI clock runs at 24 ticks per quarter note)
//   Start (0xFA)    = (re)start the layer on the master's downbeat
//   Continue (0xFB) = start the layer where the master is
//   Stop (0xFC)     = stop the layer (its sounding note is released)
//
// Hardware requirements:
//   - A board with a second hardware UART (e.g. Arduino Mega, Leonardo, Due,
//     Teensy): Serial sends MIDI out and Serial1 receives MIDI in.
//   - A MIDI input circuit on Serial1 (RX1), connected to a device that sends
//     MIDI clock.
//
// Setup needed to run the examples: https://github.com/razrotenberg/Midier#setup
//
#include <Midier.h>

midier::Layers<8> layers;
midier::Sequencer sequencer(layers);
midier::Sequencer::Handle handle;
bool playing = false;

void play()
{
    if (!playing)
    {
        handle = sequencer.start(1);
        playing = true;
    }
}

void stop()
{
    if (playing)
    {
        sequencer.stop(handle);
        playing = false;
    }
}

void setup()
{
    Serial.begin(31250);  // MIDI out
    Serial1.begin(31250); // MIDI in
}

void loop()
{
    // handle every byte: skipping clock ticks would make the layers drift
    while (Serial1.available() > 0)
    {
        const byte message = Serial1.read();

        if (message == 0xF8) // clock
        {
            sequencer.tick(midier::Time::SubdivisionsPerMidiClock);
        }
        else if (message == 0xFA) // start: the next tick is the master's downbeat
        {
            stop();
            midier::Time::now = midier::Time(0, 0);
            play();
        }
        else if (message == 0xFB) // continue
        {
            play();
        }
        else if (message == 0xFC) // stop
        {
            stop();
        }
    }

    // do other things here (read buttons, update LEDs, etc.) - but don't call
    // click(): the incoming clock is what advances the sequencer
}
