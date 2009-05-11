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
modes_t previousmode;


// Screen Display Functions

void drawtopscreen() {
	
	for (int i = 0; i < 4; i++)
	{
	
		drawbigbuttonSUB(i, 0, playingtracks[i]);
		
		if (i == activetracknumber)
		{
			drawbiglongbuttonSUB((i * 8), 6, 3);
		} else {
			drawbiglongbuttonSUB((i * 8), 6, 2);
		}
		
		drawbignumberSUB(((i * 8) + 3),1,(i + 1));
		
	}
	
	if (((globalstep % 4) == 0) || ((globalstep % 4) == 1))
	{
		drawbiglongbuttonSUB(22,18,1);
	} else {
		drawbiglongbuttonSUB(22,18,0);
	}
	
	drawbpmtextSUB(2,18);
	
	displaybpm(bpm,12,18);

	drawstepdisplaySUB(globalstep, 11);
	
}


void drawsetupscreen () {

	setupscreenbackground();
	navbuttonwords();
	drawsetuptext(midienable);

	modebuttons(currentmode);
	
	drawbigbutton(8,12,midienable);

}

void drawloadsavescreen () {

	setupscreenbackground();
	navbuttonwords();

	modebuttons(currentmode);
	

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
	
	modebuttons(currentmode);
	navbuttonwords();

}


void drawoptionsscreen () {
	
	tracks[activetracknumber]->optionsview();
	
	navbuttonwords();
	modebuttons(currentmode);

}


void drawfollowscreen() {

	tracks[activetracknumber]->flowview();
	
	modebuttons(currentmode);
	navbuttonwords();

}



void draweditscreen() {

	tracks[activetracknumber]->editview();
	
	modebuttons(currentmode);
	navbuttonwords();

}

void drawpatternseqscreen() {

	tracks[activetracknumber]->patternseqview();
	
	modebuttons(currentmode);
	navbuttonwords();

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
			
		case misc:
		
			break;
	}
	
	drawtopscreen();

}


// Mode Button Press Functions



void navbuttonpresses (int xval) {

	if ((xval == 0) || (xval == 1))
	{
		currentmode = home;
	}
	else if (xval == 2)
	{
		currentmode = edit;
	}
	else if (xval == 3)
	{
		currentmode = seqpatterns;
	}
	else if (xval == 4)
	{	
		currentmode = follow;
	}
	else if (xval == 5)
	{
		currentmode = options;
	}
	else if (xval == 5)
	{

	} else if ((xval == 6) || (xval == 7))
	{	 
		currentmode = loadsave;
	}


}



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
			int xval = (xaxispress / 32);
			
			navbuttonpresses(xval);
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
			
			navbuttonpresses((xaxispress / 32));
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
		
		int xval = (touch.px / 32);
		int yval = (touch.py / 32);
		
		if (yval == 5)
		{
			navbuttonpresses(xval);	
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
		
		int xval = (touch.px / 32);
		int yval = (touch.py / 32);

		if (yval == 5)
		{
			navbuttonpresses(xval);
		}	
	}
}

void loadsaveviewbuttonpresses () {


	touchPosition touch;

	scanKeys();
	
	standardbuttons();

	if (keysDown() & KEY_TOUCH)
	{
		touchRead(&touch);
		
		int xaxispress = touch.px;
		int yaxispress = touch.py;
		
		int yval = (yaxispress / 8);
		
		if (yval > 19)
		{
			int xval = (xaxispress / 32);
			
			navbuttonpresses(xval);
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
		
		int xval = (touch.px / 32);
		int yval = (touch.py / 32);

		if (yval == 5)
		{
			navbuttonpresses(xval);
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
			
			int xval = (touch.px / 32);
			int yval = (touch.py  / 32);
			
			if (yval == 5)
			{
				navbuttonpresses(xval);
			}
		}
		
	}
}



// Mode Functions



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
			for (int i = 0; i < 4; i++ )
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
			
			for (int i = 0; i < 4; i++ )
			{
			tracks[i]->sequencerclock();
			}
			
			for (int i = 0; i < 4; i++ )
			{
				tracks[i]->sendmididata();
			}
		}
		
		bpmcount++;
		if (bpmcount == 60 * 4) {
			bpmcount = 0;
		}
		
	} else {

		bpmcount = 0;
		globalstep = 0;
		
		for (int i = 0; i < 4; i++ )
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
				
			case misc:
			
				break;
				
		}
	}

    return 0;
}
