#include "stdint.h"


#ifndef eGFX_DATA_TYPES_H
#define eGFX_DATA_TYPES_H


#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


typedef int16_t FIXED_7_8;
typedef int16_t FIXED_9_6;

typedef int16_t FIXED_0_15;
typedef int16_t FIXED_1_14;


#define eGFX_IMAGE_PLANE_1BPP		 0
#define eGFX_IMAGE_PLANE_4BPP		 1
#define eGFX_IMAGE_PLANE_8BPP		 2
#define eGFX_IMAGE_PLANE_16BPP_565    3

#define eGFX_IMAGE_PLANE_24BPP    4
#define eGFX_IMAGE_PLANE_32BPP    5



typedef struct
{
    uint8_t Type;
    uint8_t *Data;
    uint16_t SizeX;
    uint16_t SizeY;

} eGFX_ImagePlane;

typedef struct
{
    int16_t X;
    int16_t Y;
} eGFX_Point;

typedef struct
{
    float X; 	
    float Y;  
} eGFX_PointF;

typedef struct
{
    eGFX_Point P1;
    eGFX_Point P2;
} eGFX_Box;

typedef struct
{
    eGFX_Point Center;
    uint8_t NumPoints;
    uint8_t DrawMode;
    const eGFX_Point *PointList;

} eGFX_ListPrimitive;



#define eGFX_LIST_PRIMITIVE_CONNECTED                              0x01
#define eGFX_LIST_PRIMITIVE_DISCONNECTED                           0x02
#define eGFX_LIST_PRIMITIVE_CLOSED								   0x03
#define eGFX_LIST_PRIMITIVE_CONNECTED_YFLIPPED			 		   0x04
#define eGFX_LIST_PRIMITIVE_DISCONNECTED_YFLIPPED		   		   0x05
#define eGFX_LIST_PRIMITIVE_CLOSED_YFLIPPED			   			   0x06

typedef struct
{
    const eGFX_ImagePlane *CharacterSprites[0x80-0x20];
  
	//Some TT Metrics
	int32_t Ascent; //Ascent scaled up by 65536 to store fractional value		(Q15.16)
	int32_t Descent; //Descent scaled up by 65536 to store fractional value		(Q15.16)
	int32_t LineSpacing; //Linespacing scaled up by 65536 to store fractional value  (Q15.16)
	
	uint8_t  SpacesPerTab;
    uint8_t  InterCharacterSpacing;
   
	uint8_t  Padding[2];

} eGFX_Font;


#define eGFX_PIXEL_ON     0xFFFFFFFF
#define eGFX_PIXEL_OFF    0

typedef uint32_t eGFX_PixelState;

/***
*       ____  _     _           _
*      / __ \| |   (_)         | |
*     | |  | | |__  _  ___  ___| |_ ___
*     | |  | | '_ \| |/ _ \/ __| __/ __|
*     | |__| | |_) | |  __/ (__| |_\__ \
*      \____/|_.__/| |\___|\___|\__|___/
*                 _/ |
*                |__/
*/

typedef struct 
{
	uint8_t Type;
	uint8_t State;
	
}	eGFX_ObjectHeader;

#define eGFX_OBJECT_STATE_ACTIVE								1
#define eGFX_OBJECT_STATE_ANIMATION_ACTIVE								2
#define eGFX_OBJECT_STATE_INACTIVE										0

/***
*                     _                 _                ____  _     _           _
*         /\         (_)               | |              / __ \| |   (_)         | |
*        /  \   _ __  _ _ __ ___   __ _| |_ ___  _ __  | |  | | |__  _  ___  ___| |_ ___
*       / /\ \ | '_ \| | '_ ` _ \ / _` | __/ _ \| '__| | |  | | '_ \| |/ _ \/ __| __/ __|
*      / ____ \| | | | | | | | | | (_| | || (_) | |    | |__| | |_) | |  __/ (__| |_\__ \
*     /_/    \_\_| |_|_|_| |_| |_|\__,_|\__\___/|_|     \____/|_.__/| |\___|\___|\__|___/
*                                                                  _/ |
*                                                                 |__/
*/



#define eGFX_OBJECT_POINTF_BISECT_ANIMATOR								0xF0
#define eGFX_OBJECT_POINTF_LINEAR_ANIMATOR								0xF1

