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
	new midinotetrack(2),
	new mididrumtrack(3),
	new midicctrack(4),

};

int playingtracks[4];

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

	navbuttons(1,5,currentmode);
	
	drawbigbutton(8,12,midienable);

}

void drawhomescreen () {

	
	for (int i = 0; i < 4; i++)
	{
		drawbigbutton((i * 8),0,playingtracks[i]);
		
		if (i == activetracknumber)
		{
			drawbiglongbutton((i * 8),6,3);
		} else {
			drawbiglongbutton((i * 8),6,2);
		}
		
		mainnumbers(((i * 8) + 3),1,(i + 1));
	}
	
	drawbigbutton(12,12,globalplay);
	
	drawkeypad(4,12);
	
	calcanddispnumber(4,14,bpm);
	
	navbuttons(1,5,currentmode);
	navbuttonwords();

}


void drawoptionsscreen () {
	
	tracks[activetracknumber]->optionsview();
	
	navbuttonwords();
	navbuttons(1,5,currentmode);

}


void drawfollowscreen() {

	tracks[activetracknumber]->flowview();
	
	navbuttons(1,5,currentmode);
	navbuttonwords();

}



void draweditscreen() {

	tracks[activetracknumber]->editview();
	
	navbuttons(1,5,currentmode);
	navbuttonwords();

}

void drawpatternseqscreen() {

	tracks[activetracknumber]->patternseqview();
	
	navbuttons(1,5,currentmode);
	navbuttonwords();

}

void drawloadsavescreen() {

	filebrowsescreenbackground();

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
	}
	
	drawtopscreen();

}


// Mode Button Press Functions



void navbuttonpresses (int xval) {

	if (xval == 0)
	{
		currentmode = home;
	} else if (xval == 1)
	{
		currentmode = edit;
		tracks[activetracknumber]->currenteditpattern = 0;
	} else if (xval == 2)
	{
		currentmode = seqpatterns;
		tracks[activetracknumber]->currenteditpattern = 7;
	} else if (xval == 3)
	{	
		currentmode = follow;
	} else if (xval == 4)
	{
		currentmode = options;
	} else if (xval == 5)
	{
		currentmode = setup;
	} else if (xval == 6)
	{	 
		tracks[activetracknumber]->filesave();
	} else if (xval == 7 )
	{
		tracks[activetracknumber]->fileload();
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

	if (activetracknumber > 3) {
		activetracknumber = 0;
	} else if (activetracknumber < 0) {
		activetracknumber = 3;
	}	

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
		
		int xval = (touch.px / 8);
		int yval = (touch.py / 8);
		
		if (yval < 6)
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
		} else if ((yval > 5) && (yval < 10))
		{
			if (xval < 8) {
			activetracknumber = 0;
			}
			if ((xval > 7) && (xval < 16)) {
			activetracknumber = 1;
			}
			if ((xval > 15) && (xval < 24)) {
			activetracknumber = 2;
			}
			if ((xval > 23)) {
			activetracknumber = 3;
			}
		} else if ((xval > 11) && (xval < 20) && (yval > 11) && (yval < 18))
		{
			if (globalplay == 0) {
				globalplay = 1;
			} else if (globalplay == 1) {
				globalplay = 0;
			}
			
			
		} else if ((xval > 3) && (xval < 12) && (yval > 11) && (yval < 20))
		{
			xval = (touch.px / 16);
			yval = (touch.py / 16);
			
			if (yval == 6)
			{
				if (xval == 2)
				{
					changebpm(100);
				}
				if (xval == 3)
				{
					changebpm(10);
				}
				if (xval == 4)
				{
					changebpm(1);
				}
			}
			
			if (yval == 8)
			{
				if (xval == 2)
				{
					changebpm(-100);
				}
				if (xval == 3)
				{
					changebpm(-10);
				}
				if (xval == 4)
				{
					changebpm(-1);
				}
			}
		} else {
			
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


void triggertracks () {

	for (int i = 0; i < 4; i++ )
	{
		tracks[i]->sequencerclock();
	}

}


void bpmtimer() {

	if (globalplay == 1)
	{	
		
		if (bpmcount == 0)
		{			
			triggerglobalstep();
			triggertracks();
		}
		
		bpmcount++;
		if (bpmcount == 60 * 4) {
			bpmcount = 0;
		}
		
		for (int i = 0; i < 4; i++ )
		{
			tracks[i]->sendmididata();
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
			
				break;
		}
	}

    return 0;
}
