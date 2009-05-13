#include <nds.h>
#include <stdio.h>

#include "structs.h"

// Grid Buttons
#include "gfx_bluegridbutton.h"
#include "gfx_greengridbutton.h"
#include "gfx_graygridbutton.h"
#include "gfx_orangegridbutton.h"
#include "gfx_yellowgridbutton.h"
#include "gfx_redgridbutton.h"

// Menu Buttons
#include "gfx_lrgbluebutton.h"
#include "gfx_lrggreenbutton.h"
#include "gfx_lrgredbutton.h"
#include "gfx_lrgyellowbutton.h"

// Screen Tilemap Tiles
#include "gfx_optionsviewtiles.h"

// Number Tiles
#include "gfx_midnumbers.h"

// Large Text Tiles
#include "gfx_bignumb0.h"
#include "gfx_bignumb1.h"
#include "gfx_bignumb2.h"
#include "gfx_bignumb3.h"
#include "gfx_bignumb4.h"
#include "gfx_bignumb5.h"
#include "gfx_bignumb6.h"
#include "gfx_bignumb7.h"
#include "gfx_bignumb8.h"
#include "gfx_bignumb9.h"


// Small Text Tiles
#include "gfx_uparrow.h"
#include "gfx_downarrow.h"

// Top Screen Specific Tiles
#include "gfx_bigbpmtext.h"

// GENERAL BG TILES
#define tile_empty				0

// GRID BUTTON TILES
#define tile_gridbutton			1

// MENU BUTTON TILES
#define tile_buttontiles		17

// NUMBER TILES
#define tile_midnumbers			26
#define tile_bignumb0			66
#define tile_bignumb1			78
#define tile_bignumb2			90
#define tile_bignumb3			102
#define tile_bignumb4			114
#define tile_bignumb5			126
#define tile_bignumb6			138
#define tile_bignumb7			150
#define tile_bignumb8			162
#define tile_bignumb9			174


// KEYMAP TILES
#define tile_optionsviewtiles	186


// SMALL TEXT TILES
#define tile_uparrow			197
#define tile_downarrow			201


// TILES ONLY FOR SUBRAM

#define tile_bigbpmtext			205



// BG MAIN VRAM PALETTES
#define pal_graygridbutton		1
#define pal_greengridbutton		2
#define pal_bluegridbutton		3
#define pal_yellowgridbutton	4
#define pal_redgridbutton		5
#define pal_orangegridbutton	6
#define pal_lrgbluebutton		7
#define pal_lrggreenbutton		8
#define pal_lrgredbutton		9
#define pal_lrgyellowbutton		10
#define pal_brandw				11
#define pal_optionsviewtiles	12


// BG Colours
#define backdrop_colour		RGB8( 40, 40, 40 )
#define subbackdrop_colour	RGB8( 40, 40, 40 )


// MACRO DEFS
#define tile2bgram(t)		(BG_GFX + (t) * 16)
#define pal2bgram(p)		(BG_PALETTE + (p) * 16)

#define tile2bgsubram(t)	(BG_GFX_SUB + (t) * 16)
#define pal2bgsubram(p)		(BG_PALETTE_SUB + (p) * 16)


//background map defines
#define bg1map			((u16*)BG_MAP_RAM(5))
#define bg1mapsub		((u16*)BG_MAP_RAM_SUB(5))
#define bg2map			((u16*)BG_MAP_RAM(6))
#define bg2mapsub		((u16*)BG_MAP_RAM_SUB(6))

#define pixelbg			((u16*)BG_BMP_RAM(4))


u8 longbutton[36] = 
{
	0,4,4,4,4,4,4,4,4,4,4,1,
	7,8,8,8,8,8,8,8,8,8,8,5,
	2,6,6,6,6,6,6,6,6,6,6,3,
};

