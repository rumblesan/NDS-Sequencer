
#ifndef STRUCTS_H
#define STRUCTS_H

enum modes_t {home, edit, seqpatterns, follow, options, setup, loadsave};


struct settingsbuffer {

int settingsnumber;

uint8_t midichannel;
uint8_t midinotes[8][3];
uint8_t notelength;

};

struct patternbuffer {

int patternnumber;

int patterns[8][16][8];
int patternseq[16];
int patternseqlength;
int stepbeatlength;

};

struct trackbuffer {

int patternnumber[4];
int presetnumber[4];

int patterns[4][8][16][8];
int patternseq[4][16];
int patternseqlength[4];

uint8_t midichannel[4];
uint8_t midinotes[4][8][3];

};

#endif //STRUCTS_H
