
#ifndef STRUCTS_H
#define STRUCTS_H

enum modes_t {home, edit, patternseq, follow, options, setup};


struct midinoteoptionsbuffer {

int tracknumber;
int presetnumber;
int patternnumber;

uint8_t midichannel;
uint8_t midinotes[8][3];

};

struct presetbuffer {

int presetnumber;

uint8_t midichannel;
uint8_t midinotes[8][3];

};

struct patternbuffer {

int patternnumber;

int patterns[8][16][8];
int patternseq[16];
int patternseqlength;

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
