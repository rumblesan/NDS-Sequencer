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

};

int playingtracks[4];

int globalnumber = 1;

int bpm = 120;

int activetracknumber;

int setuprow = -1;

int midienable = 0;

int globalplay = 0;
int globalstep = -1;

int midisyncclock = -1;

int bpmcount;

modes_t currentmode;


/* global setting load and save functions
void globalfileloader() {

	filebrowsescreenbackground();

	trackbuffer globalloadsave;

	char filePath[MAXPATHLEN * 2];
	int pathLen;
	std::string filename;
	FILE * pFile;
	
	filename = browseForFile (".glb");

	if (filename != "NULL")
	{
		// Construct a command line if we weren't supplied with one
		getcwd (filePath, MAXPATHLEN);
		pathLen = strlen (filePath);
		strcpy (filePath + pathLen, filename.c_str());
		
		pFile = fopen ( filePath , "r" );
		
		fread((char *)&globalloadsave, sizeof(globalloadsave), 1, pFile);
	
		int i, x, y, z;
	
		for (i = 0; i < 4; i++)
		{
			
			tracks[i].patternnumber = globalloadsave.patternnumber[i];
			tracks[i].presetnumber = globalloadsave.presetnumber[i];
			
			tracks[i].midichannel = globalloadsave.midichannel[i];
			
			tracks[i].patternseqlength = globalloadsave.patternseqlength[i];
			
			for (x = 0; x < 8; x ++)
			{
				for (y = 0; y < 3; y ++)
				{
					tracks[i].midinotes[x][y] = globalloadsave.midinotes[i][x][y];
				}
			}
			
			for ( z = 0; z < 8; z++ )
			{				
				for( x = 0; x < 16; x++ )
				{
					for( y = 0; y < 8; y++ )
					{
						tracks[i].patterns[z][x][y] = globalloadsave.patterns[i][z][x][y];
					}
				}
			}
		}
		
		fclose (pFile);
	}
	
	clearbottomscreen();

}

void globalfilesaver() {

	trackbuffer globalloadsave;
	
	char format[] = "/seqgrid/files/global-%i.glb";
	char filename[sizeof format+100];
	sprintf(filename,format,globalnumber);
	FILE *pFile = fopen(filename,"w");

	int i, x, y, z;

	for (i = 0; i < 4; i++)
	{
		
		globalloadsave.patternnumber[i] = tracks[i].patternnumber;
		globalloadsave.presetnumber[i] = tracks[i].presetnumber;
		
		globalloadsave.midichannel[i] = tracks[i].midichannel;
		
		globalloadsave.patternseqlength[i] = tracks[i].patternseqlength;
		
		for (x = 0; x < 8; x ++)
		{
			for (y = 0; y < 3; y ++)
			{
				globalloadsave.midinotes[i][x][y] = tracks[i].midinotes[x][y];
			}
		}
		
		for ( z = 0; z < 8; z++ )
		{			
			for( x = 0; x < 16; x++ )
			{
				for( y = 0; y < 8; y++ )
				{
					globalloadsave.patterns[i][z][x][y] = tracks[i].patterns[z][x][y];
				}
			}
		}
	}

	fwrite((char *)&globalloadsave, sizeof(trackbuffer), 1, pFile);

	fclose (pFile);

}


*/


void incrementglobalval(int amount) {
	
	int tempvalue;

	if (setuprow == 4) {
	
		tempvalue = globalnumber + amount;
		
		if (tempvalue > 200)
		{
			tempvalue = 200;
		}
		if (tempvalue < 0)
		{
			tempvalue = 0;
		}
		
		globalnumber = tempvalue;
		
	}
	
}
// Edit and Follow View Functions


void drawsetupscreen () {

	setupscreenbackground(setuprow);
	navbuttonwords();
	drawsetuptext(midienable, globalnumber);

	drawkeypad(24,2);

	navbuttons(1,5,currentmode);
	
	displaysetuprowvalue(setuprow, globalnumber);
	
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
	
	tracks[activetracknumber]->displayoptions();
	
	navbuttonwords();
	navbuttons(1,5,currentmode);

}


void drawfollowscreen() {

	tracks[activetracknumber]->displayactivepattern();
	
	navbuttons(2,4,tracks[activetracknumber]->currenteditpattern);
	navbuttons(1,5,currentmode);
	
	navbuttonwords();

}



void draweditscreen() {

	tracks[activetracknumber]->displaypattern();
	
	navbuttons(2,4,tracks[activetracknumber]->currenteditpattern);
	navbuttons(1,5,currentmode);
	
	navbuttonwords();

}

