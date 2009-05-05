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
extern modes_t previousmode;

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
		patterns[7][x][0] = 1;
	}

	patterns[7][0][7] = 1;
	patterns[7][1][7] = 1;
	patterns[7][2][7] = 1;
	patterns[7][3][7] = 1;	
	
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
	if (clockcount == stepbeatlength) {
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
	
	
void midinotetrack::editpress(int xval, int yval) {
	
	yval = (yval / 16);
	xval = (xval / 16);
	
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

	

// Pattern sequencer functions

void midinotetrack::patternseqview(void) {
	
	int x,y;
	
	for ( x = 0; x < 16; x++ )
	{
		for( y = 0; y < 8; y++ )
		{
			drawgridbutton(x,y,(patterns[7][x][y]));
		}
	}
	
	navbuttons(2,4,currenteditpattern);

}
void midinotetrack::patternseqpress(int xval, int yval) {

	yval = (yval / 16);
	xval = (xval / 16);
	
	if (yval < 7)
	{
		for (int i = 0; i < 7; i++)
		{
			patterns[7][xval][i] = 0;
		}
		patterns[7][xval][yval] = 1;
		patternseq[xval] = yval;

	} else if (yval == 7)
	{
		for (int i = 0; i < 16; i++)
		{
			if (i <= xval)
			{
				patterns[7][i][7] = 1;
			}
			else if (i > xval)
			{
				patterns[7][i][7] = 0;
			}
		}
		patternseqlength = (xval + 1);
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

	
void midinotetrack::flowpress(int xval, int yval) {
	
	yval = (yval / 16);
	xval = (xval / 16);
	
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
	
	optionsscreenbackground(optionsrow, optionscolumn);
	
	drawkeypad(24,2);
	
	int activevalue = 0;

	if (optionsrow == -1) {activevalue = 0;}

	if (optionsrow == 4) {activevalue = settingsnumber;}
	if (optionsrow == 5) {activevalue = patternnumber;}
	
	if (optionsrow == 8) {activevalue = midichannel;}

	if ((optionsrow > 10) && (optionsrow < 19) && (optionscolumn > -1)) {activevalue = midinotes[optionsrow - 11][optionscolumn];}

	calcanddispnumber(24,4,activevalue);
}
	
	
void midinotetrack::optionspress(int xval, int yval) {
	
	yval = (yval / 8);
	xval = (xval / 8);

	if ((xval > 1) && (xval < 23) && (yval > 1) && (yval < 19))
	{
		if (yval == 4) {
		optionsrow = 4;
		}
		if (yval == 5) {
			optionsrow = 5;
		}
			
		if (yval == 7) {
			optionsrow = 7;
		}
		
		if (yval == 8) {
			optionsrow = 8;
		}
		
		if (yval == 11) {
			optionsrow = 11;
		}
		if (yval == 12) {
			optionsrow = 12;
		}
		if (yval == 13) {
			optionsrow = 13;
		}
		if (yval == 14) {
			optionsrow = 14;
		}
		if (yval == 15) {
			optionsrow = 15;
		}
		if (yval == 16) {
			optionsrow = 16;
		}
		if (yval == 17) {
			optionsrow = 17;
		}
		if (yval == 18) {
			optionsrow = 18;
		}
		if ((xval > 9) && (xval < 13)) {
			optionscolumn = 0;
		}
		if ((xval > 14) && (xval < 18)) {
			optionscolumn = 1;
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




// Load and Save Functions

void midinotetrack::fileload(void) {

	previousmode = currentmode;
	currentmode = loadsave;

	if ((previousmode == edit) || (previousmode == seqpatterns) || (previousmode == follow)) {
	
		patternfileloader();
	
	} else if (previousmode == options) {
	
		settingsfileloader();
	
	}
	
	currentmode = previousmode;

}

void midinotetrack::filesave(void) {

	previousmode = currentmode;
	currentmode = loadsave;

	if ((previousmode == edit) || (previousmode == seqpatterns) || (previousmode == follow)) {
	
		patternfilesaver();
	
	} else if (previousmode == options) {
	
		settingsfilesaver();
	
	}
	
	currentmode = previousmode;

}

// Private Object Functions


// Load and Save Sub Functions

void midinotetrack::patternfileloader() {

	patternbuffer patternloadstruct;
	
	filebrowsescreenbackground();

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
	
	filebrowsescreenbackground();

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
	
	if (optionsrow == 4) {
	
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
		
	} else if (optionsrow == 5) {
	
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
		
	} else if (optionsrow == 8) {
	
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
		
	} else if ((optionsrow > 10) && (optionsrow < 19)) {
	
		if (optionscolumn == 0)
		{
			tempvalue = midinotes[optionsrow - 11][0] + amount;
		
			if (tempvalue > 127)
			{
				tempvalue = 127;
			}
			if (tempvalue < 0)
			{
				tempvalue = 0;
			}
			
			midinotes[optionsrow - 11][0] = tempvalue;
			
		} else if (optionscolumn == 1)
		{
			tempvalue = midinotes[optionsrow - 11][1] + amount;
		
			if (tempvalue > 127)
			{
				tempvalue = 127;
			}
			if (tempvalue < 0)
			{
				tempvalue = 0;
			}
			
			midinotes[optionsrow - 11][1] = tempvalue;
			
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

