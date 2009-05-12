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

midinotetrack::midinotetrack(int assignedtracknumber) {
	
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





// Sequencer Functions

void midinotetrack::starttrack(int playstatus) {

	if (playstatus == 1)
	{
		playing = 1;
		triggerplay = 1;
		
	} else {
		triggerplay = 0;
	}
	
}

void midinotetrack::resettrack(void) {

	playing = 0;
	triggerplay = 0;
	stepposition = 0;
	patternseqpos = 0;
	clockcount = 0;
	
}

void midinotetrack::sequencerclock(void) {

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
			triggernotes();
		}
		
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

void midinotetrack::sendmididata(void) {

	for (int i = 0; i < pendinglistpos; i++) {
	
		midinote(pendingsenddata[i][0],pendingsenddata[i][1],pendingsenddata[i][2]);
	
	}
	
	pendinglistpos = 0;

}





// Mode button functions

void midinotetrack::modebuttondisplay(void) {

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
	
	
void midinotetrack::modebuttonpress(int xval) {

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

void midinotetrack::editview(void) {
	
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
	
	
void midinotetrack::editpress(void) {
	
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
				patterns[currenteditpattern][xval][yval] = 2;
			}
			else if (patterns[currenteditpattern][xval][yval] == 2)
			{
				patterns[currenteditpattern][xval][yval] = 0;
			}
		}
		else if ((yval == 8) || (yval == 9))
		{	
			xval = xval / 2;
			
			currenteditpattern = xval;
		}
	}
}

	

// Pattern sequencer functions

void midinotetrack::patternseqview(void) {
	
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
void midinotetrack::patternseqpress(void) {

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

void midinotetrack::flowview(void) {
	
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

	
void midinotetrack::flowpress(void) {
	
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
				patterns[currenteditpattern][xval][yval] = 2;
			}
			else if (patterns[currenteditpattern][xval][yval] == 2)
			{
				patterns[currenteditpattern][xval][yval] = 0;
			}
		}
		else if ((yval == 8) || (yval == 9))
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

void midinotetrack::optionsview(void) {

	iprintf("\x1b[2;4HTrack");
	
	iprintf("\x1b[4;2Hsettings No.");
	iprintf("\x1b[5;2HPattern No.");
	
	iprintf("\x1b[7;2HStep Length");

	iprintf("\x1b[8;2HChannel");
	
	iprintf("\x1b[10;8HNote");
	iprintf("\x1b[10;13HVel");
	
	iprintf("\x1b[11;2HRow 1");
	iprintf("\x1b[12;2HRow 2");
	iprintf("\x1b[13;2HRow 3");
	iprintf("\x1b[14;2HRow 4");
	iprintf("\x1b[15;2HRow 5");
	iprintf("\x1b[16;2HRow 6");
	iprintf("\x1b[17;2HRow 7");
	iprintf("\x1b[18;2HRow 8");
	
	iprintf("\x1b[2;12H%i  ",tracknumber);
	
	iprintf("\x1b[4;14H%i  ",settingsnumber);
	iprintf("\x1b[5;14H%i  ",patternnumber);

	iprintf("\x1b[8;14H%i  ",midichannel);
	
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
	
	optionsscreenbackground(activerow, activecolumn);
	
	drawkeypad(24,2);
	
	int activevalue = 0;

	if (activerow == -1) {activevalue = 0;}

	if (activerow == 4) {activevalue = settingsnumber;}
	if (activerow == 5) {activevalue = patternnumber;}
	
	if (activerow == 8) {activevalue = midichannel;}

	if ((activerow > 10) && (activerow < 19) && (activecolumn > -1)) {activevalue = midinotes[activerow - 11][activecolumn];}

	calcanddispnumber(24,4,activevalue);
}
	
	
void midinotetrack::optionspress(void) {
	
	touchPosition touch;
	
	if (keysDown() & KEY_TOUCH){

		touchRead(&touch);
		
		int yval = (touch.py / 8);
		int xval = (touch.px / 8);

		if ((xval > 1) && (xval < 23) && (yval > 1) && (yval < 19))
		{
			if (yval == 4) {
			activerow = 4;
			}
			if (yval == 5) {
				activerow = 5;
			}
				
			if (yval == 7) {
				activerow = 7;
			}
			
			if (yval == 8) {
				activerow = 8;
			}
			
			if (yval == 11) {
				activerow = 11;
			}
			if (yval == 12) {
				activerow = 12;
			}
			if (yval == 13) {
				activerow = 13;
			}
			if (yval == 14) {
				activerow = 14;
			}
			if (yval == 15) {
				activerow = 15;
			}
			if (yval == 16) {
				activerow = 16;
			}
			if (yval == 17) {
				activerow = 17;
			}
			if (yval == 18) {
				activerow = 18;
			}
			if ((xval > 9) && (xval < 13)) {
				activecolumn = 0;
			}
			if ((xval > 14) && (xval < 18)) {
				activecolumn = 1;
			}
		} else  if ((xval > 23) && (xval < 30) && (yval > 1) && (yval < 9))
		{
			xval = (xval / 2);
			yval = (yval / 2);
				
			if (yval == 1)
			{
				if (xval == 12)
				{
					editmidioptions(100);
				}
				if (xval == 13)
				{
					editmidioptions(10);
				}
				if (xval == 14)
				{
					editmidioptions(1);
				}
			}
			if (yval == 3)
			{
				if (xval == 12)
				{
					editmidioptions(-100);
				}
				if (xval == 13)
				{
					editmidioptions(-10);
				}
				if (xval == 14)
				{
					editmidioptions(-1);
				}
			}
		}
	}
}



// Load and Save Functions

void midinotetrack::loadsaveview(void) {

	iprintf("\x1b[2;4HTrack");
	
	iprintf("\x1b[4;2Hsettings No.");
	iprintf("\x1b[5;2HPattern No.");
	

	
	iprintf("\x1b[2;12H%i  ",tracknumber);
	
	iprintf("\x1b[4;14H%i  ",settingsnumber);
	iprintf("\x1b[5;14H%i  ",patternnumber);
	
	optionsscreenbackground(activerow, activecolumn);
	
	drawbigbutton(3,7,1);
	drawbigbutton(3,13,1);
	drawbigbutton(12,7,1);
	drawbigbutton(12,13,1);
	
	drawkeypad(24,2);
	
	int activevalue = 0;

	if (activerow == -1) {activevalue = 0;}

	if (activerow == 4) {activevalue = settingsnumber;}
	if (activerow == 5) {activevalue = patternnumber;}

	calcanddispnumber(24,4,activevalue);

}

void midinotetrack::loadsavepress(void) {



}

// Private Object Functions


// Load and Save Sub Functions

void midinotetrack::patternfileloader() {

	patternbuffer patternloadstruct;
	
	char filePath[MAXPATHLEN * 2];
	int pathLen;
	std::string filename;
	FILE * pFile;
	
	filename = browseForFile (".ptr");

	int x, y, z;

	if (filename != "NULL")
	{
		// Construct a command line if we weren't supplied with one
		getcwd (filePath, MAXPATHLEN);
		pathLen = strlen (filePath);
		strcpy (filePath + pathLen, filename.c_str());
		
		pFile = fopen ( filePath , "r" );
		
		fread((char *)&patternloadstruct, sizeof(patternbuffer), 1, pFile);
		
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

void midinotetrack::patternfilesaver() {
	
	patternbuffer patternsavestruct;
	
	char format[] = "/seqgrid/files/pattern-%d.ptr";
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
	
	fwrite((char *)&patternsavestruct, sizeof(patternbuffer), 1, pFile);

	fclose (pFile);

}
void midinotetrack::settingsfileloader() {

	settingsbuffer settingsloadstruct;
	
	char filePath[MAXPATHLEN * 2];
	int pathLen;
	std::string filename;
	FILE * pFile;
	
	filename = browseForFile (".set");

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

		
		for (int i = 0; i < 8; i ++)
		{
			for (int j = 0; j < 2; j ++)
			{
				midinotes[i][j] = settingsloadstruct.midinotes[i][j];
			}
		}
		
		fclose (pFile);
	}
	
	clearbottomscreen();
	
}

void midinotetrack::settingsfilesaver() {
	
	settingsbuffer settingssavestruct;
	
	char format[] = "/seqgrid/files/settings-%d.prs";
	char filename[sizeof format+100];
	sprintf(filename,format,settingsnumber);
	FILE *pFile = fopen(filename,"w");

	settingssavestruct.settingsnumber = settingsnumber;
	settingssavestruct.midichannel = midichannel;
	
	for (int i = 0; i < 8; i ++)
	{
		for (int j = 0; j < 2; j ++)
		{
			settingssavestruct.midinotes[i][j] = midinotes[i][j];
		}
	}
	
	fwrite((char *)&settingssavestruct, sizeof(settingsbuffer), 1, pFile);

	fclose (pFile);

}






// Pattern sequencer functions

void midinotetrack::editmidioptions(int amount) {
	

	int tempvalue;
	
	if (activerow == 4) {
	
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
		
	} else if (activerow == 5) {
	
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
		
	} else if (activerow == 8) {
	
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
		
	} else if ((activerow > 10) && (activerow < 19)) {
	
		if (activecolumn == 0)
		{
			tempvalue = midinotes[activerow - 11][0] + amount;
		
			if (tempvalue > 127)
			{
				tempvalue = 127;
			}
			if (tempvalue < 0)
			{
				tempvalue = 0;
			}
			
			midinotes[activerow - 11][0] = tempvalue;
			
		} else if (activecolumn == 1)
		{
			tempvalue = midinotes[activerow - 11][1] + amount;
		
			if (tempvalue > 127)
			{
				tempvalue = 127;
			}
			if (tempvalue < 0)
			{
				tempvalue = 0;
			}
			
			midinotes[activerow - 11][1] = tempvalue;
			
		}
	}
}





// Note functions

void midinotetrack::triggernotes(void) {

	int j;
	int activetrackpattern = patternseq[patternseqpos];

	for (j = 0 ; j < 8 ; j++)
	{
		int stepvalue = patterns[activetrackpattern][stepposition][j];
		
		if ((stepvalue == 1) || (stepvalue == 2))
		{
			pendingsenddata[pendinglistpos][0] = midichannel;
			pendingsenddata[pendinglistpos][1] = midinotes[j][0];
			
			if (stepvalue == 1) {pendingsenddata[pendinglistpos][2] = midinotes[j][1];}
			if (stepvalue == 2) {pendingsenddata[pendinglistpos][2] = 0;}
			
			pendinglistpos++;
			
			if ((currentonnotes[j][0] != midichannel || currentonnotes[j][1] != midinotes[j][0]) && ( currentonnotes[j][2] == 1)) {
				
				pendingsenddata[pendinglistpos][0] = midichannel;
				pendingsenddata[pendinglistpos][1] = midinotes[j][0];
				pendingsenddata[pendinglistpos][2] = 0;
				
				pendinglistpos++;
				
			}
			
			currentonnotes[j][0] = midichannel;
			currentonnotes[j][1] = midinotes[j][0];
			
			if (stepvalue == 1) {currentonnotes[j][2] = 1;}
			if (stepvalue == 2) {currentonnotes[j][2] = 0;}
			
		}
	}

}


// End of Functions