void drawpatternseqscreen() {

	tracks[activetracknumber]->displaypatternseq();
	
	navbuttons(2,4,tracks[activetracknumber]->currenteditpattern);
	navbuttons(1,5,currentmode);
	
	navbuttonwords();

}
// Mode Button Press Functions


void navbuttonpresses (int xval) {

	if (xval == 0)
	{
		currentmode = home;
	}
	if (xval == 1)
	{
		currentmode = edit;
		tracks[activetracknumber]->currenteditpattern = 0;
	}
	if (xval == 2)
	{
		currentmode = seqpatterns;
		tracks[activetracknumber]->currenteditpattern = 7;
	}
	if (xval == 3)
	{	
		currentmode = follow;
	}
	if (xval == 4)
	{
		currentmode = options;
	}
	if (xval == 5)
	{
		currentmode = setup;
	}
	if (xval == 6)
	{	 
		tracks[activetracknumber]->filesave(currentmode);
	}
	if (xval == 7 )
	{
		tracks[activetracknumber]->fileload(currentmode);			
	}


}

void changebpm(int changeval) {

	bpm += changeval;

	if (bpm > 300) {
		bpm = 300;
	}
		if (bpm < 40) {
		bpm = 40;
	}	

	int onebeat = (bpm * 16 * 24);

	TIMER_DATA(0) = TIMER_FREQ_64(onebeat);
}

void changetrack(int changeval) {

	activetracknumber += changeval;

	if (activetracknumber > 3) {
		activetracknumber = 0;
	}
		if (activetracknumber < 0) {
		activetracknumber = 3;
	}	

}

