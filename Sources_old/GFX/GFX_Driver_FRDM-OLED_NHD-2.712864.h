#include "GFX_DataTypes.h"

#ifndef GFX_DRIVER_CFAG12232_H_
#define GFX_DRIVER_CFAG12232_H_


//LCD GFX Driver Configuration.
// The Symbols below must be defined for the GFX library to work.

#define GFX_DRIVER_PRESENT				//This is must be define for the main GFX library to compile
#define GFX_PHYSICAL_SCREEN_SIZE_X	((uint16_t)128)		//This is the actual X and Y size of the physical screen in *pixels*
#define GFX_PHYSICAL_SCREEN_SIZE_Y	((uint16_t)64)


#define GFX_BACKBUFFER_SIZE_X           ((((GFX_PHYSICAL_SCREEN_SIZE_X+8)>>3))<<3)		//This must be *at least* the physical screen size *rounded up* to the next multiple of 8
//to ensure each line in the backbuffer is on a byte boundary.  This is required for the
//bit manipulation functions to work correctly

#define GFX_BACKBUFFER_SIZE_Y	GFX_PHYSICAL_SCREEN_Y		//This should always be the same as GFX_PHYSICAL_SCREEN_Y

//These are the prototypes for the GFX HAL
void GFX_InitPhysicalScreen();
void GFX_DumpImagePlaneToPhysicalScreen(GFX_ImagePlane *Image);
void GFX_Init();
void GFX_PowerUpScreen();
void GFX_PowerDownScreen();

void SSD1325_ScrollLeft();
void SSD_WriteColumn(uint8_t * Data);
void SSD1325_ScrollRight();
void SSD1325_SetColumnStartEnd(uint8_t Start, uint8_t End);
void SSD1325_SetRemap(uint8_t Setting);

#endif
