
#ifndef TRACKCLASSES_H
#define TRACKCLASSES_H

#include "structs.h"

class track {

protected:
	
	int tracknumber;

public:
	
	int currenteditpattern;
	
	virtual void starttrack(int playstatus) =0;
	
	virtual void sequencerclock(void) =0;
	
	virtual void displaypattern(void) =0;
	virtual void editpress(int xval, int yval) =0;
	
	virtual void displaypatternseq(void) =0;
	virtual void patternseqpress(int xval, int yval) =0;
	
	virtual void displayoptions(void) =0;
	virtual void optionspress(int xval, int yval) =0;

	virtual void displayactivepattern(void) =0;
	
	virtual void sendmididata(void) =0;

};



class mididrumtrack : public track {

private:

	int playing;
	
	int tracknumber;

	int presetnumber;
	int patternnumber;

	int patterns[8][16][8];
	int patternseq[16];
	int patternseqlength;

	int patternseqpos;
	
	int stepposition;

	int optionsrow;
	int optionscolumn;
	
	int activerow;
	int activecolumn;
	
	int stepbeatlength;
	int clockcount;
	
	int pendinglistpos;
	int onnoteslistpos;
	uint8_t pendingsenddata[32][3];
	uint8_t currentonnotes[8][3];

	uint8_t midichannel;
	uint8_t midinotes[8][3];
	uint8_t notelength;
	
	void triggernoteson(void);
	void triggernotesoff(void);
	
	void editmidioptions(int amount);

public:

	mididrumtrack(int assignedtracknumber);
	
	void starttrack(int playstatus);
	void sequencerclock(void);
	
	void displaypattern(void);
	void editpress(int xval, int yval);
	
	void displaypatternseq(void);
	void patternseqpress(int xval, int yval);
	
	void displayoptions(void);
	void optionspress(int xval, int yval);

	void displayactivepattern(void);
	
	void sendmididata(void);


};

/*
class midicctrack :public track {

private;

	int patterns[8][256];
	int patternlengths[8];

	int patternseqpos;
	int currenteditpattern;
	int stepposition;

	uint8_t midichannel;
	uint8_t midiccnumbers[8];


public;

	void midicctrack();
	
	void sequencerclock(void);
	
	void displaypattern(void);
	void editpattern(void);
	
	void displaypatternseq(void);
	void editpatternseq(void);
	
	void displaymidioptions(void);
	void editmidioptions(void);

	void displayactivepattern(void);
	
	void setactivepattern(void);

};
*/

#endif //TRACKCLASSES_H