void setupviewbuttonpresses () {

	touchPosition touch;

	scanKeys();
	
	if (keysDown() & KEY_UP) {
	changebpm(10);
	}
	if (keysDown() & KEY_DOWN) {
	changebpm(-10);
	}
	if (keysDown() & KEY_LEFT) {
	changebpm(-1);
	}
	if (keysDown() & KEY_RIGHT) {
	changebpm(1);
	}
	if (keysDown() & KEY_L) {
	changetrack(-1);
	}
	if (keysDown() & KEY_R) {
	changetrack(1);
	}

	
	
	if (keysDown() & KEY_TOUCH) {
		touchRead(&touch);
		
		int xaxispress = touch.px;
		int yaxispress = touch.py;
		
		int yval = (yaxispress / 8);
		int xval = (xaxispress / 8);
		
		if ((yval == 4) && (xval > 16) && (xval < 20))
		{
			setuprow = 4;
			
		} else if ((xval > 23) && (xval < 30) && (yval > 1) && (yval < 9))
		{
			
			int xval = (xaxispress / 16);
			int yval = (yaxispress / 16);
			
			if (setuprow > 0)
			{			
				if (yval == 1)
				{
					if (xval == 12)
					{
						incrementglobalval(100);
					}
					if (xval == 13)
					{
						incrementglobalval(10);
					}
					if (xval == 14)
					{
						incrementglobalval(1);
					}
				}
				
				if (yval == 3)
				{
					if (xval == 12)
					{
						incrementglobalval(-100);
					}
					if (xval == 13)
					{
						incrementglobalval(-10);
					}
					if (xval == 14)
					{
						incrementglobalval(-1);
					}
				}
			}
		} else if ((xval > 7) && (xval < 16) && (yval > 11) && (yval < 18))
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
	
	if (keysDown() & KEY_UP) {
	changebpm(10);
	}
	if (keysDown() & KEY_DOWN) {
	changebpm(-10);
	}
	if (keysDown() & KEY_LEFT) {
	changebpm(-1);
	}
	if (keysDown() & KEY_RIGHT) {
	changebpm(1);
	}
	if (keysDown() & KEY_L) {
	changetrack(-1);
	}
	if (keysDown() & KEY_R) {
	changetrack(1);
	}
	
	if (keysDown() & KEY_TOUCH) {
		touchRead(&touch);
		
		int xaxispress = touch.px;
		int yaxispress = touch.py;
		int xval;
		
		if (yaxispress < 160) {
		
			tracks[activetracknumber]->optionspress(xaxispress, yaxispress);
			
		} else {
		
			xval = (xaxispress / 32);
			
			navbuttonpresses(xval);
		}
	}
}


void patternseqbuttonpresses () {

	touchPosition touch;

	scanKeys();
	
	if (keysDown() & KEY_UP) {
		changebpm(10);
	}
	if (keysDown() & KEY_DOWN) {
		changebpm(-10);
	}
	if (keysDown() & KEY_LEFT) {
		changebpm(-1);
	}
	if (keysDown() & KEY_RIGHT) {
		changebpm(1);
	}
	if (keysDown() & KEY_L) {
	changetrack(-1);
	}
	if (keysDown() & KEY_R) {
	changetrack(1);
	}
	if (keysDown() & KEY_START)
	{
		if(playingtracks[activetracknumber] == 0)
		{
			playingtracks[activetracknumber] = 1;
		}
		else if(playingtracks[activetracknumber] == 1)
		{
			playingtracks[activetracknumber] = 0;
		}
	}	
	
	if (keysDown() & KEY_TOUCH) {
		touchRead(&touch);
		
		int xaxispress = touch.px;
		int yaxispress = touch.py;
		
		int xval = (xaxispress / 16);
		int yval = (yaxispress / 16);

		if (yval < 8)
		{
			tracks[activetracknumber]->patternseqpress(xval,yval);
		}
		
		xval = (xval / 2);
		yval = (yval / 2);
		
		if (yval == 4)
		{
			
			if (xval < 7)
			{
				currentmode = edit;
				tracks[activetracknumber]->currenteditpattern = xval;
			}
			else if(xval == 7)
			{
				currentmode = seqpatterns;
				tracks[activetracknumber]->currenteditpattern = xval;
			}		
		}
		if (yval == 5)
		{
			navbuttonpresses(xval);	
		}	
	}
}




void followviewbuttonpresses () {

	touchPosition touch;

	scanKeys();
	
	if (keysDown() & KEY_UP) {
	changebpm(10);
	}
	if (keysDown() & KEY_DOWN) {
	changebpm(-10);
	}
	if (keysDown() & KEY_LEFT) {
	changebpm(-1);
	}
	if (keysDown() & KEY_RIGHT) {
	changebpm(1);
	}
	if (keysDown() & KEY_L) {
	changetrack(-1);
	}
	if (keysDown() & KEY_R) {
	changetrack(1);
	}
	if (keysDown() & KEY_START)
	{
		if(playingtracks[activetracknumber] == 0)
		{
			playingtracks[activetracknumber] = 1;
		}
		else if(playingtracks[activetracknumber] == 1)
		{
			playingtracks[activetracknumber] = 0;
		}
	}
	
	if (keysDown() & KEY_TOUCH) {
		touchRead(&touch);
			
		int xaxispress = touch.px;
		int yaxispress = touch.py;
		
		int xval = (xaxispress / 16);
		int yval = (yaxispress / 16);

		if (yval < 8)
		{
			tracks[activetracknumber]->editpress(xval,yval);
		}
		
		xval = (xval / 2);
		yval = (yval / 2);
		
		if (yval == 4)
		{	
			if (xval < 7)
			{
				tracks[activetracknumber]->currenteditpattern = xval;
				currentmode = edit;
			}
			else if(xval == 7)
			{
				tracks[activetracknumber]->currenteditpattern = xval;
				currentmode = seqpatterns;
			}
		}
		if (yval == 5)
		{
			navbuttonpresses(xval);
		}	
	}
}

void editviewbuttonpresses () {

	touchPosition touch;

	scanKeys();
	
	if (keysDown() & KEY_UP) {
	changebpm(10);
	}
	if (keysDown() & KEY_DOWN) {
	changebpm(-10);
	}
	if (keysDown() & KEY_LEFT) {
	changebpm(-1);
	}
	if (keysDown() & KEY_RIGHT) {
	changebpm(1);
	}
	if (keysDown() & KEY_L) {
	changetrack(-1);
	}
	if (keysDown() & KEY_R) {
	changetrack(1);
	}
	if (keysDown() & KEY_START)
	{
		if(playingtracks[activetracknumber] == 0)
		{
			playingtracks[activetracknumber] = 1;
		}
		else if(playingtracks[activetracknumber] == 1)
		{
			playingtracks[activetracknumber] = 0;
		}
	}

	
	if (keysDown() & KEY_TOUCH) {
		touchRead(&touch);
		
		int xaxispress = touch.px;
		int yaxispress = touch.py;
		
		int xval = (xaxispress / 16);
		int yval = (yaxispress / 16);

		if (yval < 8)
		{
			tracks[activetracknumber]->editpress(xval,yval);
		}
		
		xval = (xval / 2);
		yval = (yval / 2);
		
		if (yval == 4)
		{	
			if (xval < 7)
			{
				currentmode = edit;
				tracks[activetracknumber]->currenteditpattern = xval;
			}
			else if(xval == 7)
			{
				currentmode = seqpatterns;
				tracks[activetracknumber]->currenteditpattern = xval;
			}
		}
		if (yval == 5)
		{
			navbuttonpresses(xval);
		}	
	}
}

void homeviewbuttonpresses () {

	touchPosition touch;

	scanKeys();
	
	if (keysDown() & KEY_UP) {
	changebpm(10);
	}
	if (keysDown() & KEY_DOWN) {
	changebpm(-10);
	}
	if (keysDown() & KEY_LEFT) {
	changebpm(-1);
	}
	if (keysDown() & KEY_RIGHT) {
	changebpm(1);
	}
	if (keysDown() & KEY_L) {
	changetrack(-1);
	}
	if (keysDown() & KEY_R) {
	changetrack(1);
	}
	if (keysDown() & KEY_START) {
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


void topscreendisplay() {
	
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
	
	if ((globalstep % 4) == 0)
	{
		drawbiglongbuttonSUB(22,18,0);
	} else {
		drawbiglongbuttonSUB(22,18,1);
	}
	
	drawbpmtextSUB(2,18);
	
	displaybpm(bpm,12,18);

	drawstepdisplaySUB(globalstep, 11);
	
}



void setupview () {

	while(currentmode == setup)
    {
		swiWaitForVBlank();
		
		drawsetupscreen();
		
		setupviewbuttonpresses();
    }
	
	clearbottomscreen();
	
	setuprow = -1;
}

void homeview () {

	while(currentmode == home)
    {
		swiWaitForVBlank();
		
		drawhomescreen();
		
		homeviewbuttonpresses();
    }
	
	clearbottomscreen();
}

void followview () {

	while(currentmode == follow)
    {
		swiWaitForVBlank();
		
		drawfollowscreen();
		
		followviewbuttonpresses();
    }
	
	clearbottomscreen();
}

void patternseqview () {

	while(currentmode == seqpatterns)
    {
		swiWaitForVBlank();
		
		draweditscreen();
		
		patternseqbuttonpresses();
		
    }
	
	clearbottomscreen();
}

void editview () {

	while(currentmode == edit)
    {
		swiWaitForVBlank();
		
		draweditscreen();
		
		editviewbuttonpresses();
		
    }
	
	clearbottomscreen();
}



void optionsview () {

	while(currentmode == options)
    {
		swiWaitForVBlank();		
		
		drawoptionsscreen();
		
		optionsviewbuttonpresses();
		
	}
	
	clearbottomscreen();
	
}


// Sequencer Functions



void syncstarttracks() {

	int i;
	
	for (i = 0; i < 4; i++)
	{
		if (playingtracks[i] == 1)
		{
			tracks[i]->starttrack(1);
		}
	}
}

void movestepforward() {

	bpmcount++;
	
	if (bpmcount >= 6) {
		bpmcount = 0;

		if (globalplay == 1)
		{
			globalstep++;
			
			if (globalstep >= 16) {
				globalstep = 0;
			}
			
			if (globalstep == 0)
			{
				syncstarttracks();
			}
			
			
		}
		else if (globalplay == 0)
		{
			for (int i = 0; i < 4; i++)
			{
				playingtracks[i] = 0;
				tracks[i]->starttrack(0);
			}
			globalstep = -1;
		}
	}

}




void bpmtimer() {

	midisyncclock++;

	if (midisyncclock >= 60 * 16) {
		midisyncclock = 0;
		
		// MIDI clock function can go here
		
		for (int i = 0; i < 4; i++)
		{
			tracks[i]->sequencerclock();
		}
		for (int i = 0; i < 4; i++)
		{
			tracks[i]->sendmididata();
		}
		
		movestepforward();
		
	}
}


void setupbpmtimer() {

	TIMER_DATA(0) = TIMER_FREQ_64(bpm*24*16);
	TIMER_CR(0) = TIMER_DIV_64 | TIMER_ENABLE | TIMER_IRQ_REQ; 

	irqEnable  	(IRQ_TIMER0);
	irqSet (IRQ_TIMER0,bpmtimer);
	irqSet(IRQ_VBLANK, topscreendisplay);

}

// Main


int main(void) {


	swiWaitForVBlank();
	
    setupVideo();
	setupbpmtimer();

	PrintConsole bottomScreen;
	consoleInit(&bottomScreen, 0,BgType_Text4bpp, BgSize_T_256x256, 31, 3, true, true);
	consoleSelect(&bottomScreen);

	fatInitDefault();
	
	currentmode = home;

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
		}
	}

    return 0;
}
