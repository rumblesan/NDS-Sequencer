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

midicctrack::midicctrack(int assignedtracknumber) {

	
	int x,y;
	
	playing = 0;
	triggerplay = 0;
	
	tracknumber = assignedtracknumber;
	
	settingsnumber = 0;
	patternnumber = 0;
	
	currenteditpattern = 0;

	prevx = 0;
	prevy = 0;

	for( x = 0; x < 8; x++ )
	{
		for( y = 0; y < 256; y++ )
		{
			patterns[x][y] = 0;
		}
		patternlengths[x] = 1;
		
		patternpositions[0][x] = 0;
		patternpositions[1][x] = 0;
		
		midiccnumbers[x] = 0;
		
	}
	
	for( x = 0; x < 8; x++ )
	{
		for (y = 0; y < 16; y++)
		{
			patternseq[x][y] = 0;
		}
		patternseq[x][0] = 1;
	}
	
	activerow = -1;
	
	
	for( x = 0; x < 16; x++ )
	{
		for( y = 0; y < 3; y++ )
		{
			pendingsenddata[x][y] = 0;
		}
	}
	

	clockcount = 0;
	midichannel = 0;
	pendinglistpos = 0;
	
}





// Sequencer Functions

void midicctrack::starttrack(int playstatus) {

	if (playstatus == 1)
	{
		playing = 1;
		triggerplay = 1;
		
	} else {
		triggerplay = 0;
	}
	
}

void midicctrack::resettrack(void) {

	int x;
	
	playing = 0;
	triggerplay = 0;
	for(x = 0; x < 8; x++ )
	{
		patternpositions[0][x] = 0;
		patternpositions[1][x] = 0;
	}
	clockcount = 0;
	
}

void midicctrack::sequencerclock(void) {
	
	int x;

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
	if (clockcount == 4) {
		clockcount = 0;
		
		if (playing == 1)
		{
			stepposition++;
			if (stepposition == 16 * 4) {
				stepposition = 0;
			}
			
			for (x = 0; x < 8; x++) {
				
				if (activepatterns[x] == 1) {
					patternpositions[0][x]++;
					
					if (patternpositions[0][x] == patternlengths[x]) {
						patternpositions[0][x] = 0;
						
						patternpositions[1][x]++;
						
						if (patternpositions[1][x] == 256) {
							patternpositions[1][x] = 0;
						}
					}
				} else {
					patternpositions[0][x] = 0;
					patternpositions[1][x] = 0;
				}
			}
		}
	}
}
	
	


// Midi functions

void midicctrack::sendmididata(void) {

	for (int i = 0; i < pendinglistpos; i++) {
	
		midicc(pendingsenddata[i][0],pendingsenddata[i][1],pendingsenddata[i][2]);
	
	}
	
	pendinglistpos = 0;

}


// Edit view functions

void midicctrack::editview(void) {
	
	int colour = 100;
	
	for (int x = 0; x < 256; x++) {
	
		drawpoint(x, patterns[currenteditpattern][x], colour);
	
	}
	
	navbuttons(2,4,currenteditpattern);
}
	
	
void midicctrack::editpress(int xval, int yval) {

	if (yval < 128)
	{
		if (keysDown() & KEY_TOUCH) {
		
			prevx = xval;
			prevy = yval;
			
			patterns[currenteditpattern][xval] = (127 - yval);
		
		} else if (keysHeld() & KEY_TOUCH) {
		
			if ((xval != prevx) && (yval != prevy)) {
			
				linealg(prevx, prevy, xval, yval);
				prevx = xval;
				prevy = yval;			
			}
		
		}
	} else if ((xval > 127) && (xval < 160)) {
		
		xval = xval / 32;
		
		currenteditpattern = xval;
		
	}
}

	

// Pattern sequencer functions

void midicctrack::patternseqview(void) {
	
	for(int x = 0; x < 8; x++) {
		for(int y = 0; y < 16; y++) {
			drawgridbutton(x,y,patternseq[x][y]);
		}
	}
	
	navbuttons(2,4,currenteditpattern);

}
void midicctrack::patternseqpress(int xval, int yval) {

	yval = (yval / 16);
	xval = (xval / 16);
	
	if (yval < 8)
	{
		for (int i = 0; i < 16; i++)
		{
			if (i <= xval)
			{
				patternseq[i][yval] = 1;
			}
			else if (i > xval)
			{
				patternseq[i][yval] = 0;
			}
		}
		patternlengths[yval] = (xval + 1);
	} 
	else if ((yval == 8) || (yval == 9))
	{	
		xval = xval / 2;
		
		currenteditpattern = xval;
		
	}

}
	