u8 doublemidbuttonmap[32] = 
{
	0,4,4,4,4,4,4,1,
	7,8,8,8,8,8,8,5,
	7,8,8,8,8,8,8,5,
	2,6,6,6,6,6,6,3,
};

u8 bigbuttonmap[48] = 
{
	0,4,4,4,4,4,4,1,
	7,8,8,8,8,8,8,5,
	7,8,8,8,8,8,8,5,
	7,8,8,8,8,8,8,5,
	7,8,8,8,8,8,8,5,
	2,6,6,6,6,6,6,3,
};


u8 midbuttonmap[16] = 
{
	0,4,4,1,
	7,8,8,5,
	7,8,8,5,
	2,6,6,3,
};

u8 keypadmap[64] = 
{

	1,5,5,5,5,5,5,2,
	6,9,9,9,9,9,9,7,
	6,9,9,9,9,9,9,7,
	6,9,9,9,9,9,9,7,
	6,9,9,9,9,9,9,7,
	6,9,9,9,9,9,9,7,
	6,9,9,9,9,9,9,7,
	3,8,8,8,8,8,8,4,
	
	
};


u8 optionsscreen[768] =
{

	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,2,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,3,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,4,0,
	
};



u8 filebrowsescreen[768] =
{

	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,2,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,0,
	0,3,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,4,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	
};





// Setup Function

