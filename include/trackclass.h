
#ifndef TRACKCLASSES_H
#define TRACKCLASSES_H

#include "structs.h"

class track {

protected:

public:
	
	int currenteditpattern;
	
	virtual void starttrack(int playstatus) =0;
	virtual void resettrack(void) =0;
	
	virtual void sequencerclock(void) =0;
	
	virtual void displaypattern(void) =0;
	virtual void editpress(int xval, int yval) =0;
	
	virtual void displaypatternseq(void) =0;
	virtual void patternseqpress(int xval, int yval) =0;
	
	virtual void displayoptions(void) =0;
	virtual void optionspress(int xval, int yval) =0;

	virtual void displayactivepattern(void) =0;
	
	virtual void sendmididata(void) =0;

	virtual void fileload(modes_t currentmode) =0;
	virtual void filesave(modes_t currentmode) =0;

};



class mididrumtrack : public track {

private:

	int playing;
	int triggerplay;

	int tracknumber;

	int settingsnumber;
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
	
	void patternfileloader(void);
	void patternfilesaver(void);
	void settingsfileloader(void);
	void settingsfilesaver(void);

public:

	mididrumtrack(int assignedtracknumber);
	
	void starttrack(int playstatus);
	void resettrack();
	void sequencerclock(void);
	
	void displaypattern(void);
	void editpress(int xval, int yval);
	
	void displaypatternseq(void);
	void patternseqpress(int xval, int yval);
	
	void displayoptions(void);
	void optionspress(int xval, int yval);

	void displayactivepattern(void);
	
	void sendmididata(void);

	void fileload(modes_t currentmode);
	void filesave(modes_t currentmode);


};


class midinotetrack :public track {

private:

	int playing;
	int triggerplay;
	
	int tracknumber;

	int settingsnumber;
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
	uint8_t pendingsenddata[32][3];
	uint8_t currentonnotes[8][3];

	uint8_t midichannel;
	uint8_t midinotes[8][2];
	
	void triggernotes(void);
	
	void editmidioptions(int amount);
	
	void patternfileloader(void);
	void patternfilesaver(void);
	void settingsfileloader(void);
	void settingsfilesaver(void);

public:

	midinotetrack(int assignedtracknumber);
	
	void starttrack(int playstatus);
	void resettrack();
	void sequencerclock(void);
	
	void displaypattern(void);
	void editpress(int xval, int yval);
	
	void displaypatternseq(void);
	void patternseqpress(int xval, int yval);
	
	void displayoptions(void);
	void optionspress(int xval, int yval);

	void displayactivepattern(void);
	
	void sendmididata(void);

	void fileload(modes_t currentmode);
	void filesave(modes_t currentmode);


};

/*
class midicctrack :public track {

private:

	int playing;
	int triggerplay;
	
	int tracknumber;

	int settingsnumber;
	int patternnumber;

	int patterns[8][256];
	int patternlengths[8];
	
	int stepposition;

	int optionsrow;
	int optionscolumn;
	
	int activerow;
	int activecolumn;
	
	int stepbeatlength;
	int clockcount;
	
	int pendinglistpos;
	uint8_t pendingsenddata[32][3];

	uint8_t midichannel;
	uint8_t midiccnumbers[8];
	
	void triggernotes(void);
	
	void editmidioptions(int amount);
	
	void patternfileloader(void);
	void patternfilesaver(void);
	void settingsfileloader(void);
	void settingsfilesaver(void);

public:

	midicctrack(int assignedtracknumber);
	
	void starttrack(int playstatus);
	void resettrack();
	void sequencerclock(void);
	
	void displaypattern(void);
	void editpress(int xval, int yval);
	
	void displaypatternseq(void);
	void patternseqpress(int xval, int yval);
	
	void displayoptions(void);
	void optionspress(int xval, int yval);

	void displayactivepattern(void);
	
	void sendmididata(void);

	void fileload(modes_t currentmode);
	void filesave(modes_t currentmode);

};

*/
#endif //TRACKCLASSES_H