// Follow view functions

void midicctrack::flowview(void) {
	
	
	navbuttons(2,4,currenteditpattern);

}

	
void midicctrack::flowpress(int xval, int yval) {
	

}



// Options view functions

void midicctrack::optionsview(void) {

	iprintf("\x1b[2;4HTrack");
	
	iprintf("\x1b[4;2Hsettings No.");
	iprintf("\x1b[5;2HPattern No.");
	
	iprintf("\x1b[7;2HStep Length");

	iprintf("\x1b[8;2HChannel");
	
	iprintf("\x1b[10;8HCC Numbers");
	
	iprintf("\x1b[11;2HPattern 1");
	iprintf("\x1b[12;2HPattern 2");
	iprintf("\x1b[13;2HPattern 3");
	iprintf("\x1b[14;2HPattern 4");
	iprintf("\x1b[15;2HPattern 5");
	iprintf("\x1b[16;2HPattern 6");
	iprintf("\x1b[17;2HPattern 7");
	iprintf("\x1b[18;2HPattern 8");
	
	iprintf("\x1b[2;12H%i  ",tracknumber);
	
	iprintf("\x1b[4;14H%i  ",settingsnumber);
	iprintf("\x1b[5;14H%i  ",patternnumber);

	iprintf("\x1b[8;14H%i  ",midichannel);
	
	iprintf("\x1b[11;9H%i  ",midiccnumbers[0]);
	iprintf("\x1b[12;9H%i  ",midiccnumbers[1]);
	iprintf("\x1b[13;9H%i  ",midiccnumbers[2]);
	iprintf("\x1b[14;9H%i  ",midiccnumbers[3]);
	iprintf("\x1b[15;9H%i  ",midiccnumbers[4]);
	iprintf("\x1b[16;9H%i  ",midiccnumbers[5]);
	iprintf("\x1b[17;9H%i  ",midiccnumbers[6]);
	iprintf("\x1b[18;9H%i  ",midiccnumbers[7]);
	
	optionsscreenbackground(activerow, 0);
	
	drawkeypad(24,2);
	
	int activevalue = 0;

	if (activerow == -1) {activevalue = 0;}

	if (activerow == 4) {activevalue = settingsnumber;}
	if (activerow == 5) {activevalue = patternnumber;}
	
	if (activerow == 8) {activevalue = midichannel;}

	if ((activerow > 10) && (activerow < 19)) {activevalue = midiccnumbers[activerow - 11];}

	calcanddispnumber(24,4,activevalue);
}
	
	
void midicctrack::optionspress(int xval, int yval) {
	
	yval = (yval / 8);
	xval = (xval / 8);

	if ((xval > 1) && (xval < 23) && (yval > 1) && (yval < 19))
	{
		if (yval == 4) {
			activerow = 4;
		}
		else if (yval == 5)
		{
			activerow = 5;
		}
		else if (yval == 8)
		{
			activerow = 8;
		}
		else if (yval == 11)
		{
			activerow = 11;
		}
		else if (yval == 12)
		{
			activerow = 12;
		}
		else if (yval == 13)
		{
			activerow = 13;
		}
		else if (yval == 14)
		{
			activerow = 14;
		}
		else if (yval == 15)
		{
			activerow = 15;
		}
		else if (yval == 16)
		{
			activerow = 16;
		}
		else if (yval == 17)
		{
			activerow = 17;
		}
		else if (yval == 18)
		{
			activerow = 18;
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
			else if (xval == 13)
			{
				editmidioptions(10);
			}
			else if (xval == 14)
			{
				editmidioptions(1);
			}
		}
		else if (yval == 3)
		{
			if (xval == 12)
			{
				editmidioptions(-100);
			}
			else if (xval == 13)
			{
				editmidioptions(-10);
			}
			else if (xval == 14)
			{
				editmidioptions(-1);
			}
		}
	}
}




// Load and Save Functions