#define eGFX_OBJECT_SCALARF_BISECT_ANIMATOR								0xF2
#define eGFX_OBJECT_SCALARF_LINEAR_ANIMATOR								0xF3




typedef struct
{
	uint8_t Type;
	uint8_t State;
	eGFX_PointF Target;
	eGFX_PointF *Current;
	uint32_t CurrentStep;
	uint32_t LastStep;
	float FractionalBisect; //The amount of percentage of the distance the point should move each step. must between a number between zero and 1

} eGFX_PointF_BisectAnimator;

typedef struct
{
	uint8_t Type;
	uint8_t State;
	eGFX_PointF Target;
	eGFX_PointF *Current;
	uint32_t CurrentStep;
	uint32_t LastStep;
	float IncrementX;
	float IncrementY;

} eGFX_PointF_LinearAnimator;


typedef struct
{
	uint8_t Type;
	uint8_t State;
	float Target;
	float *Current;
	uint32_t CurrentStep;
	uint32_t LastStep;
	float Increment; //Increment we should make each step

} eGFX_ScalarF_LinearAnimator;


typedef struct
{
	uint8_t Type;
	uint8_t State;
	float Target;
	float *Current;
	uint32_t CurrentStep;
	uint32_t LastStep;
	float FractionalBisect; //The amount of percentage of the distance the point should move each step. must between a number between zero and 1

} eGFX_ScalarF_BisectAnimator;

/***
*       _____                 _     _         ____  _     _           _
*      / ____|               | |   (_)       / __ \| |   (_)         | |
*     | |  __ _ __ __ _ _ __ | |__  _  ___  | |  | | |__  _  ___  ___| |_ ___
*     | | |_ | '__/ _` | '_ \| '_ \| |/ __| | |  | | '_ \| |/ _ \/ __| __/ __|
*     | |__| | | | (_| | |_) | | | | | (__  | |__| | |_) | |  __/ (__| |_\__ \
*      \_____|_|  \__,_| .__/|_| |_|_|\___|  \____/|_.__/| |\___|\___|\__|___/
*                      | |                              _/ |
*                      |_|                             |__/
*/

#define eGFX_OBJECT_TEXTF												0x0000
#define eGFX_OBJECT_SHADED_TEXTF										0x0001
#define eGFX_OBJECT_LINEF												0x0002
#define eGFX_OBJECT_SPRITEF												0x0003
#define eGFX_OBJECT_SCALED_SPRITEF										0x0004
#define eGFX_OBJECT_BOXF												0x0005
#define eGFX_OBJECT_CIRCLEF												0x0006


typedef struct
{
	uint8_t Type;
	uint8_t State;
	eGFX_PointF Position;
	char *String;
	eGFX_Font *Font;
	
} eGFX_Obj_TextF;

typedef struct
{
	uint8_t Type;
	uint8_t State;
	eGFX_PointF Position;
	char *String;
	eGFX_Font *Font;
	float Shading;

} eGFX_Obj_ShadedTextF;



typedef struct
{
	uint8_t Type;
	uint8_t State;
	eGFX_PointF Start;
	eGFX_PointF End;
	eGFX_PixelState PixelState; 
} eGFX_Obj_LineF;

#define eGFX_CIRCLEF_RENDER_OPTION_OUTLINE	0x00
#define eGFX_CIRCLEF_RENDER_OPTION_FILLED	0x01

typedef struct
{
	uint8_t Type;
	uint8_t State;
	eGFX_PointF Position;
	float Radius;
	eGFX_PixelState PixelState; 
	uint8_t RenderOption;
} eGFX_Obj_CircleF;


typedef struct
{
	uint8_t Type;
	uint8_t State;
	eGFX_PointF Position;
	eGFX_ImagePlane * Sprite;
	
} eGFX_Obj_SpriteF;


typedef struct
{
	uint8_t Type;
	uint8_t State;
	
	eGFX_PointF Position;
	eGFX_ImagePlane * Sprite;

	float Scale;

} eGFX_Obj_ScaledSpriteF;


#define eGFX_BOXF_RENDER_OPTION_OUTLINE	0x00
#define eGFX_BOXF_RENDER_OPTION_FILLED	0x01

typedef struct
{
	uint8_t Type;
	uint8_t State;

	eGFX_PointF P1;
	eGFX_PointF P2;

	eGFX_PixelState PixelState;

	uint8_t BoxF_RenderOption;

} eGFX_Obj_BoxF;


#endif
