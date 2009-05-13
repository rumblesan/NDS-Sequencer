
#ifndef STRUCTS_H
#define STRUCTS_H

enum modes_t {home, edit, seqpatterns, follow, options, loadsave, setup, filebrowse, misc};


struct settingsbuffer {

int settingsnumber;

uint8_t midichannel;
uint8_t midinotes[8][3];
uint8_t notelength;

};

struct gridpatternbuffer {

int patternnumber;

int patterns[8][16][8];
int patternseq[16];
int patternseqlength;
int stepbeatlength;

};

struct ccpatternbuffer {

int patternnumber;

int patterns[8][256];
int patternlengths[8];

};

struct ccsettingsbuffer {

int settingsnumber;

uint8_t midichannel;
uint8_t midiccnumbers[8];

};

#endif //STRUCTS_H
