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


// Nav Buttons

void navbuttons(int colour, int yoffset, int activebutton);

// Grid Button Function

void drawgridbutton(int xoffset, int yoffset, int buttonval);



// Curve Drawing Functions

void drawcolumn(int x, int y);
void drawline(int x);
void singlepoint(int x, int y);
void drawpixel(int x, int y, int colour);



// Button Display Functions

void drawbigbutton(int xoffset, int yoffset, int colour);
void drawbiglongbutton(int xoffset, int yoffset, int colour);
void drawmidbutton(int xoffset, int yoffset, int colour);


// Text Display Functions

void drawmidnumber(int xoffset, int yoffset, int number);
void drawbignumber(int xoffset, int yoffset, int number);
void drawarrow(int xoffset, int yoffset, int direction);
void mainnumbers(int xoffset, int yoffset, int colour);
void calcanddispnumber(int xoffset, int yoffset, int number);


// File Browser

void filebrowsescreenbackground();


// Setup Screen

void setupscreenbackground();
void drawsetuptext(int dsmienable);


// Options Screen

void optionsscreenbackground(int xval, int yval, int length);
void drawkeypad(int xoffset, int yoffset);


// Clear Screen Function

void clearbottomscreen();


#endif //GFX_H