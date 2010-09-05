#include <nds.h>
#include <stdio.h>
#include <fat.h>

// Included Source Files

#include "gfx.h"
#include "structs.h"
#include "trackclass.h"
#include "midi.h"



// Global Variables

track* tracks[] =
{

	new mididrumtrack(1),
	new mididrumtrack(2),
	new midinotetrack(3),
	new midinotetrack(4),
	new midicctrack(5),
	new midicctrack(6),

};

int playingtracks[6];

int bpm;

int activetracknumber;

int midienable;

int globalplay;

int fatstatus;

int globalstep = 0;
int globalcount = 0;
int bpmcount = 0;


modes_t currentmode;

// Screen Display Functions

void drawtopscreen() {

int x, y;
	for (int i = 0; i < 6; i++)
	{
	
		if ( i < 4) {
			x = i;
			y = 0;
		}
		else
		{
			x = (i - 4);
			y = 10;
		}
		
		drawbigbuttonSUB((x * 8),y,playingtracks[i]);
		
		if (i == activetracknumber)
		{
			drawbiglongbuttonSUB((x * 8),(y + 6),3);
		} else {
			drawbiglongbuttonSUB((x * 8),(y + 6),2);
		}
		
		drawbignumberSUB(((x * 8) + 3),(y + 1),(i + 1));
		
	}
	
	if (((globalstep % 4) == 0) || ((globalstep % 4) == 1))
	{
		drawbiglongbuttonSUB(20,16,1);
	} else {
		drawbiglongbuttonSUB(20,16,0);
	}
	

	
	displaybpm(bpm,20,12);

	drawstepdisplaySUB(globalstep, 11);
	
}


void drawsetupscreen () {

	setupscreenbackground();
	drawsetuptext(midienable);
	
	drawbigbutton(8,12,midienable);

	tracks[activetracknumber]->modebuttondisplay();
}

void drawloadsavescreen () {

	tracks[activetracknumber]->loadsaveview();
	
	tracks[activetracknumber]->modebuttondisplay();

}

void drawfilebrowsescreen () {

	filebrowsescreenbackground();

}

void drawhomescreen () {

int x, y;
	
	for (int i = 0; i < 6; i++)
	{
		if ( i < 4) {
			x = i;
			y = 0;
		}
		else
		{
			x = (i - 4);
			y = 10;
		}
		
		drawbigbutton((x * 8),y,playingtracks[i]);
		
		if (i == activetracknumber)
		{
			drawbiglongbutton((x * 8),(y + 6),3);
		} else {
			drawbiglongbutton((x * 8),(y + 6),2);
		}
		
		mainnumbers(((x * 8) + 3),(y + 1),(i + 1));
	}
	
	drawbigbutton(24,10,globalplay);
	drawbiglongbutton(24,16,2);
	
	drawkeypad(17,11);
	
	calcanddispnumber(17,13,bpm);
	
	tracks[activetracknumber]->modebuttondisplay();

}


void drawoptionsscreen () {
	
	tracks[activetracknumber]->optionsview();
	
	tracks[activetracknumber]->modebuttondisplay();

}


void drawfollowscreen() {

	tracks[activetracknumber]->flowview();
	
	tracks[activetracknumber]->modebuttondisplay();

}



void draweditscreen() {

	tracks[activetracknumber]->editview();
	
	tracks[activetracknumber]->modebuttondisplay();

}

void drawpatternseqscreen() {

	tracks[activetracknumber]->patternseqview();
	
	tracks[activetracknumber]->modebuttondisplay();

}



// Display Updater



void displayupdater () {
	
	switch (currentmode) {
		
		case home:
		
			drawhomescreen();
			break;
			
		case seqpatterns:
		
			drawpatternseqscreen();
			break;
			
		case edit:
		
			draweditscreen();
			break;
			
		case options:
		
			drawoptionsscreen();
			break;
			
		case follow:
		
			drawfollowscreen();
			break;
		
		case setup:
		
			drawsetupscreen();
			break;
			
		case loadsave:
		
			drawloadsavescreen();
			break;
		
		case filebrowse:
		
			drawfilebrowsescreen();
			break;
			
		case misc:
			break;
	}
	
	drawtopscreen();

}


// General Button Press Functions


void changebpm(int changeval) {

	bpm += changeval;

	if (bpm > 300) {
		bpm = 300;
	} else if (bpm < 40) {
		bpm = 40;
	}	

	TIMER_DATA(0) = TIMER_FREQ_64(bpm * 16 * 16);
}

void changetrack(int changeval) {

	activetracknumber += changeval;

	if (activetracknumber > 5) {
		activetracknumber = 0;
	} else if (activetracknumber < 0) {
		activetracknumber = 5;
	}

	clearbottomscreen();
}


void standardbuttons () {

	if (keysDown() & KEY_UP)
	{
	changebpm(10);
	}
	if (keysDown() & KEY_DOWN)
	{
	changebpm(-10);
	}
	if (keysDown() & KEY_LEFT)
	{
	changebpm(-1);
	}
	if (keysDown() & KEY_RIGHT)
	{
	changebpm(1);
	}
	if (keysDown() & KEY_L)
	{
	changetrack(-1);
	}
	if (keysDown() & KEY_R)
	{
	changetrack(1);
	}

}


