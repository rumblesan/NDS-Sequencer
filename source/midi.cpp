
#include <nds.h>
#include <libdsmi.h>

int dsmienable = 0;


void midinote (uint8_t channel, uint8_t note, uint8_t velocity) {

	if (dsmienable)
	{
		dsmi_write(NOTE_ON|channel, note, velocity);
	}

}


void midicc (uint8_t channel, uint8_t ccnumber, uint8_t value) {

	if (dsmienable)
	{
		dsmi_write(MIDI_CC|channel, ccnumber, value);
	}

}


bool initmidi () {

	if (dsmi_connect())
	{
		dsmienable = 1;
		return true;
	} else {
		return false;
	}

}
