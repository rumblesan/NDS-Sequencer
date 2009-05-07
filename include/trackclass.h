
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
	
	virtual void sendmididata(void) =0;
	
	virtual void editview(void) =0;
	virtual void editpress(int xval, int yval) =0;
	
	virtual void patternseqview(void) =0;
	virtual void patternseqpress(int xval, int yval) =0;
	
	virtual void flowview(void) =0;
	virtual void flowpress(int xval, int yval) =0;
	
	virtual void optionsview(void) =0;
	virtual void optionspress(int xval, int yval) =0;

	virtual void fileload(void) =0;
	virtual void filesave(void) =0;

};



class mididrumtrack :public track {

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
	
	void editmidioptions(int amount);
	
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
	
	void editview(void);
	void editpress(int xval, int yval);
	
	void patternseqview(void);
	void patternseqpress(int xval, int yval);
	
	void flowview(void);
	void flowpress(int xval, int yval);
	
	void optionsview(void);
	void optionspress(int xval, int yval);

	void fileload(void);
	void filesave(void);

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
	void resettrack(void);
	
	void sequencerclock(void);
	
	void sendmididata(void);
	
	void editview(void);
	void editpress(int xval, int yval);
	
	void patternseqview(void);
	void patternseqpress(int xval, int yval);
	
	void flowview(void);
	void flowpress(int xval, int yval);
	
	void optionsview(void);
	void optionspress(int xval, int yval);

	void fileload(void);
	void filesave(void);


};


class midicctrack :public track {

private:

	int playing;
	int triggerplay;
	
	int tracknumber;

	int settingsnumber;
	int patternnumber;

	int patterns[8][256];
	int patternlengths[8];

	int patternpositions[8];
	
	int activerow;
	int activecolumn;
	
	int clockcount;
	
	int pendinglistpos;
	uint8_t pendingsenddata[32][3];

	uint8_t midichannel;
	uint8_t midiccnumbers[8];
	
	void loadvalues(void);
	
	void editmidioptions(int amount);
	
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
	
	void editview(void);
	void editpress(int xval, int yval);
	
	void patternseqview(void);
	void patternseqpress(int xval, int yval);
	
	void flowview(void);
	void flowpress(int xval, int yval);
	
	void optionsview(void);
	void optionspress(int xval, int yval);

	void fileload(void);
	void filesave(void);

};


#endif //TRACKCLASSES_H