void setupVideo() {


	vramSetBankA( VRAM_A_MAIN_BG );

	vramSetBankC( VRAM_C_SUB_BG );

	videoSetMode( MODE_3_2D | DISPLAY_BG1_ACTIVE | DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE );
	videoSetModeSub( MODE_0_2D | DISPLAY_BG1_ACTIVE | DISPLAY_BG2_ACTIVE );

    lcdMainOnBottom();

    int n;
    for( n = 0; n < 16; n++ ) {
        BG_GFX[n] = 0;
        BG_GFX_SUB[n] = 0;
	}
    
	
    // MAIN ENGINE BG VRAM
	
    dmaCopyHalfWords( 3, gfx_bluegridbuttonTiles,   tile2bgram( tile_gridbutton ),       gfx_bluegridbuttonTilesLen );
 	
    dmaCopyHalfWords( 3, gfx_lrgbluebuttonTiles,    tile2bgram( tile_buttontiles ),      gfx_lrgbluebuttonTilesLen );

	dmaCopyHalfWords( 3, gfx_midnumbersTiles,       tile2bgram( tile_midnumbers ),       gfx_midnumbersTilesLen );

	dmaCopyHalfWords( 3, gfx_bignumb0Tiles,         tile2bgram( tile_bignumb0 ),         gfx_bignumb0TilesLen );
	dmaCopyHalfWords( 3, gfx_bignumb1Tiles,         tile2bgram( tile_bignumb1 ),         gfx_bignumb1TilesLen );
	dmaCopyHalfWords( 3, gfx_bignumb2Tiles,         tile2bgram( tile_bignumb2 ),         gfx_bignumb2TilesLen );
	dmaCopyHalfWords( 3, gfx_bignumb3Tiles,         tile2bgram( tile_bignumb3 ),         gfx_bignumb3TilesLen );
	dmaCopyHalfWords( 3, gfx_bignumb4Tiles,         tile2bgram( tile_bignumb4 ),         gfx_bignumb4TilesLen );
	dmaCopyHalfWords( 3, gfx_bignumb5Tiles,         tile2bgram( tile_bignumb5 ),         gfx_bignumb5TilesLen );
	dmaCopyHalfWords( 3, gfx_bignumb6Tiles,         tile2bgram( tile_bignumb6 ),         gfx_bignumb6TilesLen );
	dmaCopyHalfWords( 3, gfx_bignumb7Tiles,         tile2bgram( tile_bignumb7 ),         gfx_bignumb7TilesLen );
	dmaCopyHalfWords( 3, gfx_bignumb8Tiles,         tile2bgram( tile_bignumb8 ),         gfx_bignumb8TilesLen );
	dmaCopyHalfWords( 3, gfx_bignumb9Tiles,         tile2bgram( tile_bignumb9 ),         gfx_bignumb9TilesLen );

	dmaCopyHalfWords( 3, gfx_optionsviewtilesTiles, tile2bgram( tile_optionsviewtiles ), gfx_optionsviewtilesTilesLen );
	
	dmaCopyHalfWords( 3, gfx_uparrowTiles,          tile2bgram( tile_uparrow ),          gfx_uparrowTilesLen );
	dmaCopyHalfWords( 3, gfx_downarrowTiles,        tile2bgram( tile_downarrow ),        gfx_downarrowTilesLen );
	

	// SUB ENGINE BG VRAM
	
	dmaCopyHalfWords( 3, gfx_bluegridbuttonTiles,   tile2bgsubram( tile_gridbutton ),       gfx_bluegridbuttonTilesLen );
 	
    dmaCopyHalfWords( 3, gfx_lrgbluebuttonTiles,    tile2bgsubram( tile_buttontiles ),      gfx_lrgbluebuttonTilesLen );

	dmaCopyHalfWords( 3, gfx_midnumbersTiles,       tile2bgsubram( tile_midnumbers ),       gfx_midnumbersTilesLen );

	dmaCopyHalfWords( 3, gfx_bignumb0Tiles,         tile2bgsubram( tile_bignumb0 ),         gfx_bignumb0TilesLen );
	dmaCopyHalfWords( 3, gfx_bignumb1Tiles,         tile2bgsubram( tile_bignumb1 ),         gfx_bignumb1TilesLen );
	dmaCopyHalfWords( 3, gfx_bignumb2Tiles,         tile2bgsubram( tile_bignumb2 ),         gfx_bignumb2TilesLen );
	dmaCopyHalfWords( 3, gfx_bignumb3Tiles,         tile2bgsubram( tile_bignumb3 ),         gfx_bignumb3TilesLen );
	dmaCopyHalfWords( 3, gfx_bignumb4Tiles,         tile2bgsubram( tile_bignumb4 ),         gfx_bignumb4TilesLen );
	dmaCopyHalfWords( 3, gfx_bignumb5Tiles,         tile2bgsubram( tile_bignumb5 ),         gfx_bignumb5TilesLen );
	dmaCopyHalfWords( 3, gfx_bignumb6Tiles,         tile2bgsubram( tile_bignumb6 ),         gfx_bignumb6TilesLen );
	dmaCopyHalfWords( 3, gfx_bignumb7Tiles,         tile2bgsubram( tile_bignumb7 ),         gfx_bignumb7TilesLen );
	dmaCopyHalfWords( 3, gfx_bignumb8Tiles,         tile2bgsubram( tile_bignumb8 ),         gfx_bignumb8TilesLen );
	dmaCopyHalfWords( 3, gfx_bignumb9Tiles,         tile2bgsubram( tile_bignumb9 ),         gfx_bignumb9TilesLen );

	dmaCopyHalfWords( 3, gfx_optionsviewtilesTiles, tile2bgsubram( tile_optionsviewtiles ), gfx_optionsviewtilesTilesLen );
	
	dmaCopyHalfWords( 3, gfx_bigbpmtextTiles,       tile2bgsubram( tile_bigbpmtext ),       gfx_bigbpmtextTilesLen );



    // MAIN ENGINE PALLETTE RAM
	
    dmaCopyHalfWords( 3, gfx_bluegridbuttonPal,    pal2bgram( pal_bluegridbutton ),    gfx_bluegridbuttonPalLen );	
    dmaCopyHalfWords( 3, gfx_greengridbuttonPal,   pal2bgram( pal_greengridbutton ),   gfx_greengridbuttonPalLen );	
    dmaCopyHalfWords( 3, gfx_graygridbuttonPal,    pal2bgram( pal_graygridbutton ),    gfx_graygridbuttonPalLen );	
    dmaCopyHalfWords( 3, gfx_orangegridbuttonPal,  pal2bgram( pal_orangegridbutton ),  gfx_orangegridbuttonPalLen );
	dmaCopyHalfWords( 3, gfx_yellowgridbuttonPal,  pal2bgram( pal_yellowgridbutton ),  gfx_yellowgridbuttonPalLen );	
    dmaCopyHalfWords( 3, gfx_redgridbuttonPal,     pal2bgram( pal_redgridbutton ),     gfx_redgridbuttonPalLen );

	dmaCopyHalfWords( 3, gfx_lrgbluebuttonPal,     pal2bgram( pal_lrgbluebutton ),     gfx_lrgbluebuttonPalLen );	
    dmaCopyHalfWords( 3, gfx_lrggreenbuttonPal,    pal2bgram( pal_lrggreenbutton ),    gfx_lrggreenbuttonPalLen );	
	dmaCopyHalfWords( 3, gfx_lrgredbuttonPal,      pal2bgram( pal_lrgredbutton ),      gfx_lrgredbuttonPalLen );	
    dmaCopyHalfWords( 3, gfx_lrgyellowbuttonPal,   pal2bgram( pal_lrgyellowbutton ),   gfx_lrgyellowbuttonPalLen );
	
	dmaCopyHalfWords( 3, gfx_bignumb0Pal,          pal2bgram( pal_brandw ),            gfx_bignumb0PalLen );
    
	dmaCopyHalfWords( 3, gfx_optionsviewtilesPal,  pal2bgram( pal_optionsviewtiles ),  gfx_optionsviewtilesPalLen );
	
	// SUB ENGINE PALLETTE RAM
	
    dmaCopyHalfWords( 3, gfx_bluegridbuttonPal,    pal2bgsubram( pal_bluegridbutton ),   gfx_bluegridbuttonPalLen );	
    dmaCopyHalfWords( 3, gfx_greengridbuttonPal,   pal2bgsubram( pal_greengridbutton ),  gfx_greengridbuttonPalLen );	
    dmaCopyHalfWords( 3, gfx_graygridbuttonPal,    pal2bgsubram( pal_graygridbutton ),   gfx_graygridbuttonPalLen );	
    dmaCopyHalfWords( 3, gfx_orangegridbuttonPal,  pal2bgsubram( pal_orangegridbutton ), gfx_orangegridbuttonPalLen );
    dmaCopyHalfWords( 3, gfx_yellowgridbuttonPal,  pal2bgsubram( pal_yellowgridbutton ), gfx_yellowgridbuttonPalLen );	
    dmaCopyHalfWords( 3, gfx_redgridbuttonPal,     pal2bgsubram( pal_redgridbutton ),    gfx_redgridbuttonPalLen );

	dmaCopyHalfWords( 3, gfx_lrgbluebuttonPal,     pal2bgsubram( pal_lrgbluebutton ),    gfx_lrgbluebuttonPalLen );	
    dmaCopyHalfWords( 3, gfx_lrggreenbuttonPal,    pal2bgsubram( pal_lrggreenbutton ),   gfx_lrggreenbuttonPalLen );	
	dmaCopyHalfWords( 3, gfx_lrgredbuttonPal,      pal2bgsubram( pal_lrgredbutton ),     gfx_lrgredbuttonPalLen );	
    dmaCopyHalfWords( 3, gfx_lrgyellowbuttonPal,   pal2bgsubram( pal_lrgyellowbutton ),  gfx_lrgyellowbuttonPalLen );
	
	dmaCopyHalfWords( 3, gfx_bignumb0Pal,          pal2bgsubram( pal_brandw ),           gfx_bignumb0PalLen );
    
	dmaCopyHalfWords( 3, gfx_optionsviewtilesPal,  pal2bgsubram( pal_optionsviewtiles ), gfx_optionsviewtilesPalLen );
	

    // set backdrop colours
    BG_PALETTE[0] = backdrop_colour;
    BG_PALETTE_SUB[0] = subbackdrop_colour;

	REG_BG3CNT = BG_BMP16_256x256 | BG_BMP_BASE(4);
	
	REG_BG3PA=1<<8;

	REG_BG3PB=0;
	REG_BG3PC=0;
	REG_BG3PD=1<<8;

	REG_BG1CNT = BG_MAP_BASE(5);
	REG_BG1CNT_SUB = BG_MAP_BASE(5);

	REG_BG2CNT = BG_MAP_BASE(6);
    REG_BG2CNT_SUB = BG_MAP_BASE(6);

}