// View specific button press functions


void setupviewbuttonpresses () {

	touchPosition touch;

	scanKeys();
	
	standardbuttons();


	if (keysDown() & KEY_TOUCH)
	{
		touchRead(&touch);
		
		int xaxispress = touch.px;
		int yaxispress = touch.py;
		
		int yval = (yaxispress / 8);
		int xval = (xaxispress / 8);
		
		if ((xval > 7) && (xval < 16) && (yval > 11) && (yval < 18))
		{
			if (midienable == 0)
			{
				if (initmidi()) {
					midienable = 1;
				}
			}			
			
		} else if (yval > 19)
		{
			tracks[activetracknumber]->modebuttonpress(xaxispress);
		}
	}
}

void optionsviewbuttonpresses () {

	touchPosition touch;

	scanKeys();
	
	standardbuttons();
	
	tracks[activetracknumber]->optionspress();

	if (keysDown() & KEY_TOUCH) {
		touchRead(&touch);
		
		int xaxispress = touch.px;
		int yaxispress = touch.py;
		
		if (yaxispress > 159) {
			
			tracks[activetracknumber]->modebuttonpress(xaxispress);
		}
	}
}


void patternseqbuttonpresses () {

	touchPosition touch;

	scanKeys();

	standardbuttons();

	tracks[activetracknumber]->patternseqpress();

	if (keysDown() & KEY_TOUCH) {
		touchRead(&touch);
		
		int xaxispress = touch.px;
		int yaxispress = touch.py;
		
		if (yaxispress > 159)
		{
			tracks[activetracknumber]->modebuttonpress(xaxispress);	
		}	
	}
}




void followviewbuttonpresses () {

	touchPosition touch;

	scanKeys();

	standardbuttons();
	
	tracks[activetracknumber]->flowpress();

	if (keysDown() & KEY_TOUCH) {
		touchRead(&touch);
		
		int xaxispress = touch.px;
		int yaxispress = touch.py;
		
		if (yaxispress > 159)
		{
			tracks[activetracknumber]->modebuttonpress(xaxispress);
		}	
	}
}

void loadsaveviewbuttonpresses () {


	touchPosition touch;

	scanKeys();
	
	standardbuttons();

	tracks[activetracknumber]->loadsavepress();
	
	if (keysDown() & KEY_TOUCH)
	{
		touchRead(&touch);
		
		int xaxispress = touch.px;
		int yaxispress = touch.py;
		
		if (yaxispress > 159)
		{			
			tracks[activetracknumber]->modebuttonpress(xaxispress);
		}
	}

}

void editviewbuttonpresses () {

	touchPosition touch;

	scanKeys();
	
	standardbuttons();

	tracks[activetracknumber]->editpress();
	

	if (keysDown() & KEY_TOUCH) {
		touchRead(&touch);
		
		int xaxispress = touch.px;
		int yaxispress = touch.py;
		
		if (yaxispress > 159)
		{
			tracks[activetracknumber]->modebuttonpress(xaxispress);
		}	
	}
}

void homeviewbuttonpresses () {

	touchPosition touch;

	scanKeys();
	
	standardbuttons();
	
	if (keysDown() & KEY_START)
	{
		if (globalplay == 1)
		{
			globalplay = 0;
		} else if (globalplay == 0)
		{
			globalplay = 1;
		}
	}
	
	if (keysDown() & KEY_TOUCH) {
		touchRead(&touch);
		
		int xval = (touch.px / 16);
		int yval = (touch.py / 16);
		
		if ((yval == 0) || (yval == 1) || (yval == 2))
		{
			xval = (touch.px / 64);
			
			if(playingtracks[xval] == 0)
			{
				playingtracks[xval] = 1;
			}
			else if(playingtracks[xval] == 1)
			{
				playingtracks[xval] = 0;
			}
		}
		else if (((yval == 5) || (yval == 6) || (yval == 7)) && (xval < 8))
		{
			xval = (touch.px / 64);
			
			if(playingtracks[xval + 4] == 0)
			{
				playingtracks[xval + 4] = 1;
			}
			else if(playingtracks[xval + 4] == 1)
			{
				playingtracks[xval + 4] = 0;
			}
		}
		else if ((yval == 3) || (yval == 4))
		{
			xval = (touch.px / 64);
			
			activetracknumber = xval;
		}
		else if (((yval == 8) || (yval == 9)) && (xval < 8))
		{
			xval = (touch.px / 64);
			
			activetracknumber = xval + 4;
		}
		else if ((xval > 11) && (xval < 16) && (yval > 4) && (yval < 8))
		{
			if (globalplay == 0) {
				globalplay = 1;
			} else if (globalplay == 1) {
				globalplay = 0;
			}
		}
		else if ((xval > 11) && (xval < 16) && (yval > 7) && (yval < 10))
		{
			currentmode = setup;
		}
		else if ((xval > 7) && (xval < 12) && (yval > 4) && (yval < 9))
		{	
			xval = ((touch.px + 8) / 16);
			yval = ((touch.py + 8) / 16);
		
			if (yval == 6)
			{
				if (xval == 9)
				{
					changebpm(100);
				}
				if (xval == 10)
				{
					changebpm(10);
				}
				if (xval == 11)
				{
					changebpm(1);
				}
			}
			
			if (yval == 8)
			{
				if (xval == 9)
				{
					changebpm(-100);
				}
				if (xval == 10)
				{
					changebpm(-10);
				}
				if (xval == 11)
				{
					changebpm(-1);
				}
			}
		}
		else
		{
			
			int xaxispress = touch.px;
			int yaxispress = touch.py;
			
			if (yaxispress > 159)
			{
				tracks[activetracknumber]->modebuttonpress(xaxispress);
			}
		}
		
	}
}



