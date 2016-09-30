
#ifdef __cplusplus
extern "C" {
#endif


#include <stdlib.h>

#include "eGFX_DataTypes.h"


#ifndef _eGFX_H__
#define _eGFX_H__

#define eGFX_MAX_ANIMATORS										  32
#define eGFX_MAX_OBJECTS										  64

#define eGFX_SPACES_PER_TAB                                        4
#define eGFX_MAX_STRING_LEN                                        64
#define eGFX_MAX_PRINTF_BUF_LENGTH                                 64

#define eGFX_API_LEVEL											   1
#define eGFX_REVISION											   1

#define eGFX_CALCULATE_1BPP_BUFFER_ROW_BYTE_SIZE(x)			 ((x+7)>>3) //We add 8 to round up to the next even byte boundary
#define eGFX_CALCULATE_4BPP_BUFFER_ROW_BYTE_SIZE(x)			 ((x+1)>>1) //We add 1 to round up to the next even byte boundary
#define eGFX_CALCULATE_8BPP_BUFFER_ROW_BYTE_SIZE(x)			 (x)
#define eGFX_CALCULATE_16BPP_BUFFER_ROW_BYTE_SIZE(x)		 (x*2)
#define eGFX_CALCULATE_24BPP_BUFFER_ROW_BYTE_SIZE(x)	 	 (x*3)
#define	eGFX_CALCULATE_32BPP_BUFFER_ROW_BYTE_SIZE(x)	 	 (x*4)

#define eGFX_CALCULATE_1BPP_IMAGE_STORAGE_SPACE_SIZE(x,y)    (eGFX_CALCULATE_1BPP_BUFFER_ROW_BYTE_SIZE(x)	* y)
#define eGFX_CALCULATE_4BPP_IMAGE_STORAGE_SPACE_SIZE(x,y)    (eGFX_CALCULATE_4BPP_BUFFER_ROW_BYTE_SIZE(x)	* y)
#define eGFX_CALCULATE_8BPP_IMAGE_STORAGE_SPACE_SIZE(x,y)    (eGFX_CALCULATE_8BPP_BUFFER_ROW_BYTE_SIZE(x)	* y)
#define eGFX_CALCULATE_16BPP_IMAGE_STORAGE_SPACE_SIZE(x,y)   (eGFX_CALCULATE_16BPP_BUFFER_ROW_BYTE_SIZE(x)	* y)
#define eGFX_CALCULATE_24BPP_IMAGE_STORAGE_SPACE_SIZE(x,y)   (eGFX_CALCULATE_24BPP_BUFFER_ROW_BYTE_SIZE(x)	* y)
#define eGFX_CALCULATE_32BPP_IMAGE_STORAGE_SPACE_SIZE(x,y)   (eGFX_CALCULATE_32BPP_BUFFER_ROW_BYTE_SIZE(x)	* y)

#define ROUND_TO_INT16_T(x)	((int16_t)(x + 0.5f))

extern const FIXED_1_14 eGFX_Fixed_1_14_SineTable [256];
extern const FIXED_7_8 eGFX_Fixed_7_8_SineTable[256];

#define eGFX_GET_INT_FROM_FIXED_7_8(A)				(int16_t)(A>>8)
#define eGFX_INT_TO_FIXED_7_8(A)					((FIXED_7_8)(A)<<8)
#define eGFX_GET_FRAC_FROM_FIXED_7_8(A)				(FIXED_7_8)(A&0xFF)

#define eGFX_GET_INT_FROM_FIXED_9_6_(A)				(int16_t)(A>>6)
#define eGFX_INT_TO_FIXED_9_6(A)					((FIXED_9_6)(A)<<6)
#define eGFX_GET_FRAC_FROM_FIXED_6_9(A)				(FIXED_9_6)(A&0x3F)

FIXED_7_8 eGFX_GET_INT_FROM_FIXED_7_8_ROUND_TO_NEAREST(FIXED_7_8 A);
FIXED_7_8 eGFX_FLOAT_TO_FIXED_7_8(float A);
int16_t eGFX_FMul_15_0to0_15(int16_t A, int16_t B);
int16_t eGFX_FMul_7_8to0_15(int16_t A, int16_t B);
int16_t eGFX_FMul_15_0to7_8(int16_t A,int16_t B);
int16_t eGFX_FMul_15_0to1_14(int16_t A,int16_t B);
int16_t eGFX_ABS(int16_t A);



/***
 *      _____ _          _    ____                       _   _
 *     |  __ (_)        | |  / __ \                     | | (_)
 *     | |__) |__  _____| | | |  | |_ __   ___ _ __ __ _| |_ _  ___  _ __  ___
 *     |  ___/ \ \/ / _ \ | | |  | | '_ \ / _ \ '__/ _` | __| |/ _ \| '_ \/ __|
 *     | |   | |>  <  __/ | | |__| | |_) |  __/ | | (_| | |_| | (_) | | | \__ \
 *     |_|   |_/_/\_\___|_|  \____/| .__/ \___|_|  \__,_|\__|_|\___/|_| |_|___/
 *                                 | |
 *                                 |_|
 */
void eGFX_PutPixel(eGFX_ImagePlane *Image,
                   int16_t x,
                   int16_t y,
                   eGFX_PixelState PS);

eGFX_PixelState eGFX_GetPixel(const eGFX_ImagePlane *Image,
                         int16_t x,
                         int16_t y);




/***
 *      _____      _           _ _   _              ____                       _   _
 *     |  __ \    (_)         (_) | (_)            / __ \                     | | (_)
 *     | |__) | __ _ _ __ ___  _| |_ ___   _____  | |  | |_ __   ___ _ __ __ _| |_ _  ___  _ __  ___
 *     |  ___/ '__| | '_ ` _ \| | __| \ \ / / _ \ | |  | | '_ \ / _ \ '__/ _` | __| |/ _ \| '_ \/ __|
 *     | |   | |  | | | | | | | | |_| |\ V /  __/ | |__| | |_) |  __/ | | (_| | |_| | (_) | | | \__ \
 *     |_|   |_|  |_|_| |_| |_|_|\__|_| \_/ \___|  \____/| .__/ \___|_|  \__,_|\__|_|\___/|_| |_|___/
 *                                                       | |
 *                                                       |_|
 */

 
void eGFX_DrawHline(eGFX_ImagePlane *Image,
                    int16_t XStart,
                    int16_t XStop,
                    int16_t Y,
                    eGFX_PixelState PS);

void eGFX_DrawFilledBox(eGFX_ImagePlane *Image,
                        eGFX_Box *Box,
                        eGFX_PixelState PS);

void eGFX_DrawBox(eGFX_ImagePlane *Image,
                  eGFX_Box *Box,
                  eGFX_PixelState PS);

void eGFX_DrawVline(eGFX_ImagePlane *Image,
                    int16_t YStart,
                    int16_t YStop,
                    int16_t X,
                    eGFX_PixelState PS);

void eGFX_DrawLine(eGFX_ImagePlane *Image,
                   int16_t X1,
                   int16_t Y1,
                   int16_t X2,
                   int16_t Y2,
                   eGFX_PixelState PS);

void eGFX_DrawListPrimitive(eGFX_ImagePlane *Image,
                            eGFX_ListPrimitive *LP ,
                            eGFX_PixelState PS);

void eGFX_DrawScaledListPrimitive(eGFX_ImagePlane *Image,
                                  eGFX_ListPrimitive *LP ,
                                  FIXED_7_8 Scale,
                                  eGFX_PixelState PS);

void eGFX_DrawRotatedListPrimitive(eGFX_ImagePlane *Image,
                                   eGFX_ListPrimitive *LP ,
                                   uint8_t Angle,
                                   eGFX_PixelState PS);

void eGFX_DrawScaledRotatedListPrimitive(eGFX_ImagePlane *Image,
        eGFX_ListPrimitive *LP ,
        uint8_t Angle,
        FIXED_7_8 Scale,
        eGFX_PixelState PS);

void eGFX_DrawCircle(eGFX_ImagePlane *RC,
                     int16_t x0,
                     int16_t y0,
                     int16_t radius,
                     eGFX_PixelState PS);

void eGFX_DrawFilledCircle(eGFX_ImagePlane *RC,
	int16_t x0,
	int16_t y0,
	int16_t radius,
	eGFX_PixelState PS);



/***
 *       _____ _        _                ____                       _   _
 *      / ____| |      (_)              / __ \                     | | (_)
 *     | (___ | |_ _ __ _ _ __   __ _  | |  | |_ __   ___ _ __ __ _| |_ _  ___  _ __  ___
 *      \___ \| __| '__| | '_ \ / _` | | |  | | '_ \ / _ \ '__/ _` | __| |/ _ \| '_ \/ __|
 *      ____) | |_| |  | | | | | (_| | | |__| | |_) |  __/ | | (_| | |_| | (_) | | | \__ \
 *     |_____/ \__|_|  |_|_| |_|\__, |  \____/| .__/ \___|_|  \__,_|\__|_|\___/|_| |_|___/
 *                               __/ |        | |
 *                              |___/         |_|
 */

int16_t eGFX_ScaledDrawCharacter(eGFX_ImagePlane *Image,
                                 int16_t StartX,
                                 int16_t StartY,
                                 uint8_t Character,
                                 const eGFX_Font *MyFont,
                                 float ScaleFactor);

int16_t eGFX_DrawCharacter(eGFX_ImagePlane *Image,
                           int16_t StartX,
                           int16_t StartY,
                           uint8_t Character,
                           const eGFX_Font *MyFont);

int16_t eGFX_DrawCharacterFaded(eGFX_ImagePlane *Image,
                           int16_t StartX,
                           int16_t StartY,
                           uint8_t Character,
                           const eGFX_Font *MyFont,
                           eGFX_PixelState PS);

int16_t eGFX_DrawCharacterShaded(eGFX_ImagePlane *Image,
									int16_t StartX,
									int16_t StartY,
									uint8_t Character,
									const eGFX_Font *MyFont,
									float Shading);


int16_t eGFX_GetStringWidth(char *String,
                            const eGFX_Font *MyFont);

int16_t eGFX_GetStringWidth_CustomSpacing(char *String,
											const eGFX_Font *MyFont,
											uint16_t Spacing);


void eGFX_DrawHorizontalCenteredString(eGFX_ImagePlane *Image,
                                       int16_t StartY,
                                       char *String,
                                       const eGFX_Font *MyFont);

int16_t eGFX_DrawRightAlignedCenteredString_CustomSpacing(eGFX_ImagePlane *Image,
        int16_t StartY,
        char *String,
        const eGFX_Font *MyFont,
        uint16_t CustomCharacterSpacing,
        int16_t RightSideBuffer);

void eGFX_DrawHorizontalCenteredString_CustomSpacing(eGFX_ImagePlane *Image,
        int16_t StartY,
        char *String,
        const eGFX_Font *MyFont,
        uint16_t Spacing);

void eGFX_ScaledDrawString(eGFX_ImagePlane *Image,
                           int16_t StartX,
                           int16_t StartY,
                           char *String,
                           const eGFX_Font *MyFont,
                           float ScaleFactor);

int16_t  eGFX_DrawStringFaded(eGFX_ImagePlane *Image,
						 char *String,
                         int16_t StartX,
                         int16_t StartY,
                         const eGFX_Font *MyFont,
						eGFX_PixelState PS
                         );


int16_t  eGFX_DrawStringShaded(eGFX_ImagePlane *Image,
								char *String,
								int16_t StartX,
								int16_t StartY,
								const eGFX_Font *MyFont,
								float Shading
								);

void eGFX_ScaledDrawString_CustomSpacing(eGFX_ImagePlane *Image,
        int16_t StartX,
        int16_t StartY,
        char *String,
        const eGFX_Font *MyFont,
        float ScaleFactor ,
        uint16_t Spacing);

int16_t  eGFX_DrawString(eGFX_ImagePlane *Image,
						char * String,
                         int16_t StartX,
                         int16_t StartY,

                         const eGFX_Font *MyFont
                         );

int16_t  eGFX_printf(eGFX_ImagePlane *Image,
                     int16_t StartX,
                     int16_t StartY,
                     const eGFX_Font *MyFont,
                     
                     char *FormatString,...);

int16_t eGFX_DrawString_CustomSpacing(eGFX_ImagePlane *Image,
                                      int16_t StartX,
                                      int16_t StartY,
                                      char *String,
                                      const eGFX_Font *MyFont,uint16_t Spacing);
                                      
int16_t  eGFX_printf_Colored(eGFX_ImagePlane *Image,
                     int16_t StartX,
                     int16_t StartY,
                     const eGFX_Font *MyFont,
                     uint32_t Color,
                     char *FormatString,...);

int16_t  eGFX_printf_HorizontalCentered_Colored(eGFX_ImagePlane *Image,
                                         int16_t StartY,
                                        const eGFX_Font *MyFont,
                                        uint32_t Color,
                                        char *FormatString,...);
/***
*      _____  _                     ____                       _   _
*     |  __ \| |                   / __ \                     | | (_)
*     | |__) | | __ _ _ __   ___  | |  | |_ __   ___ _ __ __ _| |_ _  ___  _ __  ___
*     |  ___/| |/ _` | '_ \ / _ \ | |  | | '_ \ / _ \ '__/ _` | __| |/ _ \| '_ \/ __|
*     | |    | | (_| | | | |  __/ | |__| | |_) |  __/ | | (_| | |_| | (_) | | | \__ \
*     |_|    |_|\__,_|_| |_|\___|  \____/| .__/ \___|_|  \__,_|\__|_|\___/|_| |_|___/
*                                        | |
*                                        |_|
*/

void eGFX_Blit(eGFX_ImagePlane *Destination,
               int16_t x,
               int16_t y,
               const eGFX_ImagePlane *Sprite);

void eGFX_CenteredBlit(eGFX_ImagePlane *Destination,
			int16_t x,
			int16_t y,
			const eGFX_ImagePlane *Sprite);

void eGFX_BlitFaded(eGFX_ImagePlane *Destination,
               int16_t x,
               int16_t y,
               const eGFX_ImagePlane *Sprite,
               eGFX_PixelState PS);

void eGFX_BlitShaded(eGFX_ImagePlane *Destination,
					int16_t x,
					int16_t y,
					const eGFX_ImagePlane *Sprite,
					float Shading);
                    
void eGFX_BlitColored(eGFX_ImagePlane *Destination,
                    int16_t x,
                    int16_t y,
                    const eGFX_ImagePlane *Sprite,
                    uint32_t Color);

void eGFX_ScaledBlit(eGFX_ImagePlane *Destination,
                     int16_t x,
                     int16_t y,
                     const eGFX_ImagePlane *Sprite,
                     float ScaleFactor);

uint8_t eGFX_GetBitFromArray(uint8_t *DataArray,uint16_t Bit);


uint8_t eGFX_ImagePlane_GetByte(eGFX_ImagePlane   *Image,
                                uint16_t X,
                                uint16_t Y);


void eGFX_ImagePlaneInit(eGFX_ImagePlane   *Image, uint8_t * Store, int16_t SizeX, int16_t SizeY, uint8_t Type);

void eGFX_ImagePlane_Clear(eGFX_ImagePlane   *Image);



/***
*                     _                 _                        _____ _____
*         /\         (_)               | |                 /\   |  __ \_   _|
*        /  \   _ __  _ _ __ ___   __ _| |_ ___  _ __     /  \  | |__) || |
*       / /\ \ | '_ \| | '_ ` _ \ / _` | __/ _ \| '__|   / /\ \ |  ___/ | |
*      / ____ \| | | | | | | | | | (_| | || (_) | |     / ____ \| |    _| |_
*     /_/    \_\_| |_|_|_| |_| |_|\__,_|\__\___/|_|    /_/    \_\_|   |_____|
*
*
*/

//Call this in the main loop.   Animators will be stepped at the rate that this is called!
void eGFX_ProcessAnimators();

//Adds an animator object to the list.  returns TRUE if successful.
uint32_t eGFX_AddAnimator(void * Animator);

//Clears all animators from the list
void eGFX_ClearAnimators();

//Gets the number of animator objects in the list
uint32_t eGFX_GetAnimatorCount();

//returns TRUE if all of the animators are complete
uint32_t eGFX_AnimatorsComplete();

void eGFX_PauseAnimators();

void eGFX_StartAnimators();


/***
*      ___     _     _   ___   ___ _            _
*     | _ \___(_)_ _| |_| __| | _ |_)___ ___ __| |_
*     |  _/ _ \ | ' \  _| _|  | _ \ (_-</ -_) _|  _|
*     |_| \___/_|_||_\__|_|   |___/_/__/\___\__|\__|
*
*/

//Initializes  a point bisect animator.  It does not add it to the list
//returns true if successful, false if not
uint32_t eGFX_InitPointF_BisectAnimator(  eGFX_PointF_BisectAnimator *P,
										eGFX_PointF Target,
										eGFX_PointF *Current,
										uint32_t LastStep,			//Maximum Allow Steps before forced convergence
										float FractionalBisect		//The amount of percentage of the distance the point should move each step.   Range is 0 to 1.0.    values of 0 or less will map to 0.5.   Value of 1 or greater will map to 0.5
);

//Initializes and adds a point bisect animator to the list.
//returns true if successful, false if not
uint32_t eGFX_AddPointF_BisectAnimator(   eGFX_PointF_BisectAnimator *P,
										eGFX_PointF Target,
										eGFX_PointF *Current,
										uint32_t LastStep,			//Maximum Allow Steps before forced convergence
										float FractionalBisect //The amount of percentage of the distance the point should move each step.   Range is 0 to 1.0.    values of 0 or less will map to 0.5.   Value of 1 or greater will map to 0.5
										);

void eGFX_ProcessPointF_BisectAnimator(eGFX_PointF_BisectAnimator *P);


/***
*      ___          _          ___   ___ _            _
*     / __| __ __ _| |__ _ _ _| __| | _ |_)___ ___ __| |_
*     \__ \/ _/ _` | / _` | '_| _|  | _ \ (_-</ -_) _|  _|
*     |___/\__\__,_|_\__,_|_| |_|   |___/_/__/\___\__|\__|
*
*/

//Initializes the data struct
uint32_t eGFX_InitScalarF_BisectAnimator(eGFX_ScalarF_BisectAnimator *P,
	float Target,
	float *Current,
	uint32_t LastStep,			//Maximum Allow Steps before forced convergence
	float FractionalBisect);

uint32_t eGFX_AddScalarF_BisectAnimator(eGFX_ScalarF_BisectAnimator *P,
	float Target,
	float *Current,
	uint32_t LastStep,			//Maximum Allow Steps before forced convergence
	float FractionalBisect);


void eGFX_ProcessScalarF_BisectAnimator(eGFX_ScalarF_BisectAnimator *P);




/***
*      ___          _            _    _
*     / __| __ __ _| |__ _ _ _  | |  (_)_ _  ___ __ _ _ _
*     \__ \/ _/ _` | / _` | '_| | |__| | ' \/ -_) _` | '_|
*     |___/\__\__,_|_\__,_|_|   |____|_|_||_\___\__,_|_|
*
*/

//Initializes the data struct
uint32_t eGFX_InitScalarF_LinearAnimator(eGFX_ScalarF_LinearAnimator *P,
										float Target,
										float *Current,
										uint32_t LastStep,			//Maximum Allow Steps before forced convergence
										float Increment
										);



//Initializes and adds a scalarf linear animator and adds it to the list
//returns true if successful, false if not
uint32_t eGFX_AddScalarF_LinearAnimator(eGFX_ScalarF_LinearAnimator *P,
										float Target,
										float *Current,
										uint32_t LastStep,			//Maximum Allow Steps before forced convergence
										float Increment
										);

										
void eGFX_ProcessScalarF_LinearAnimator(eGFX_ScalarF_LinearAnimator *P);


/***
*      ___     _     _   ___   _    _
*     | _ \___(_)_ _| |_| __| | |  (_)_ _  ___ __ _ _ _
*     |  _/ _ \ | ' \  _| _|  | |__| | ' \/ -_) _` | '_|
*     |_| \___/_|_||_\__|_|   |____|_|_||_\___\__,_|_|
*
*/

//Initializes the data struct
uint32_t eGFX_InitPointF_LinearAnimator(eGFX_PointF_LinearAnimator *P,
	eGFX_PointF Target,
	eGFX_PointF *Current,
	uint32_t LastStep,			//Maximum Allow Steps before forced convergence
	float IncrementX,
	float IncrementY
	);



//Initializes and adds a scalarf linear animator and adds it to the list
//returns true if successful, false if not
uint32_t eGFX_AddPointF_LinearAnimator(eGFX_PointF_LinearAnimator *P,
	eGFX_PointF Target,
	eGFX_PointF *Current,
	uint32_t LastStep,			//Maximum Allow Steps before forced convergence
	float IncrementX,
	float IncrementY
	);


void eGFX_ProcessPointF_LinearAnimator(eGFX_PointF_LinearAnimator *P);





										
/***
 *       ____  ____       _ ______ _____ _______            _____ _____ 
 *      / __ \|  _ \     | |  ____/ ____|__   __|     /\   |  __ \_   _|
 *     | |  | | |_) |    | | |__ | |       | |       /  \  | |__) || |  
 *     | |  | |  _ < _   | |  __|| |       | |      / /\ \ |  ___/ | |  
 *     | |__| | |_) | |__| | |___| |____   | |     / ____ \| |    _| |_ 
 *      \____/|____/ \____/|______\_____|  |_|    /_/    \_\_|   |_____|
 *                                                                      
 *                                                                      
 */
		
//Call this when you are ready to draw all the objects on the list
void eGFX_DrawObjects(eGFX_ImagePlane * Image);

//Adds a eGFX object to the list
uint32_t eGFX_AddObject(void * Animator);

//Clears all objects from the list
void eGFX_ClearObjects();

//Gets the number of objects in the object list
uint32_t eGFX_GetAnimatorCount();


/***
*      _____        _   ___
*     |_   _|____ _| |_| __|
*       | |/ -_) \ /  _| _|
*       |_|\___/_\_\\__|_|
*
*/

//Draws a TextF object onto an image Plane
void eGFX_DrawObject_TextF(eGFX_ImagePlane * Image, eGFX_Obj_TextF *TextF_Object);

//Initializes a TextF object
void eGFX_InitObjectTextF(eGFX_Obj_TextF * TF,
							eGFX_PointF Position,
							eGFX_Font *Font,
							char * String
							);	

//Initializes a TextF object and also adds it to the current draw list
void eGFX_AddObjectTextF(eGFX_Obj_TextF * TF,
						eGFX_PointF Position,
						eGFX_Font *Font,
						char * String
						);

/***
*      ___ _            _        _   _____        _   ___
*     / __| |_  __ _ __| |___ __| | |_   _|____ _| |_| __|
*     \__ \ ' \/ _` / _` / -_) _` |   | |/ -_) \ /  _| _|
*     |___/_||_\__,_\__,_\___\__,_|   |_|\___/_\_\\__|_|
*
*/

void eGFX_InitObjectShadedTextF(eGFX_Obj_ShadedTextF * TF,
	eGFX_PointF Position,
	eGFX_Font *Font,
	char * String,
	float Shading
	);


void eGFX_AddObjectShadedTextF(eGFX_Obj_ShadedTextF * TF,
	eGFX_PointF Position,
	eGFX_Font *Font,
	char * String,
	float Shading
	);

void	eGFX_DrawObject_ShadedTextF(eGFX_ImagePlane * Image, eGFX_Obj_ShadedTextF *ShadedTextF_Object);

						
/***
*      _    _          ___
*     | |  (_)_ _  ___| __|
*     | |__| | ' \/ -_) _|
*     |____|_|_||_\___|_|
*
*/

//Initializes a LineF object
void eGFX_InitObjectLineF(eGFX_Obj_LineF * LF,
	eGFX_PointF Start,
	eGFX_PointF End,
	eGFX_PixelState PS
	);


//Initializes a LineF object and also adds it to the current draw list
void eGFX_AddObjectLineF(eGFX_Obj_LineF * LF,
	eGFX_PointF Start,
	eGFX_PointF End,
	eGFX_PixelState PS
	);


//Draws a LineF object onto an image Plane
void	eGFX_DrawObject_LineF(eGFX_ImagePlane * Image, eGFX_Obj_LineF *LineF_Object);

/***
*      ___          _ _       ___
*     / __|_ __ _ _(_) |_ ___| __|
*     \__ \ '_ \ '_| |  _/ -_) _|
*     |___/ .__/_| |_|\__\___|_|
*         |_|
*/

//Initializes a SpriteF object
void eGFX_InitObjectSpriteF(eGFX_Obj_SpriteF * S,
							eGFX_PointF Position,
							eGFX_ImagePlane *	Sprite
							);

//Initializes a SpriteF object and adds it to the object list
void eGFX_AddObjectSpriteF(eGFX_Obj_SpriteF * S,
							eGFX_PointF Position,
							eGFX_ImagePlane *	Sprite
							);

//draws a spritef onto an image plane
void	eGFX_DrawObject_SpriteF(eGFX_ImagePlane * Image, eGFX_Obj_SpriteF *SpriteF_Object);

/***
*      ___          _ _       ___   ___          _        _
*     / __|_ __ _ _(_) |_ ___| __| / __| __ __ _| |___ __| |
*     \__ \ '_ \ '_| |  _/ -_) _|  \__ \/ _/ _` | / -_) _` |
*     |___/ .__/_| |_|\__\___|_|   |___/\__\__,_|_\___\__,_|
*         |_|
*/

//Initializes a scaled sprited object
void eGFX_InitObjectScaledSpriteF(eGFX_Obj_ScaledSpriteF * S,
	eGFX_PointF Position,
	eGFX_ImagePlane *	Sprite,
	float Scale
	);


//Initializes a scaled sprited object and adds it to the object list
void eGFX_AddObjectScaledSpriteF(eGFX_Obj_ScaledSpriteF * S,
	eGFX_PointF Position,
	eGFX_ImagePlane *	Sprite,
	float Scale
	);

//draws a scaled spritef onto an image plane
void	eGFX_DrawObject_ScaledSpriteF(eGFX_ImagePlane * Image, eGFX_Obj_ScaledSpriteF *SpriteF_Object);


/***
*       ___          ___
*      | _ ) _____ _| __|
*      | _ \/ _ \ \ / _|
*      |___/\___/_\_\_|
*
*/


void eGFX_InitObjectBoxF(eGFX_Obj_BoxF * BF,
	eGFX_PointF P1,
	eGFX_PointF P2,
	eGFX_PixelState PS,
	uint8_t BoxF_RenderOption
	);


void eGFX_AddObjectBoxF(eGFX_Obj_BoxF * BF,
	eGFX_PointF P1,
	eGFX_PointF P2,
	eGFX_PixelState PS,
	uint8_t BoxF_RenderOption
	);


void	eGFX_DrawObject_BoxF(eGFX_ImagePlane * Image, eGFX_Obj_BoxF *BoxF_Object);


/***
*       ___ _        _     ___
*      / __(_)_ _ __| |___| __|
*     | (__| | '_/ _| / -_) _|
*      \___|_|_| \__|_\___|_|
*
*/


void eGFX_InitObjectCircleF(eGFX_Obj_CircleF * CF,
	eGFX_PointF Position,
	float Radius,
	eGFX_PixelState PS,
	uint8_t CircleF_RenderOption
	);


void eGFX_AddObjectCircleF(eGFX_Obj_CircleF * CF,
	eGFX_PointF Position,
	float Radius,
	eGFX_PixelState PS,
	uint8_t CircleF_RenderOption
	);

void	eGFX_DrawObject_CircleF(eGFX_ImagePlane * Image, eGFX_Obj_CircleF *CircleF_Object);


#endif


#ifdef __cplusplus
 }
#endif