// Top screen Functions

void drawbignumberSUB(int xoffset, int yoffset, int number) {

	int x, y;
	int numbercycle = 0;
	for( y = 0; y < 4; y++ )
	{
		for( x = 0; x < 3; x++ )
		{
			int tile = (tile_bignumb0 + (number * 12))  + numbercycle;
			numbercycle++;
			bg1mapsub[(x + xoffset) + (y + yoffset) * 32] = tile | (pal_brandw << 12);
		}
	}
}

void displaybpm(int bpm, int xoffset, int yoffset) {


	int hundreds = (bpm / 100);

	int tens = ((bpm % 100) / 10);

	int units = (bpm % 10);
	
	drawbignumberSUB((xoffset),yoffset,hundreds);
	drawbignumberSUB((xoffset + 3),yoffset,tens);
	drawbignumberSUB((xoffset + 6),yoffset,units);

}


void drawbpmtextSUB(int xoffset, int yoffset) {

	int x, y;
	int numbercycle = 0;
	for( y = 0; y < 4; y++ )
	{
		for( x = 0; x < 9; x++ )
		{
			int tile = tile_bigbpmtext + numbercycle;
			numbercycle++;
			bg1mapsub[(x + xoffset) + (y + yoffset) * 32] = tile | (pal_brandw << 12);
		}
	}
}
void drawgridbuttonSUB(int xoffset, int yoffset, int buttonval) {


	int palref = pal_graygridbutton + buttonval;


    int x, y;
	int buttoncycle = 0;
    for( y = 0; y < 2; y++ )
    {
        for( x = 0; x < 2; x++ )
        {
			int tile = tile_gridbutton + buttoncycle;
			buttoncycle++;
		    bg1mapsub[(x + (xoffset * 2)) + (y + (yoffset * 2)) * 32] = tile | (palref << 12);
        }
    }
}




