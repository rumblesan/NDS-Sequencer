#include <nds.h>
#include <stdio.h>
#include <fat.h>
#include <string.h>
#include <unistd.h>

#include "trackclass.h"
#include "gfx.h"
#include "midi.h"
#include "structs.h"

#include "file_browse.h"

extern modes_t currentmode;


// Constructor

mididrumtrack::mididrumtrack(int assignedtracknumber) {
	
	int x,y,z;
	
	tracknumber = assignedtracknumber;
	
	patternseqpos = 0;
	currenteditpattern = 0;
	patternseqlength = 4;
	
	playing = 0;
	triggerplay = 0;
	
	stepbeatlength = 4;
	clockcount = 0;
	stepposition = 0;

	settingsnumber = 0;
	patternnumber = 0;
	
	midichannel = 1;
	
	for ( z = 0; z < 8; z++ )
	{
		for( x = 0; x < 16; x++ )
		{
			for( y = 0; y < 8; y++ )
			{
				patterns[z][x][y] = 0;
			}
		}
		
		for( x = 0; x < 3; x++ )
		{
			currentonnotes[z][x] = -1;
			midinotes[z][x] = 0;
		}
	}
	
	for( x = 0; x < 16; x++ )
	{
		patternseq[x] = 0;
	}
	
}



// Sequencer functions

void mididrumtrack::starttrack(int playstatus) {

	if (playstatus == 1)
	{
		playing = 1;
		triggerplay = 1;
		
	} else {
		triggerplay = 0;
	}
	
}

void mididrumtrack::resettrack(void) {

	playing = 0;
	triggerplay = 0;
	stepposition = 0;
	patternseqpos = 0;
	clockcount = 0;
	
}

void mididrumtrack::sequencerclock(void) {
	

	if (clockcount == 0)
	{
		if (stepposition == 0)
		{
			if (triggerplay == 0)
			{
				resettrack();
			}
		}

		if (playing == 1)
		{
			triggernoteson();
		}
		
		triggernotesoff();
	}
	
	clockcount++;
	if (clockcount == stepbeatlength * 4) {
		clockcount = 0;
		
		if (playing == 1)
		{
			stepposition++;		
			if (stepposition == 16) {
				stepposition = 0;
				patternseqpos++;
				if (patternseqpos > patternseqlength) { patternseqpos = 0; }
			}
		}
	}
}



// MIDI functions

void mididrumtrack::sendmididata(void) {

	for (int i = 0; i < pendinglistpos; i++) {
	
		midinote(pendingsenddata[i][0],pendingsenddata[i][1],pendingsenddata[i][2]);
	
	}
	
	pendinglistpos = 0;

}


// Mode button functions