void midicctrack::fileload(void) {
/*
	previousmode = currentmode;
	currentmode = loadsave;

	if ((previousmode == edit) || (previousmode == seqpatterns) || (previousmode == follow)) {
	
		patternfileloader();
	
	} else if (previousmode == options) {
	
		settingsfileloader();
	
	}
	
	currentmode = previousmode;
*/
}

void midicctrack::filesave(void) {
/*
	previousmode = currentmode;
	currentmode = loadsave;

	if ((previousmode == edit) || (previousmode == seqpatterns) || (previousmode == follow)) {
	
		patternfilesaver();
	
	} else if (previousmode == options) {
	
		settingsfilesaver();
	
	}
	
	currentmode = previousmode;
*/
}

// Private Object Functions


// Load and Save Sub Functions

void midicctrack::patternfileloader() {
/*
	patternbuffer patternloadstruct;
	
	char filePath[MAXPATHLEN * 2];
	int pathLen;
	std::string filename;
	FILE * pFile;
	
	iprintf("loading\n");
	
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
*/
}

void midicctrack::patternfilesaver() {
/*	
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
*/
}

void midicctrack::settingsfileloader() {
/*
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
*/
}

void midicctrack::settingsfilesaver() {
/*
	settingsbuffer settingssavestruct;
	
	char format[] = "/seqgrid/files/settings-%d.set";
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
*/
}







// Line Calculation Algorithm

int midicctrack::interpolationalg(int pointone, int pointtwo, int stepdenominator, int steplength) {

	if (stepdenominator == 0) {
		return pointone;
	}

	int dif = pointtwo - pointone;
	
	int division = stepdenominator / steplength;
	
	int interpolatedval = division * dif;
	
	return interpolatedval;

}


// Screen press line drawing alg

void midicctrack::linealg(int x0, int y0, int x1, int y1) {

   int Dx = x1 - x0; 
   int Dy = y1 - y0;
   int steep = (abs(Dy) >= abs(Dx));
   
   if (steep) {
		int temp = y0;
		y0 = x0;
		x0 = temp;
		
		temp = y1;
		y1 = x1;
		x1 = temp;

       // recompute Dx, Dy after swap
       Dx = x1 - x0;
       Dy = y1 - y0;
   }
   
   int xstep = 1;
   if (Dx < 0) {
       xstep = -1;
       Dx = -Dx;
   }
   
   int ystep = 1;
   if (Dy < 0) {
       ystep = -1;		
       Dy = -Dy; 
   }
   
   int TwoDy = 2*Dy; 
   int TwoDyTwoDx = TwoDy - 2*Dx; // 2*Dy - 2*Dx
   int E = TwoDy - Dx; //2*Dy - Dx
   int y = y0;
   int xDraw, yDraw;	
   for (int x = x0; x != x1; x += xstep) {	
	
       if (steep) {			
           xDraw = y;
           yDraw = x;
       } else {			
           xDraw = x;
           yDraw = y;
       }
       // plot
	   
	   	patterns[currenteditpattern][xDraw] = yDraw;
       
	   // next
       if (E > 0) {
           E += TwoDyTwoDx; //E += 2*Dy - 2*Dx;
           y = y + ystep;
       } else {
           E += TwoDy; //E += 2*Dy;
       }
   }
}




// Midi options menu functions

void midicctrack::editmidioptions(int amount) {
	

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
	
		tempvalue = midiccnumbers[activerow - 11] + amount;
		
		if (tempvalue > 127)
		{
			tempvalue = 127;
		}
		if (tempvalue < 0)
		{
			tempvalue = 0;
		}
		
		midiccnumbers[activerow - 11] = tempvalue;
		
	}
}





// Note functions

void midicctrack::triggernotes(void) {

	for (int x = 0; x < 8; x++) {
		
		if (activepatterns[x] == 1) {
			
			int pointone = patternpositions[1][x];
			
			int pointtwo = patternpositions[1][x] + 1;
			if (pointtwo == 256) {
				pointtwo = 0;
			}
			
			int stepdenominator = patternpositions[0][x];
			int steplength = patternlengths[x];
			
			int ccvalue = interpolationalg(pointone, pointtwo, stepdenominator, steplength);
			
			pendingsenddata[pendinglistpos][0] = midichannel;
			pendingsenddata[pendinglistpos][1] = midiccnumbers[x];
			pendingsenddata[pendinglistpos][2] = ccvalue;
			
			pendinglistpos++;
		}	
	}
	
}
// End of Functions