void drawstepdisplaySUB(int globalstep, int yoffset) {

    int x;

	for( x = 0; x < 16; x++ )
	{
		if (x == globalstep)
		{
			drawgridbuttonSUB(x, yoffset, 3);
		} else {
			drawgridbuttonSUB(x, yoffset, 0);
		}
	}


}
void drawbigbuttonSUB(int xoffset, int yoffset, int colour) {

int pallette;

	if(colour == 0) {pallette = pal_lrgredbutton;}
	if(colour == 1) {pallette = pal_lrggreenbutton;}
	if(colour == 2) {pallette = pal_lrgbluebutton;}
	if(colour == 3) {pallette = pal_lrgyellowbutton;}

    int x, y;
	int buttoncycle = 0;
    for( y = 0; y < 6; y++ )
    {
        for( x = 0; x < 8; x++ )
        {
			int tile = tile_buttontiles + bigbuttonmap[buttoncycle];
			buttoncycle++;
		    bg2mapsub[(x + (xoffset)) + (y + (yoffset)) * 32] = tile | (pallette << 12);
        }
    }
}

void drawbiglongbuttonSUB(int xoffset, int yoffset, int colour) {

int pallette;

	if(colour == 0) {pallette = pal_lrgredbutton;}
	if(colour == 1) {pallette = pal_lrggreenbutton;}
	if(colour == 2) {pallette = pal_lrgbluebutton;}
	if(colour == 3) {pallette = pal_lrgyellowbutton;}

    int x, y;
	int tile;
	int buttoncycle = 0;
    for( y = 0; y < 4; y++ )
    {
        for( x = 0; x < 8; x++ )
        {
			tile = tile_buttontiles + doublemidbuttonmap[buttoncycle];
			buttoncycle++;
		    bg2mapsub[(x + xoffset) + (y + yoffset) * 32] = tile | (pallette << 12);
        }
    }
}