void mididrumtrack::modebuttondisplay(void) {

	int modebutton[6];

	for (int i = 0 ; i < 6 ; i++)
	{
		if (i == currentmode) {
			modebutton[i] = 1;
		}
		else
		{
			modebutton[i] = 0;
		}
	}

	drawbiglongbutton(0,20,modebutton[0]);
	drawmidbutton(8,20,modebutton[1]);
	drawmidbutton(12,20,modebutton[2]);
	drawmidbutton(16,20,modebutton[3]);
	drawmidbutton(20,20,modebutton[4]);
	drawbiglongbutton(24,20,modebutton[5]);
	
	iprintf("\x1b[21;2HHome");

	iprintf("\x1b[21;9HEd   ");
	iprintf("\x1b[22;9Hit   ");

	iprintf("\x1b[21;13HSe   ");
	iprintf("\x1b[22;13Hq.   ");

	iprintf("\x1b[21;17HFl   ");
	iprintf("\x1b[22;17How   ");
	
	iprintf("\x1b[21;21HSe   ");
	iprintf("\x1b[22;21Ht.   ");

	iprintf("\x1b[21;26HLoad");
	iprintf("\x1b[22;26HSave");

}
	
	
void mididrumtrack::modebuttonpress(int xval) {

	xval = (xval / 32);
	
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

	
// Edit view functions

void mididrumtrack::editview(void) {
	
	int x,y;
	
	for ( x = 0; x < 16; x++ )
	{
		for( y = 0; y < 8; y++ )
		{
			drawgridbutton(x,y,(patterns[currenteditpattern][x][y]));
		}
	}
	
	navbuttons(2,4,currenteditpattern);
}
	
	
void mididrumtrack::editpress(void) {

	touchPosition touch;
	
	if (keysDown() & KEY_TOUCH){

		touchRead(&touch);
		
		int yval = (touch.py / 16);
		int xval = (touch.px / 16);
		
		if (yval < 8)
		{
			if (patterns[currenteditpattern][xval][yval] == 0)
			{
				patterns[currenteditpattern][xval][yval] = 1;
			}
			else if (patterns[currenteditpattern][xval][yval] == 1)
			{
				patterns[currenteditpattern][xval][yval] = 0;
			}
		} else if ((yval == 8) || (yval == 9))
		{	
			xval = xval / 2;
			
			currenteditpattern = xval;
		}
	}
}

	

// Pattern sequencer functions

void mididrumtrack::patternseqview(void) {
	
	int x,y;
	
	for ( x = 0; x < 16; x++ )
	{
		for( y = 0; y < 8; y++ )
		{
			if (patternseq[x] == y) {
				drawgridbutton(x,y,1);
			} else {
				drawgridbutton(x,y,0);
			}
		}
	}
	
	for (int x = 0; x < 16; x++)
	{
		if (x < patternseqlength)
		{
			drawgridbutton(x,9,1);
		}
		else if (x >= patternseqlength)
		{
			drawgridbutton(x,9,0);
		}
	}

}
void mididrumtrack::patternseqpress(void) {

	touchPosition touch;
	
	if (keysDown() & KEY_TOUCH){

		touchRead(&touch);
		
		int yval = (touch.py / 16);
		int xval = (touch.px / 16);
	
		if (yval < 8)
		{
			patternseq[xval] = yval;

		} else if (yval == 9)
		{
			patternseqlength = (xval + 1);
		}
	}
}
	


// Follow view functions

void mididrumtrack::flowview(void) {
	
	int x,y;
	
	int activepattern = patternseq[patternseqpos];
	currenteditpattern = activepattern;
	
	for ( x = 0; x < 16; x++ )
	{
		for( y = 0; y < 8; y++ )
		{
			if (x == stepposition)
			{
				drawgridbutton(x,y,((patterns[activepattern][x][y]) + 2));
			} else
			{
				drawgridbutton(x,y,(patterns[activepattern][x][y]));
			}
		}	
	}
	
	navbuttons(2,4,currenteditpattern);

}

	
void mididrumtrack::flowpress(void) {
	
	touchPosition touch;
	
	if (keysDown() & KEY_TOUCH){

		touchRead(&touch);
		
		int yval = (touch.py / 16);
		int xval = (touch.px / 16);
	
		if (yval < 8)
		{
			if (patterns[currenteditpattern][xval][yval] == 0)
			{
				patterns[currenteditpattern][xval][yval] = 1;
			}
			else if (patterns[currenteditpattern][xval][yval] == 1)
			{
				patterns[currenteditpattern][xval][yval] = 0;
			}
		} else if ((yval == 8) || (yval == 9))
		{	
			xval = xval / 2;
			
			currenteditpattern = xval;
			
			if (xval < 7)
			{
				currentmode = edit;
			}
			else if(xval == 7)
			{
				currentmode = seqpatterns;
			}
		}
	}
}



// Options view functions

void mididrumtrack::optionsview(void) {

	iprintf("\x1b[2;4HTrack");
	
	iprintf("\x1b[5;2HStep Length");

	iprintf("\x1b[7;2HChannel");
	
	iprintf("\x1b[10;8HNote");
	iprintf("\x1b[10;13HVel");
	iprintf("\x1b[10;18HLen");
	
	iprintf("\x1b[11;2HRow 1");
	iprintf("\x1b[12;2HRow 2");
	iprintf("\x1b[13;2HRow 3");
	iprintf("\x1b[14;2HRow 4");
	iprintf("\x1b[15;2HRow 5");
	iprintf("\x1b[16;2HRow 6");
	iprintf("\x1b[17;2HRow 7");
	iprintf("\x1b[18;2HRow 8");
	
	iprintf("\x1b[2;12H%i  ",tracknumber);

	iprintf("\x1b[5;14H%i  ",stepbeatlength);

	iprintf("\x1b[7;14H%i  ",midichannel);
	
	iprintf("\x1b[11;9H%i  ",midinotes[0][0]);
	iprintf("\x1b[12;9H%i  ",midinotes[1][0]);
	iprintf("\x1b[13;9H%i  ",midinotes[2][0]);
	iprintf("\x1b[14;9H%i  ",midinotes[3][0]);
	iprintf("\x1b[15;9H%i  ",midinotes[4][0]);
	iprintf("\x1b[16;9H%i  ",midinotes[5][0]);
	iprintf("\x1b[17;9H%i  ",midinotes[6][0]);
	iprintf("\x1b[18;9H%i  ",midinotes[7][0]);
	
	iprintf("\x1b[11;14H%i  ",midinotes[0][1]);
	iprintf("\x1b[12;14H%i  ",midinotes[1][1]);
	iprintf("\x1b[13;14H%i  ",midinotes[2][1]);
	iprintf("\x1b[14;14H%i  ",midinotes[3][1]);
	iprintf("\x1b[15;14H%i  ",midinotes[4][1]);
	iprintf("\x1b[16;14H%i  ",midinotes[5][1]);
	iprintf("\x1b[17;14H%i  ",midinotes[6][1]);
	iprintf("\x1b[18;14H%i  ",midinotes[7][1]);
	
	iprintf("\x1b[11;19H%i ",midinotes[0][2]);
	iprintf("\x1b[12;19H%i ",midinotes[1][2]);
	iprintf("\x1b[13;19H%i ",midinotes[2][2]);
	iprintf("\x1b[14;19H%i ",midinotes[3][2]);
	iprintf("\x1b[15;19H%i ",midinotes[4][2]);
	iprintf("\x1b[16;19H%i ",midinotes[5][2]);
	iprintf("\x1b[17;19H%i ",midinotes[6][2]);
	iprintf("\x1b[18;19H%i ",midinotes[7][2]);
	
	drawkeypad(24,2);
	
	int activevalue = 0;
	int xval = -1;
	int yval = -1;
	int length = 0;

	if (activerow == 1)
	{
		activevalue = stepbeatlength;
		
		xval = 14;
		yval = 5;
		length = 3;
	}
	else if (activerow == 2)
	{
		activevalue = midichannel;
		
		xval = 14;
		yval = 7;
		length = 3;
	}
	else if ((activerow > 2) && (activerow < 11) && (activecolumn > -1))
	{
		activevalue = midinotes[activerow - 3][activecolumn];
		
		xval = (9 + (activecolumn * 5));
		yval = (11 + activerow - 3);
		length = (3 - (activecolumn / 2));
	}

	calcanddispnumber(24,4,activevalue);
	
	optionsscreenbackground(xval, yval, length);
}
	
	
void mididrumtrack::optionspress(void) {
	
	touchPosition touch;
	
	if (keysDown() & KEY_TOUCH){

		touchRead(&touch);
		
		int yval = (touch.py / 8);
		int xval = (touch.px / 8);

		if ((xval > 1) && (xval < 23) && (yval > 1) && (yval < 21))
		{
			
			if (yval == 5) {
				activerow = 1;
			}
			else if (yval == 7) {
				activerow = 2;
			}
			else if (yval == 11) {
				activerow = 3;
			}
			else if (yval == 12) {
				activerow = 4;
			}
			else if (yval == 13) {
				activerow = 5;
			}
			else if (yval == 14) {
				activerow = 6;
			}
			else if (yval == 15) {
				activerow = 7;
			}
			else if (yval == 16) {
				activerow = 8;
			}
			else if (yval == 17) {
				activerow = 9;
			}
			else if (yval == 18) {
				activerow = 10;
			}
			if ((xval > 8) && (xval < 12)) {
				activecolumn = 0;
			}
			if ((xval > 13) && (xval < 17)) {
				activecolumn = 1;
			}
			else if ((xval > 18) && (xval < 21)) {
				activecolumn = 2;
			}
			
			
		} else  if ((xval > 23) && (xval < 30) && (yval > 1) && (yval < 9))
		{
			xval = (xval / 2);
			yval = (yval / 2);
				
			if (yval == 1)
			{
				if (xval == 12)
				{
					editoption(100);
				}
				else if (xval == 13)
				{
					editoption(10);
				}
				else if (xval == 14)
				{
					editoption(1);
				}
			}
			else if (yval == 3)
			{
				if (xval == 12)
				{
					editoption(-100);
				}
				else if (xval == 13)
				{
					editoption(-10);
				}
				else if (xval == 14)
				{
					editoption(-1);
				}
			}
		}
	}
}




// Load and Save Functions

void mididrumtrack::loadsaveview(void) {
	
	int activevalue = 0;
	int xval = -1;
	int yval = -1;
	int length = 0;

	if (activerow == 11)
	{
		activevalue = settingsnumber;
		
		xval = 7;
		yval = 5;
		length = 3;
	}
	else if (activerow == 12)
	{
		activevalue = patternnumber;
		
		xval = 16;
		yval = 5;
		length = 3;
	}

	calcanddispnumber(24,4,activevalue);
	
	optionsscreenbackground(xval, yval, length);
	
	iprintf("\x1b[2;4HTrack");
	
	iprintf("\x1b[4;3HSettings");
	iprintf("\x1b[4;12HPatterns");
	
	iprintf("\x1b[2;12H%i  ",tracknumber);
	
	iprintf("\x1b[5;3HNo. %i  ",settingsnumber);
	iprintf("\x1b[5;12HNo. %i  ",patternnumber);
	
	iprintf("\x1b[8;5HLoad");
	iprintf("\x1b[14;5HSave");
	
	iprintf("\x1b[8;14HLoad");
	iprintf("\x1b[14;14HSave");
	
	drawbiglongbutton(3,7,1);
	drawbiglongbutton(3,13,1);
	drawbiglongbutton(12,7,1);
	drawbiglongbutton(12,13,1);
	
	drawkeypad(24,2);

}

void mididrumtrack::loadsavepress(void) {

	touchPosition touch;
	
	if (keysDown() & KEY_TOUCH){

		touchRead(&touch);
		
		int yval = (touch.py / 8);
		int xval = (touch.px / 8);

		if ((yval == 5) && (xval < 23))
		{
			if ((xval > 6) && (xval < 10))
			{
				activerow = 11;
			}
			else if ((xval > 15) && (xval < 19))
			{
				activerow = 12;
			}
	
		}
		else if ((xval > 2) && (xval < 11) && (yval > 6) && (yval < 11))
		{
			currentmode = filebrowse;
			settingsfileloader();
			currentmode = loadsave;
		}
		else if ((xval > 2) && (xval < 11) && (yval > 12) && (yval < 17))
		{
			settingsfilesaver();
		}
		else if ((xval > 11) && (xval < 20) && (yval > 6) && (yval < 11))
		{
			currentmode = filebrowse;
			patternfileloader();
			currentmode = loadsave;
		}
		else if ((xval > 11) && (xval < 20) && (yval > 12) && (yval < 17))
		{
			patternfilesaver();
		}
		else if ((xval > 23) && (xval < 30) && (yval > 1) && (yval < 9))
		{
			xval = (xval / 2);
			yval = (yval / 2);
				
			if (yval == 1)
			{
				if (xval == 12)
				{
					editoption(100);
				}
				else if (xval == 13)
				{
					editoption(10);
				}
				else if (xval == 14)
				{
					editoption(1);
				}
			}
			else if (yval == 3)
			{
				if (xval == 12)
				{
					editoption(-100);
				}
				else if (xval == 13)
				{
					editoption(-10);
				}
				else if (xval == 14)
				{
					editoption(-1);
				}
			}
		}
	}
}



// Private Object Functions


// Load and Save Sub Functions

void mididrumtrack::patternfileloader() {

	gridpatternbuffer patternloadstruct;
	
	char filePath[MAXPATHLEN * 2];
	int pathLen;
	std::string filename;
	FILE * pFile;
	
	filename = browseForFile (".ptn");

	int x, y, z;

	if (filename != "NULL")
	{
		// Construct a command line if we weren't supplied with one
		getcwd (filePath, MAXPATHLEN);
		pathLen = strlen (filePath);
		strcpy (filePath + pathLen, filename.c_str());
		
		pFile = fopen ( filePath , "r" );
		
		fread((char *)&patternloadstruct, sizeof(gridpatternbuffer), 1, pFile);
		
		patternnumber = patternloadstruct.patternnumber;
		
		patternseqlength = patternloadstruct.patternseqlength;
		stepbeatlength = patternloadstruct.stepbeatlength;
		
		for ( z = 0; z < 8; z++ )
		{
			for( x = 0; x < 16; x++ )
			{
				for( y = 0; y < 8; y++ )
				{
					patterns[z][x][y] = patternloadstruct.patterns[z][x][y];
					if (patterns[z][x][y] == 2) {patterns[z][x][y] = 0;}
				}
			}
		}
		
		for( x = 0; x < 16; x++ )
		{
			patternseq[x] = patternloadstruct.patternseq[x];
		}
		
		
		fclose (pFile);
	}
	
    clearbottomscreen();
	
}

void mididrumtrack::patternfilesaver() {
	
	gridpatternbuffer patternsavestruct;
	
	char format[] = "/seqgrid/files/pattern-%d.ptn";
	char filename[sizeof format+100];
	sprintf(filename,format,patternnumber);
	FILE *pFile = fopen(filename,"w");

		patternsavestruct.patternnumber = patternnumber;
		
		patternsavestruct.patternseqlength = patternseqlength;
		patternsavestruct.stepbeatlength = stepbeatlength;
		
		int x, y, z;
		
		for ( z = 0; z < 8; z++ )
		{
			for( x = 0; x < 16; x++ )
			{
				for( y = 0; y < 8; y++ )
				{
					patternsavestruct.patterns[z][x][y] = patterns[z][x][y];
				}
			}
		}
		
		for( x = 0; x < 16; x++ )
		{
			patternsavestruct.patternseq[x] = patternseq[x];
		}
	
	fwrite((char *)&patternsavestruct, sizeof(gridpatternbuffer), 1, pFile);

	fclose (pFile);

}
void mididrumtrack::settingsfileloader() {

	settingsbuffer settingsloadstruct;
	
	char filePath[MAXPATHLEN * 2];
	int pathLen;
	std::string filename;
	FILE * pFile;
	
	filename = browseForFile (".dst");

	if (filename != "NULL")
	{
		// Construct a command line if we weren't supplied with one
		getcwd (filePath, MAXPATHLEN);
		pathLen = strlen (filePath);
		strcpy (filePath + pathLen, filename.c_str());
		
		pFile = fopen ( filePath , "r" );
		
		fread((char *)&settingsloadstruct, sizeof(settingsbuffer), 1, pFile);
		
		settingsnumber = settingsloadstruct.settingsnumber;
		midichannel = settingsloadstruct.midichannel;
		notelength = settingsloadstruct.notelength;

		
		for (int i = 0; i < 8; i ++)
		{
			for (int j = 0; j < 3; j ++)
			{
				midinotes[i][j] = settingsloadstruct.midinotes[i][j];
			}
		}
		
		fclose (pFile);
	}
	
	clearbottomscreen();
	
}

void mididrumtrack::settingsfilesaver() {
	
	settingsbuffer settingssavestruct;
	
	char format[] = "/seqgrid/files/settings-%d.dst";
	char filename[sizeof format+100];
	sprintf(filename,format,settingsnumber);
	FILE *pFile = fopen(filename,"w");

	settingssavestruct.settingsnumber = settingsnumber;
	settingssavestruct.midichannel = midichannel;
	settingssavestruct.notelength = notelength;
	
	for (int i = 0; i < 8; i ++)
	{
		for (int j = 0; j < 3; j ++)
		{
			settingssavestruct.midinotes[i][j] = midinotes[i][j];
		}
	}
	
	fwrite((char *)&settingssavestruct, sizeof(settingsbuffer), 1, pFile);

	fclose (pFile);

}







// Midi options menu functions

void mididrumtrack::editoption(int amount) {
	

	int tempvalue;
	
	if (activerow == 11) {
	
		tempvalue = settingsnumber + amount;
		
		if (tempvalue > 200)
		{
			tempvalue = 200;
		}
		if (tempvalue < 0)
		{
			tempvalue = 0;
		}
		
		settingsnumber = tempvalue;
		
	} else if (activerow == 12) {
	
		tempvalue = patternnumber + amount;
		
		if (tempvalue > 200)
		{
			tempvalue = 200;
		}
		if (tempvalue < 0)
		{
			tempvalue = 0;
		}
		
		patternnumber = tempvalue;
		
	} else if (activerow == 1) {
	
		tempvalue = stepbeatlength + amount;
		
		if (tempvalue > 16)
		{
			tempvalue = 16;
		}
		if (tempvalue < 1)
		{
			tempvalue = 1;
		}
		
		stepbeatlength = tempvalue;
		
	} else if (activerow == 2) {
	
		tempvalue = midichannel + amount;
		
		if (tempvalue > 15)
		{
			tempvalue = 15;
		}
		if (tempvalue < 0)
		{
			tempvalue = 0;
		}
		
		midichannel = tempvalue;
		
	} else if ((activerow > 2) && (activerow < 11)) {
	
		if (activecolumn == 0)
		{
			tempvalue = midinotes[activerow - 3][0] + amount;
		
			if (tempvalue > 127)
			{
				tempvalue = 127;
			}
			if (tempvalue < 0)
			{
				tempvalue = 0;
			}
			
			midinotes[activerow - 3][0] = tempvalue;
			
		} else if (activecolumn == 1)
		{
			tempvalue = midinotes[activerow - 3][1] + amount;
		
			if (tempvalue > 127)
			{
				tempvalue = 127;
			}
			if (tempvalue < 0)
			{
				tempvalue = 0;
			}
			
			midinotes[activerow - 3][1] = tempvalue;
			
		} else if (activecolumn == 2)
		{
			tempvalue = midinotes[activerow - 3][2] + amount;
		
			if (tempvalue > 32)
			{
				tempvalue = 32;
			}
			if (tempvalue < 0)
			{
				tempvalue = 0;
			}
			
			midinotes[activerow - 3][2] = tempvalue;
		}
	}
}





// Note functions

void mididrumtrack::triggernoteson(void) {

	int j;
	int activetrackpattern = patternseq[patternseqpos];

	for (j = 0 ; j < 8 ; j++)
	{
		if (patterns[activetrackpattern][stepposition][j] == 1)
		{				
			pendingsenddata[pendinglistpos][0] = midichannel;
			pendingsenddata[pendinglistpos][1] = midinotes[j][0];
			pendingsenddata[pendinglistpos][2] = midinotes[j][1];
			
			pendinglistpos++;
			
			if (currentonnotes[j][0] != midichannel || currentonnotes[j][1] != midinotes[j][0]) {
				
				pendingsenddata[pendinglistpos][0] = midichannel;
				pendingsenddata[pendinglistpos][1] = midinotes[j][0];
				pendingsenddata[pendinglistpos][2] = 0;
				
				pendinglistpos++;
				
			}
			currentonnotes[j][0] = midichannel;
			currentonnotes[j][1] = midinotes[j][0];
			currentonnotes[j][2] = midinotes[j][2];
			
		}
	}

}

void mididrumtrack::triggernotesoff(void) {

	for (int j = 0 ; j < 8 ; j++)
	{
		if (currentonnotes[j][2] == 0)
		{
			pendingsenddata[pendinglistpos][0] = currentonnotes[j][0];
			pendingsenddata[pendinglistpos][1] = currentonnotes[j][1];
			pendingsenddata[pendinglistpos][2] = 0;
			
			pendinglistpos++;
			
			currentonnotes[j][2] = -1;
		}
		
		if (currentonnotes[j][2] > 0)
		{
			currentonnotes[j][2] = (currentonnotes[j][2] - 1);
		}
	}

}


// End of Functions