// Mode Functions



void filebrowseview () {

	while(currentmode == filebrowse)
    {

    }
	
	clearbottomscreen();
	
}

void setupview () {

	while(currentmode == setup)
    {
		swiWaitForVBlank();
		
		setupviewbuttonpresses();
    }
	
	clearbottomscreen();
	
}

void loadsaveview () {

	while(currentmode == loadsave)
    {
		swiWaitForVBlank();
		
		loadsaveviewbuttonpresses();
    }
	
	clearbottomscreen();
	
}

void homeview () {

	while(currentmode == home)
    {
		swiWaitForVBlank();
		
		homeviewbuttonpresses();
    }
	
	clearbottomscreen();
}

void followview () {

	while(currentmode == follow)
    {
		swiWaitForVBlank();
		
		followviewbuttonpresses();
    }
	
	clearbottomscreen();
}

void patternseqview () {

	while(currentmode == seqpatterns)
    {
		swiWaitForVBlank();
		
		patternseqbuttonpresses();
		
    }
	
	clearbottomscreen();
}

void editview () {

	while(currentmode == edit)
    {
		swiWaitForVBlank();
		
		editviewbuttonpresses();
		
    }
	
	clearbottomscreen();
}



void optionsview () {

	while(currentmode == options)
    {
		swiWaitForVBlank();		
		
		optionsviewbuttonpresses();
		
	}
	
	clearbottomscreen();
	
}


// Sequencer Functions



void triggerglobalstep() {
	
	if (globalcount == 0) {
	
		if (globalstep == 0)
		{
			for (int i = 0; i < 6; i++ )
			{
				
				tracks[i]->starttrack(playingtracks[i]);
			}
		}		
	}
	
	globalcount++;
		
	if (globalcount > 15) {
		globalcount = 0;
		globalstep++;
		if (globalstep > 15) { globalstep = 0; }
	}
	
}


void bpmtimer() {

	if (globalplay == 1)
	{		
		if (bpmcount == 0)
		{			
			triggerglobalstep();
			
			for (int i = 0; i < 6; i++ )
			{
			tracks[i]->sequencerclock();
			}
			
			for (int i = 0; i < 6; i++ )
			{
				tracks[i]->triggernotes();
			}
		}
		
		bpmcount++;
		if (bpmcount == 60 * 4) {
			bpmcount = 0;
		}
		
	} else {

		bpmcount = 0;
		globalstep = 0;
		globalcount = 0;
		
		for (int i = 0; i < 6; i++ )
		{
			tracks[i]->resettrack();
		}
	}
}


void setupbpmtimer() {

	TIMER_DATA(0) = TIMER_FREQ_64(bpm * 16 * 16);
	TIMER_CR(0) = TIMER_DIV_64 | TIMER_ENABLE | TIMER_IRQ_REQ; 

	irqEnable  	(IRQ_TIMER0);
	irqSet (IRQ_TIMER0,bpmtimer);
	irqSet(IRQ_VBLANK, displayupdater);

}

// Main



int main(void) {

	currentmode = home;
	bpm = 120;
	activetracknumber = 0;
	midienable = 0;
	globalplay = 0;

	swiWaitForVBlank();
	
    setupVideo();
	setupbpmtimer();

	PrintConsole bottomScreen;
	consoleInit(&bottomScreen, 0,BgType_Text4bpp, BgSize_T_256x256, 31, 3, true, true);
	consoleSelect(&bottomScreen);


	if (fatInitDefault()) {
		fatstatus = 1;
	} else {
		fatstatus = 0;
	}


	while (1)
	{
		switch (currentmode) {
			
			case home:
			
				homeview();
				break;
				
			case seqpatterns:
			
				patternseqview();
				break;
				
			case edit:
			
				editview();
				break;
				
			case options:
			
				optionsview();
				break;
				
			case follow:
			
				followview();
				break;
				
			case setup:
			
				setupview();
				break;
				
			case loadsave:
			
				loadsaveview();
				break;
				
			case filebrowse:
			
				filebrowseview();
				break;
				
			case misc:
			
				break;
				
		}
	}

    return 0;
}