// Nav Buttons


void navbuttons(int colour, int yoffset, int activebutton) {

	int basepallette, activepallette, pallette;
	int i, x, y;
	int buttoncycle = 0;
	int tile;

	if(colour == 0) {
		basepallette = pal_lrgredbutton;
		activepallette = pal_lrggreenbutton;
	} else if(colour == 1) {
		basepallette = pal_lrggreenbutton;
		activepallette = pal_lrgredbutton;
	} else if(colour == 2) {
		basepallette = pal_lrgbluebutton;
		activepallette = pal_lrgyellowbutton;
	} else {
		basepallette = pal_lrgyellowbutton;
		activepallette = pal_lrgbluebutton;
	}


	for( i = 0; i < 8; i++ )
	{
		if ( i == activebutton)
		{
			pallette = activepallette;
		} else	{
			pallette = basepallette;
		}
		
		for( y = 0; y < 4; y++ )
		{
			for( x = 0; x < 4; x++ )
			{
				tile = tile_buttontiles + midbuttonmap[buttoncycle];
				buttoncycle++;
				bg2map[(x + (i * 4)) + (y + (yoffset * 4)) * 32] = tile | (pallette << 12);
			}
		}
		buttoncycle = 0;
	}
	
}


// Grid Button Functions

void drawgridbutton(int xoffset, int yoffset, int buttonval) {

	int palref = 0;

	if (buttonval == 0) {
		palref = pal_graygridbutton;
	}
	if (buttonval == 1) {
		palref = pal_greengridbutton;
	}
	if (buttonval == 2) {
		palref = pal_bluegridbutton;
	}
	if (buttonval == 3) {
		palref = pal_orangegridbutton;
	}
	

    int x, y;
	int buttoncycle = 0;
    for( y = 0; y < 2; y++ )
    {
        for( x = 0; x < 2; x++ )
        {
			int tile = tile_gridbutton + buttoncycle;
			buttoncycle++;
		    bg2map[(x + (xoffset * 2)) + (y + (yoffset * 2)) * 32] = tile | (palref << 12);
        }
    }
}


// Button Display Functions

void drawbigbutton(int xoffset, int yoffset, int colour) {

int pallette;

	if(colour == 0) {pallette = pal_lrgredbutton;}
	if(colour == 1) {pallette = pal_lrggreenbutton;}
	if(colour == 2) {pallette = pal_lrgbluebutton;}
	if(colour == 3) {pallette = pal_lrgyellowbutton;}

    int x, y;
	int buttoncycle = 0;
    for( y = 0; y < 6; y++ )
    {
        for( x = 0; x < 8; x++ )
        {
			int tile = tile_buttontiles + bigbuttonmap[buttoncycle];
			buttoncycle++;
		    bg2map[(x + xoffset) + (y + yoffset) * 32] = tile | (pallette << 12);
        }
    }
}

void drawbiglongbutton(int xoffset, int yoffset, int colour) {

int pallette;

	if(colour == 0) {pallette = pal_lrgredbutton;}
	if(colour == 1) {pallette = pal_lrggreenbutton;}
	if(colour == 2) {pallette = pal_lrgbluebutton;}
	if(colour == 3) {pallette = pal_lrgyellowbutton;}

    int x, y;
	int tile;
	int buttoncycle = 0;
    for( y = 0; y < 4; y++ )
    {
        for( x = 0; x < 8; x++ )
        {
			tile = tile_buttontiles + doublemidbuttonmap[buttoncycle];
			buttoncycle++;
		    bg2map[(x + xoffset) + (y + yoffset) * 32] = tile | (pallette << 12);
        }
    }
}


