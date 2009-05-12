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
		
		patternpositions[x][0] = 0;
		patternpositions[x][1] = 0;

		previousmessage[x][0] = 0;
		previousmessage[x][1] = 0;
		previousmessage[x][2] = 0;
		
		midiccnumbers[x] = 0;
		
	}
	
	for( y = 0; y < 8; y++ )
	{
		for (x = 0; x < 16; x++)
		{
			patternseq[y][x] = 0;
		}
		patternseq[y][0] = 1;
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
		patternpositions[x][0] = 0;
		patternpositions[x][1] = 0;
	}
	clockcount = 0;
	
}

void midicctrack::sequencerclock(void) {
	
	int x;

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
		
		stepposition++;
		if (stepposition == 16 * 16) {
			stepposition = 0;
		}
		
		for (x = 0; x < 8; x++) {
			
			if (activepatterns[x] == 1) {
				patternpositions[x][0]++;
				
				if (patternpositions[x][0] == patternlengths[x]) {
					patternpositions[x][0] = 0;
					
					patternpositions[x][1]++;
					
					if (patternpositions[x][1] == 256) {
						patternpositions[x][1] = 0;
					}
				}
			} else {
				patternpositions[x][0] = 0;
				patternpositions[x][1] = 0;
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


// Mode button functions

void midicctrack::modebuttondisplay(void) {

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

	iprintf("\x1b[21;9HEd");
	iprintf("\x1b[22;9Hit");

	iprintf("\x1b[21;13HSe");
	iprintf("\x1b[22;13Hq.");

	iprintf("\x1b[21;17HFl");
	iprintf("\x1b[22;17How");
	
	iprintf("\x1b[21;21HSe");
	iprintf("\x1b[22;21Ht.");

	iprintf("\x1b[21;26HLoad");
	iprintf("\x1b[22;26HSave");

}
	
	
void midicctrack::modebuttonpress(int xval) {

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

void midicctrack::editview(void) {
	
	for (int x = 0; x < 256; x++) {
	
		singlepoint(x, (128 - patterns[currenteditpattern][x]));
		
	}
	
	navbuttons(2,4,currenteditpattern);
}
	
	
void midicctrack::editpress(void) {

	if (keysDown() & KEY_START)
	{
		if (activepatterns[currenteditpattern] == 0)
		{
			activepatterns[currenteditpattern] = 1;
		}
		else if (activepatterns[currenteditpattern] == 1)
		{
			activepatterns[currenteditpattern] = 0;
		}
	}


	touchPosition touch;
	
	if ((keysDown() & KEY_TOUCH) || (keysHeld() & KEY_TOUCH)) {

		touchRead(&touch);
		
		int yval = touch.py;
		int xval = touch.px;

		if (yval < 128)
		{
			if (keysDown() & KEY_TOUCH) {
			
				prevx = xval;
				prevy = yval;
				
				patterns[currenteditpattern][xval] = (127 - yval);
						
			} else if (keysHeld() & KEY_TOUCH) {
			
				if ((xval != prevx) && (yval != prevy))
				{
					linealg(prevx, prevy, xval, yval);
					prevx = xval;
					prevy = yval;
				}
			
			}
			
			
		} else if ((yval > 127) && (yval < 160)) {
			
			
			xval = xval / 32;
			
			currenteditpattern = xval;
			
		}
	}
}

	

// Pattern sequencer functions

void midicctrack::patternseqview(void) {
	
	for(int x = 0; x < 16; x++) {
		for(int y = 0; y < 8; y++) {
			drawgridbutton(x,y,patternseq[y][x]);
		}
	}

}
void midicctrack::patternseqpress(void) {

	if (keysDown() & KEY_START)
	{
		if (activepatterns[currenteditpattern] == 0)
		{
			activepatterns[currenteditpattern] = 1;
		}
		else if (activepatterns[currenteditpattern] == 1)
		{
			activepatterns[currenteditpattern] = 0;
		}
	}

	touchPosition touch;
	
	if (keysDown() & KEY_TOUCH){

		touchRead(&touch);
		
		int yval = (touch.py / 16);
		int xval = (touch.px / 16);
	
		if (yval < 8)
		{
			for (int i = 0; i < 16; i++)
			{
				if (i <= xval)
				{
					patternseq[yval][i] = 1;
				}
				else if (i > xval)
				{
					patternseq[yval][i] = 0;
				}
			}
			patternlengths[yval] = (xval + 1);
		}
	}
}
	


// Follow view functions

void midicctrack::flowview(void) {
	
	for (int x = 0; x < 256; x++) {
		
		drawflowcurve(x, (128 - patterns[currenteditpattern][x]), patternpositions[currenteditpattern][1]);
	}
	
	navbuttons(2,4,currenteditpattern);

}

	
void midicctrack::flowpress(void) {

	if (keysDown() & KEY_START)
	{
		if (activepatterns[currenteditpattern] == 0)
		{
			activepatterns[currenteditpattern] = 1;
		}
		else if (activepatterns[currenteditpattern] == 1)
		{
			activepatterns[currenteditpattern] = 0;
		}
	}

	touchPosition touch;
	
	if ((keysDown() & KEY_TOUCH) || (keysHeld() & KEY_TOUCH)) {

		touchRead(&touch);
		
		int yval = touch.py;
		int xval = touch.px;

		if (yval < 128)
		{
			if (keysDown() & KEY_TOUCH) {
			
				prevx = xval;
				prevy = yval;
				
				patterns[currenteditpattern][xval] = (127 - yval);
						
			} else if (keysHeld() & KEY_TOUCH) {
			
				if ((xval != prevx) && (yval != prevy))
				{
					linealg(prevx, prevy, xval, yval);
					prevx = xval;
					prevy = yval;
				}
			
			}
			
			
		} else if ((yval > 127) && (yval < 160)) {
			
			
			xval = xval / 32;
			
			currenteditpattern = xval;
			
		}
	}
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
	
	iprintf("\x1b[11;14H%i  ",midiccnumbers[0]);
	iprintf("\x1b[12;14H%i  ",midiccnumbers[1]);
	iprintf("\x1b[13;14H%i  ",midiccnumbers[2]);
	iprintf("\x1b[14;14H%i  ",midiccnumbers[3]);
	iprintf("\x1b[15;14H%i  ",midiccnumbers[4]);
	iprintf("\x1b[16;14H%i  ",midiccnumbers[5]);
	iprintf("\x1b[17;14H%i  ",midiccnumbers[6]);
	iprintf("\x1b[18;14H%i  ",midiccnumbers[7]);
	
	optionsscreenbackground(activerow, 1);
	
	drawkeypad(24,2);
	
	int activevalue = 0;

	if (activerow == -1) {activevalue = 0;}

	if (activerow == 4) {activevalue = settingsnumber;}
	if (activerow == 5) {activevalue = patternnumber;}
	
	if (activerow == 8) {activevalue = midichannel;}

	if ((activerow > 10) && (activerow < 19)) {activevalue = midiccnumbers[activerow - 11];}

	calcanddispnumber(24,4,activevalue);
}
	
	
void midicctrack::optionspress(void) {
	
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
}




// Load and Save Functions

void midicctrack::loadsaveview(void) {

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

void midicctrack::loadsavepress(void) {



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

int midicctrack::interpolationalg(int valueone, int valuetwo, int stepdenominator, int steplength) {

	int dif = valuetwo - valueone;

	if ((stepdenominator == 0) || (dif == 0)) {
		return valueone;
	}
	
	int interpolatedval = (valueone + (dif * stepdenominator) / steplength);
	
	return interpolatedval;

}


// Screen press line drawing alg

void midicctrack::linealg(int x1, int y1, int x2, int y2) {

    int delta_x = abs(x2 - x1) << 1;
    int delta_y = abs(y2 - y1) << 1;

    // if x1 == x2 or y1 == y2, then it does not matter what we set here
    signed char ix = x2 > x1?1:-1;
    signed char iy = y2 > y1?1:-1;

    patterns[currenteditpattern][x1] = (128 - y1);

    if (delta_x >= delta_y)
    {
        // error may go below zero
        int error = delta_y - (delta_x >> 1);

        while (x1 != x2)
        {
            if (error >= 0)
            {
                if (error || (ix > 0))
                {
                    y1 += iy;
                    error -= delta_x;
                }
                // else do nothing
            }
            // else do nothing

            x1 += ix;
            error += delta_y;

            patterns[currenteditpattern][x1] = (128 - y1);
        }
    }
    else
    {
        // error may go below zero
        int error = delta_x - (delta_y >> 1);

        while (y1 != y2)
        {
            if (error >= 0)
            {
                if (error || (iy > 0))
                {
                    x1 += ix;
                    error -= delta_y;
                }
                // else do nothing
            }
            // else do nothing

            y1 += iy;
            error += delta_x;

            patterns[currenteditpattern][x1] = (128 - y1);
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
			
			int valone = patterns[x][patternpositions[x][1]];
			
			int postwo = patternpositions[x][1] + 1;
			if (postwo == 256) {
				postwo = 0;
			}
			
			int valtwo = patterns[x][postwo];
			
			int stepdenominator = patternpositions[x][0];
			int steplength = patternlengths[x];
			
			int ccvalue = interpolationalg(valone, valtwo, stepdenominator, steplength);
			
			if (!((midichannel == previousmessage[x][0]) && (midiccnumbers[x] == previousmessage[x][1]) && (ccvalue == previousmessage[x][2])))
			{
				pendingsenddata[pendinglistpos][0] = midichannel;
				pendingsenddata[pendinglistpos][1] = midiccnumbers[x];
				pendingsenddata[pendinglistpos][2] = ccvalue;
				
				pendinglistpos++;
				
				previousmessage[x][0] = midichannel;
				previousmessage[x][1] = midiccnumbers[x];
				previousmessage[x][2] = ccvalue;
			}
		}	
	}
	
}
// End of Functions
