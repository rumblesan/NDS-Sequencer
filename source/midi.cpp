
#include <nds.h>
#include <libdsmi.h>

int dsmienable = 0;


void midinote (uint8_t channel, uint8_t note, uint8_t velocity) {

	if (dsmienable)
	{
		dsmi_write(NOTE_ON|channel, note, velocity);
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
