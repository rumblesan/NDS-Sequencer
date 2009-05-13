
#ifndef TRACKCLASSES_H
#define TRACKCLASSES_H

#include "structs.h"

class track {

protected:

public:
	
	virtual void starttrack(int playstatus) =0;
	virtual void resettrack(void) =0;
	
	virtual void sequencerclock(void) =0;
	
	virtual void sendmididata(void) =0;
	
	virtual void modebuttondisplay(void) =0;
	virtual void modebuttonpress(int xval) =0;
	
	virtual void editview(void) =0;
	virtual void editpress(void) =0;
	
	virtual void patternseqview(void) =0;
	virtual void patternseqpress(void) =0;
	
	virtual void flowview(void) =0;
	virtual void flowpress(void) =0;
	
	virtual void optionsview(void) =0;
	virtual void optionspress(void) =0;

	virtual void loadsaveview(void) =0;
	virtual void loadsavepress(void) =0;

};



class mididrumtrack :public track {

private:

	int playing;
	int triggerplay;

	int currenteditpattern;
	
	int tracknumber;

	int settingsnumber;
	int patternnumber;

	int patterns[8][16][8];
	int patternseq[16];
	int patternseqlength;

	int patternseqpos;
	
	int stepposition;

	int activerow;
	int activecolumn;

	int stepbeatlength;
	int clockcount;
	
	int pendinglistpos;
	uint8_t pendingsenddata[32][3];
	uint8_t currentonnotes[8][3];

	uint8_t midichannel;
	uint8_t midinotes[8][3];
	uint8_t notelength;
	
	void triggernoteson(void);
	void triggernotesoff(void);
	
	void editoption(int amount);
	
	void patternfileloader(void);
	void patternfilesaver(void);
	void settingsfileloader(void);
	void settingsfilesaver(void);

public:

	mididrumtrack(int assignedtracknumber);
	
	void starttrack(int playstatus);
	void resettrack(void);
	
	void sequencerclock(void);
	
	void sendmididata(void);
	
	void modebuttondisplay(void);
	void modebuttonpress(int xval);
	
	void editview(void);
	void editpress(void);
	
	void patternseqview(void);
	void patternseqpress(void);
	
	void flowview(void);
	void flowpress(void);
	
	void optionsview(void);
	void optionspress(void);

	void loadsaveview(void);
	void loadsavepress(void);

};


class midinotetrack :public track {

private:

	int playing;
	int triggerplay;
	
	int currenteditpattern;
	
	int tracknumber;

	int settingsnumber;
	int patternnumber;

	int patterns[8][16][8];
	int patternseq[16];
	int patternseqlength;

	int patternseqpos;
	
	int stepposition;
	
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
	
	void editoption(int amount);
	
	void patternfileloader(void);
	void patternfilesaver(void);
	void settingsfileloader(void);
	void settingsfilesaver(void);

public:

	midinotetrack(int assignedtracknumber);
	
	void starttrack(int playstatus);
	void resettrack(void);
	
	void sequencerclock(void);
	
	void sendmididata(void);
	
	void modebuttondisplay(void);
	void modebuttonpress(int xval);
	
	void editview(void);
	void editpress(void);
	
	void patternseqview(void);
	void patternseqpress(void);
	
	void flowview(void);
	void flowpress(void);
	
	void optionsview(void);
	void optionspress(void);

	void loadsaveview(void);
	void loadsavepress(void);

};


class midicctrack :public track {

private:

	int playing;
	int triggerplay;
	
	int currenteditpattern;

	int tracknumber;
	
	int stepposition;

	int settingsnumber;
	int patternnumber;

	int patterns[8][256];

	int previousmessage[8][3];

	int patternpositions[8][2];
	int activepatterns[8];
	
	int activerow;
	int activecolumn;
	
	int clockcount;
	
	int pendinglistpos;
	
	uint8_t pendingsenddata[32][3];

	uint8_t midichannel;
	uint8_t midiccnumbers[8];
	int patternlengths[8];
	
	int prevx;
	int prevy;
	
	
	void triggernotes(void);
	
	int interpolationalg(int pointone, int pointtwo, int stepdenominator, int steplength);

	void linealg(int pointone, int pointtwo, int stepdenominator, int steplength);

	
	void loadvalues(void);
	
	void editoption(int amount);
	
	void patternfileloader(void);
	void patternfilesaver(void);
	void settingsfileloader(void);
	void settingsfilesaver(void);

public:

	midicctrack(int assignedtracknumber);
	
	void starttrack(int playstatus);
	void resettrack(void);
	
	void sequencerclock(void);
	
	void sendmididata(void);
	
	void modebuttondisplay(void);
	void modebuttonpress(int xval);
	
	void editview(void);
	void editpress(void);
	
	void patternseqview(void);
	void patternseqpress(void);
	
	void flowview(void);
	void flowpress(void);
	
	void optionsview(void);
	void optionspress(void);

	void loadsaveview(void);
	void loadsavepress(void);

};


#endif //TRACKCLASSES_H
