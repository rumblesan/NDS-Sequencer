#ifndef MIDI_H
#define MIDI_H

void midinote (uint8_t channel, uint8_t note, uint8_t velocity);

bool initmidi ();

#endif //MIDI_H