#ifndef GFX_H
#define GFX_H

#include "structs.h"


// Video Setup Function

void setupVideo();


// Top Screen Functions

void displaybpm(int bpm, int xoffset, int yoffset);
void drawbignumberSUB(int xoffset, int yoffset, int number);
void drawbpmtextSUB(int xoffset, int yoffset);
void drawgridbuttonSUB(int xoffset, int yoffset, int buttonval);
void drawstepdisplaySUB(int globalstep, int yoffset);
void drawbigbuttonSUB(int xoffset, int yoffset, int colour);
void drawbiglongbuttonSUB(int xoffset, int yoffset, int colour);
void drawtriggeredsoundsSUB(trackdata * trackarray, int xoffset, int yoffset, int channel);


// Nav Buttons

void navbuttonwords();
void navbuttons(int colour, int yoffset, int activebutton);


// Grid Button Function

void drawgridbutton(int xoffset, int yoffset, int buttonval);
void displaycurrentgrid(trackdata * activetrack);
void displayseqgrid(trackdata * activetrack);
void displayactivegrid(trackdata * activetrack);


// Button Display Functions

void drawbigbutton(int xoffset, int yoffset, int colour);
void drawbiglongbutton(int xoffset, int yoffset, int colour);


// Text Display Functions

void drawmidnumber(int xoffset, int yoffset, int number);
void drawbignumber(int xoffset, int yoffset, int number);
void drawarrow(int xoffset, int yoffset, int direction);
void mainnumbers(int xoffset, int yoffset, int colour);
void calcanddispnumber(int xoffset, int yoffset, int number);
void displayactiverowvalue(trackdata * activetrack, int activerow, int activecolumn);


// File Browser

void filebrowsescreenbackground();


// Setup Screen

void displaysetuprowvalue(int setuprow, int globalnumber);
void setupscreenbackground(int setuprow);
void drawsetuptext(int dsmienable, int globalnumber);


// Options Screen

void drawoptionstext(midinoteoptionsbuffer * activetrack);
void optionsscreenbackground(int activerow, int activecolumn);
void drawkeypad(int xoffset, int yoffset);


// Clear Screen Function

void clearbottomscreen();


#endif //GFX_H