void drawmidbutton(int xoffset, int yoffset, int colour) {

int pallette;

	if(colour == 0) {pallette = pal_lrgredbutton;}
	if(colour == 1) {pallette = pal_lrggreenbutton;}
	if(colour == 2) {pallette = pal_lrgbluebutton;}
	if(colour == 3) {pallette = pal_lrgyellowbutton;}

    int x, y;
	int tile;
	int buttoncycle = 0;
    for( y = 0; y < 4; y++ )
    {
        for( x = 0; x < 4; x++ )
        {
			tile = tile_buttontiles + midbuttonmap[buttoncycle];
			buttoncycle++;
		    bg2map[(x + xoffset) + (y + yoffset) * 32] = tile | (pallette << 12);
        }
    }
}



// Nav Buttons


void modebuttons(int activmode) {

	int modebutton[8];

	for (int i = 0 ; i < 8 ; i++)
	{
		if (i == activmode) {
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

}


// Curve Drawing Functions


void drawpixel(int x, int y, int colour) {

	pixelbg[x+y*256]= colour | BIT(15);

}


void drawcolumn(int x, int y) {
	
	int line = RGB15(30,30,30);
	int bg = RGB15(0,0,0);

	for (int column = 0; column < 128; column ++) {
		drawpixel(x,column,bg);
	}
	
	drawpixel(x,y,line);
	
}

void drawline(int x) {
	
	int colour = RGB15(30,0,0);

	for (int column = 0; column < 128; column ++) {
		drawpixel(x,column,colour);
	}
	
}



// Text Display Functions


void drawmidnumber(int xoffset, int yoffset, int number) {

	int x, y;
	int numbercycle = 0;
	for( y = 0; y < 2; y++ )
	{
		for( x = 0; x < 2; x++ )
		{
			int tile = (tile_midnumbers + (number * 4))  + numbercycle;
			numbercycle++;
			bg1map[(x + xoffset) + (y + yoffset) * 32] = tile | (pal_brandw << 12);
		}
	}
}

void calcanddispnumber(int xoffset, int yoffset, int number) {


	int hundreds = (number / 100);

	int tens = ((number % 100) / 10);

	int units = (number % 10);
	
	drawmidnumber((xoffset),yoffset,hundreds);
	drawmidnumber((xoffset + 2),yoffset,tens);
	drawmidnumber((xoffset + 4),yoffset,units);

}

void drawbignumber(int xoffset, int yoffset, int number) {

	int x, y;
	int numbercycle = 0;
	for( y = 0; y < 4; y++ )
	{
		for( x = 0; x < 3; x++ )
		{
			int tile = (tile_bignumb0 + (number * 12))  + numbercycle;
			numbercycle++;
			bg1map[(x + xoffset) + (y + yoffset) * 32] = tile | (pal_brandw << 12);
		}
	}
}

void drawarrow(int xoffset, int yoffset, int direction) {

	int x, y;
	int numbercycle = 0;
	for( y = 0; y < 2; y++ )
	{
		for( x = 0; x < 2; x++ )
		{
			int tile = tile_uparrow + (direction * 4) + numbercycle;
			numbercycle++;
			bg1map[(x + xoffset) + (y + yoffset) * 32] = tile | (pal_brandw << 12);
		}
	}
}

void mainnumbers(int xoffset, int yoffset, int colour) {


int tileset;
	
	if(colour == 1) {tileset = tile_bignumb1;}
	if(colour == 2) {tileset = tile_bignumb2;}
	if(colour == 3) {tileset = tile_bignumb3;}
	if(colour == 4) {tileset = tile_bignumb4;}
	if(colour == 5) {tileset = tile_bignumb5;}
	if(colour == 6) {tileset = tile_bignumb6;}
	if(colour == 7) {tileset = tile_bignumb7;}
	if(colour == 8) {tileset = tile_bignumb8;}

    int x, y;
	int buttoncycle = 0;
    for( y = 0; y < 4; y++ )
    {
        for( x = 0; x < 3; x++ )
        {
			int tile = tileset + buttoncycle;
			buttoncycle++;
		    bg1map[(x + xoffset) + (y + yoffset) * 32] = tile | (pal_brandw << 12);
        }
    }
}




// File Browser

void filebrowsescreenbackground() {

    int x, y;
	int buttoncycle = 0;


	for( y = 0; y < 24; y++ )
	{
		for( x = 0; x < 32; x++ )
        {
			int tile = tile_optionsviewtiles + filebrowsescreen[buttoncycle];
			
			buttoncycle++;
		    bg2map[x + y * 32] = tile | (pal_optionsviewtiles << 12);
			bg1map[x + y * 32] = tile_empty;
        }
    }
}


// Setup Screen


void setupscreenbackground() {

    int x, y;
	int buttoncycle = 0;


    for( y = 0; y < 20; y++ )
    {
        for( x = 0; x < 23; x++ )
        {
			int tile = tile_optionsviewtiles + optionsscreen[buttoncycle];
				
			buttoncycle++;
		    bg2map[x + y * 32] = tile | (pal_optionsviewtiles << 12);
        }
    }
}


void drawsetuptext(int dsmienable) {

	iprintf("\x1b[2;3HGlobal  Settings");

	if (dsmienable == 0)
	{
		iprintf("\x1b[14;9HEnable");
		iprintf("\x1b[15;9H  DSMI");
	} else if (dsmienable == 1)
	{
		iprintf("\x1b[14;9H  DSMI");
		iprintf("\x1b[15;9HActive");
	}
	
}




// Options Screen


void optionsscreenbackground(int xval, int yval, int length) {

    int x, y;
	int buttoncycle = 0;
	int xmin = xval;
	int xmax = (xval + length);


    for( y = 0; y < 20; y++ )
    {
        for( x = 0; x < 23; x++ )
        {
			int tile = tile_optionsviewtiles + optionsscreen[buttoncycle];
			
			if ((y == yval) && (x >= xmin) && (x < xmax))
			{
				tile += 1;
			}			
			buttoncycle++;
		    bg2map[x + y * 32] = tile | (pal_optionsviewtiles << 12);
        }
    }
}

void drawkeypad(int xoffset, int yoffset) {
	
	drawarrow(xoffset,yoffset,0);
	drawarrow((xoffset + 2),yoffset,0);
	drawarrow((xoffset + 4),yoffset,0);
	
	drawarrow(xoffset,(yoffset + 4),1);
	drawarrow((xoffset + 2),(yoffset + 4),1);
	drawarrow((xoffset + 4),(yoffset + 4),1);
	
	int buttoncycle = 0;
	int tile;
	for( int y = 0; y < 8; y++ )
    {
        for( int x = 0; x < 8; x++ )
        {
			tile = tile_optionsviewtiles + keypadmap[buttoncycle];
			
			buttoncycle++;
		    bg2map[(x + xoffset -1) + (y + yoffset -1) * 32] = tile | (pal_optionsviewtiles << 12);
        }
    }

}

// Clear Screen Function

void clearbottomscreen() {

	iprintf ("\x1b[2J");
	
    for(int y = 0; y < 24; y++ )
    {
        for(int x = 0; x < 32; x++ )
        {
			int tile = tile_empty;
		    bg2map[x + y * 32] = tile;
			bg1map[x + y * 32] = tile;
        }
    }

}



