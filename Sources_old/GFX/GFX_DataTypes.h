#include "stdint.h"

#ifndef GFX_DATA_TYPES_H
#define GFX_DATA_TYPES_H

#define uint_8 uint8_t //needed for out of the box DOT factory compatibility

typedef int16_t FIXED_7_8;
typedef int16_t FIXED_9_6;

typedef int16_t FIXED_0_15;
typedef int16_t FIXED_1_14;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define GFX_IMAGE_PLANE_TYPE_1BPP	0
#define GFX_IMAGE_PLANE_TYPE_8BPP	1


typedef struct
{
    uint8_t *ImagePlaneSpace;
    uint16_t SizeX; // must be a uint8_t aligned
    uint16_t SizeY;

} GFX_ImagePlane;


typedef struct
{
    int16_t X;
    int16_t Y;
} GFXDisplayPoint;

typedef struct
{
    GFXDisplayPoint P1;
    GFXDisplayPoint P2;
} GFXDisplayBox;


typedef struct
{
    int16_t X;
    int16_t Y;
} GFXRelativePoint;

typedef struct
{
    GFXDisplayPoint Center;
    uint8_t NumPoints;
    uint8_t DrawMode;
    const GFXRelativePoint *PointList;
} GFXListPrimitive;


// This structure describes a single character's display information
typedef struct
{
    const uint8_t widthBits;					// width, in bits (or pixels), of the character
    const uint16_t offset;					// offset of the character's bitmap, in bytes, into the the FONT_INFO's data array
} FONT_CHAR_INFO;

// Describes a single font
typedef struct
{
    const uint8_t 			heightPixels;	// height, in pixels of the character.  This must be enabled in the setup of dot factory
    const uint8_t 			startChar;		// the first character in the font (e.g. in charInfo and data)
    const uint8_t 			endChar;		// the last character in the font
    const uint8_t			spacePixels;	// number of pixels that a space character takes up
    const FONT_CHAR_INFO	*charInfo;		// pointer to array of char information
    const uint8_t 			*data;			// pointer to generated array of character visual representation

} FONT_INFO;

typedef struct
{
    const GFX_ImagePlane *CharacterSprites[0x80-0x20];
    const uint8_t  PixelsPerSpace;
    const uint8_t  SpacesPerTab;
    const uint8_t   InterCharacterSpacing;
    const uint8_t Padding;
} GFX_Font;


#define GFX_LIST_PRIMITIVE_CONNECTED                               0x01
#define GFX_LIST_PRIMITIVE_DISCONNECTED                            0x02
#define GFX_LIST_PRIMITIVE_CLOSED								   0x03
#define GFX_LIST_PRIMITIVE_CONNECTED_YFLIPPED			  			 0x04
#define GFX_LIST_PRIMITIVE_DISCONNECTED_YFLIPPED		  			 0x05
#define GFX_LIST_PRIMITIVE_CLOSED_YFLIPPED			  				 0x06


#define GFX_PIXEL_ON     1
#define GFX_PIXEL_OFF    0



#endif
