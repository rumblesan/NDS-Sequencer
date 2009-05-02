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


// Constructor

mididrumtrack::mididrumtrack(int assignedtracknumber) {
	
	int x,y,z;
	
	tracknumber = assignedtracknumber;
	
	patternseqpos = 0;
	currenteditpattern = 0;
	patternseqlength = 4;
	
	playing = 0;
	
	stepbeatlength = 4;
	clockcount = -1;
	
	stepposition = -1;

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



// Sequencer functions

void mididrumtrack::starttrack(int playstatus) {

	if (playstatus)
	{
		playing = 1;
	} else {
		playing = 0;
		clockcount = -1;
		patternseqpos = 0;
		stepposition = -1;
	}
}

void mididrumtrack::sequencerclock(void) {
	

	clockcount++;
	
	if (clockcount >= (60 * 24 * stepbeatlength))
	{
		if (playing)
		{
			
			clockcount = 0;
			stepposition++;
			
			triggernoteson();
			triggernotesoff();
			
			
			if (stepposition >= 16)
			{
				stepposition = 0;
				starttrack(1);
			}
		}
	}
}
	
	


// Edit view functions

void mididrumtrack::displaypattern(void) {
	
	int x,y;
	
	for ( x = 0; x < 16; x++ )
	{
		for( y = 0; y < 8; y++ )
		{
			drawgridbutton(x,y,(patterns[currenteditpattern][x][y]));
		}
	}
}
	
	
void mididrumtrack::editpress(int xval, int yval) {
	
	if (patterns[currenteditpattern][xval][yval] == 0)
	{
		patterns[currenteditpattern][xval][yval] = 1;
	}
	else if (patterns[currenteditpattern][xval][yval] == 1)
	{
		patterns[currenteditpattern][xval][yval] = 0;
	}
}

	

// Load and Save Functions

void mididrumtrack::fileload(modes_t currentmode) {

	if ((currentmode == edit) || (currentmode == seqpatterns) || (currentmode == follow)) {
	
		patternfileloader();
	
	} else if (currentmode == options) {
	
		settingsfileloader();
	
	}

}

void mididrumtrack::filesave(modes_t currentmode) {

	if ((currentmode == edit) || (currentmode == seqpatterns) || (currentmode == follow)) {
	
		patternfilesaver();
	
	} else if (currentmode == options) {
	
		settingsfilesaver();
	
	}

}

void mididrumtrack::patternfileloader() {

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

void mididrumtrack::patternfilesaver() {
	
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
void mididrumtrack::settingsfileloader() {

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
		notelength = settingsloadstruct.notelength;

		
		for (int i = 0; i < 8; i ++)
		{
			for (int j = 0; j < 8; j ++)
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
	
	char format[] = "/seqgrid/files/settings-%d.prs";
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






// Pattern sequencer functions

void mididrumtrack::displaypatternseq(void) {
	
	int x,y;
	
	for ( x = 0; x < 16; x++ )
	{
		for( y = 0; y < 8; y++ )
		{
			drawgridbutton(x,y,(patterns[7][x][y]));
		}
	}
}
void mididrumtrack::patternseqpress(int xval, int yval) {

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
}
	



// Midi options menu functions

void mididrumtrack::displayoptions() {

	iprintf("\x1b[2;4HTrack");
	
	iprintf("\x1b[4;2Hsettings No.");
	iprintf("\x1b[5;2HPattern No.");
	
	iprintf("\x1b[7;2HStep Length");

	iprintf("\x1b[8;2HChannel");
	
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
	
	iprintf("\x1b[4;14H%i  ",settingsnumber);
	iprintf("\x1b[5;14H%i  ",patternnumber);

	iprintf("\x1b[7;14H%i  ",stepbeatlength);

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
	
	iprintf("\x1b[11;19H%i ",midinotes[0][2]);
	iprintf("\x1b[12;19H%i ",midinotes[1][2]);
	iprintf("\x1b[13;19H%i ",midinotes[2][2]);
	iprintf("\x1b[14;19H%i ",midinotes[3][2]);
	iprintf("\x1b[15;19H%i ",midinotes[4][2]);
	iprintf("\x1b[16;19H%i ",midinotes[5][2]);
	iprintf("\x1b[17;19H%i ",midinotes[6][2]);
	iprintf("\x1b[18;19H%i ",midinotes[7][2]);
	
	optionsscreenbackground(activerow, activecolumn);
	
	drawkeypad(24,2);
	
	int activevalue = 0;

	if (activerow == -1) {activevalue = 0;}

	if (activerow == 4) {activevalue = settingsnumber;}
	if (activerow == 5) {activevalue = patternnumber;}

	if (activerow == 7) {activevalue = stepbeatlength;}
	
	if (activerow == 8) {activevalue = midichannel;}

	if ((activerow > 10) && (activerow < 19) && (activecolumn > -1)) {activevalue = midinotes[activerow - 11][activecolumn];}

	calcanddispnumber(24,4,activevalue);
}
	
	
void mididrumtrack::optionspress(int xaxispress, int yaxispress) {
	
	
	int xval = (xaxispress / 8);
	int yval = (yaxispress / 8);

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
		if ((xval > 19) && (xval < 23)) {
			activecolumn = 2;
		}
	} else  if ((xval > 23) && (xval < 30) && (yval > 1) && (yval < 9))
	{
		xval = (xaxispress / 16);
		yval = (yaxispress / 16);
			
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

void mididrumtrack::editmidioptions(int amount) {
	
	if (!playing)
	{
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
			
		}
		
		if (activerow == 5) {
		
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
		}
		
		if (activerow == 7) {
		
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
			
//			channelchangenotesoff();
		}
		
		if (activerow == 8) {
		
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
			
//			channelchangenotesoff();
		}
		
		
		if ((activerow > 10) && (activerow < 19)) {
		
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
				
//				midichangenotesoff((activerow - 10));
			}
				
			if (activecolumn == 1)
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
				
			if (activecolumn == 2)
			{
				tempvalue = midinotes[activerow - 11][2] + amount;
			
				if (tempvalue > 32)
				{
					tempvalue = 32;
				}
				if (tempvalue < 0)
				{
					tempvalue = 0;
				}
				
				midinotes[activerow - 11][2] = tempvalue;
			}
		}
	}
}





// Follow view functions

void mididrumtrack::displayactivepattern(void) {
	
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

	int j;

	for (j = 0 ; j < 8 ; j++)
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




// MIDI functions

void mididrumtrack::sendmididata(void) {

	for (int i = 0; i < pendinglistpos; i++) {
	
		midinote(pendingsenddata[i][0],pendingsenddata[i][1],pendingsenddata[i][2]);
	
	}
	
	pendinglistpos = 0;

}


