#ifndef _GFX_H
#define _GFX_H

#include "stdint.h"
#include "GFX_DataTypes.h"
#include "GFX_Driver_FRDM-OLED_NHD-2.712864.h"

#include "GFX_FONT_5x7.h"
#ifndef	GFX_DRIVER_PRESENT

#error "It doesn't appear that a GFX driver is present"

#endif


#define GFX_SPACES_PER_TAB                                        4
#define GFX_MAX_STRING_LEN                                        64
#define GFX_MAX_PRINTF_BUF_LENGTH                                 64


#define INLINE_GET_ImagePlane_BYTE
#define INLINE_ImagePlane_PUT
#define INLINE_ImagePlane_GET
#define INLINE_PUT_PIXEL
#define INLINE_GET_PIXEL


extern GFX_ImagePlane GFX_BackBuffer;

void GFX_InitImagePlane(GFX_ImagePlane   *BP, uint16_t SizeX,uint16_t SizeY,uint8_t *ImagePlaneStorage);


#ifndef INLINE_ImagePlane_PUT
void GFX_ImagePlane_Put(GFX_ImagePlane   *BP, uint16_t X,uint16_t Y, uint8_t Value);
#endif

#ifndef INLINE_ImagePlane_GET
uint8_t GFX_ImagePlane_Get(GFX_ImagePlane   *BP, uint16_t X,uint16_t Y);
#endif

#ifndef INLINE_PUT_PIXEL
void GFX_PutPixel(GFX_ImagePlane *Image, int16_t x, int16_t y, uint8_t PixelState);
#endif

#ifndef INLINE_GET_PIXEL
uint8_t GFX_GetPixel(GFX_ImagePlane *Image, int16_t x, int16_t y);
#endif

#ifndef INLINE_GET_ImagePlane_BYTE
uint8_t GFX_ImagePlane_GetByte(GFX_ImagePlane   *BP, uint16_t X,uint16_t Y);
#endif


void	GFX_ImagePlane_Clear(GFX_ImagePlane   *BP);
uint8_t GFX_GetBitFromArray(uint8_t *DataArray,uint16_t Bit);
void 	GFX_DrawHline(GFX_ImagePlane *Image, int16_t XStart, int16_t XStop, int16_t Y, uint8_t PixelState);
void 	GFX_DrawVline(GFX_ImagePlane *Image, int16_t YStart, int16_t YStop, int16_t X, uint8_t PixelState);
void 	GFX_DrawLine(GFX_ImagePlane *Image, int16_t X1,int16_t Y1, int16_t X2,int16_t Y2, uint8_t PixelState);
void 	GFX_DrawFilledBox(GFX_ImagePlane *Image, GFXDisplayBox *Box, uint8_t PixelState);
void 	GFX_DrawBox(GFX_ImagePlane *Image, GFXDisplayBox *Box, uint8_t PixelState);


int16_t GFX_DrawCharacter(GFX_ImagePlane *Image,uint8_t Character,int16_t StartX, int16_t StartY,  const GFX_Font *MyFont,uint8_t PixelState);
int16_t GFX_GetStringWidth(char *String,const GFX_Font *MyFont);
int16_t GFX_DrawString(GFX_ImagePlane *Image,char *String,int16_t StartX, int16_t StartY,   const GFX_Font *MyFont,uint8_t PixelState);
void    GFX_DrawHorizontalCenteredString(GFX_ImagePlane *Image,char *String,int16_t StartY, const GFX_Font *MyFont,uint8_t PixelState);
void    GFX_DrawListPrimitive(GFX_ImagePlane *Image,GFXListPrimitive *LP ,uint8_t PixelState);
void	GFX_DrawScaledListPrimitive(GFX_ImagePlane *Image,GFXListPrimitive *LP , FIXED_7_8 Scale, uint8_t PixelState);
void 	GFX_DrawRotatedListPrimitive(GFX_ImagePlane *Image,GFXListPrimitive *LP , uint8_t Angle, uint8_t PixelState);
void 	GFX_DrawScaledRotatedListPrimitive(GFX_ImagePlane *Image,GFXListPrimitive *LP , uint8_t Angle,FIXED_7_8 Scale, uint8_t PixelState);
void 	GFX_Blit(GFX_ImagePlane *Destination,const GFX_ImagePlane *Sprite,int16_t x,int16_t y, uint8_t PixelState);
void 	GFX_ScaledBlit(GFX_ImagePlane *Destination,const GFX_ImagePlane *Sprite,int16_t x,int16_t y, float ScaleFactor, uint8_t PixelState);
void 	GFX_ScaledDrawString(GFX_ImagePlane *Image,char *String,int16_t StartX, int16_t StartY,  const GFX_Font *MyFont,float ScaleFactor ,uint8_t PixelState);

int16_t GFX_ScaledDrawCharacter(GFX_ImagePlane *Image,uint8_t Character,int16_t StartX, int16_t StartY,  const GFX_Font *MyFont,float ScaleFactor,uint8_t PixelState);
void GFX_DrawCircle(GFX_ImagePlane *RC, int16_t x0, int16_t y0, int16_t radius, uint8_t PixelState);
void GFX_ScaledDrawString(GFX_ImagePlane *Image,char *String,int16_t StartX, int16_t StartY, const GFX_Font *MyFont,float ScaleFactor ,uint8_t PixelState);
int16_t  GFX_DrawString_CustomSpacing(GFX_ImagePlane *Image,char *String,int16_t StartX, int16_t StartY,  const GFX_Font *MyFont,uint8_t PixelState,uint16_t Spacing);
void GFX_ScaledDrawString_CustomSpacing(GFX_ImagePlane *Image,char *String,int16_t StartX, int16_t StartY,  const GFX_Font *MyFont,float ScaleFactor ,uint8_t PixelState,uint16_t Spacing);
void GFX_DrawHorizontalCenteredString_CustomSpacing(GFX_ImagePlane *Image,char *String,int16_t StartY, const GFX_Font *MyFont,uint8_t PixelState,uint16_t Spacing);
int16_t GFX_Draw_DOT_FACTORY_Character(GFX_ImagePlane *Image,uint8_t Character,int16_t StartX, int16_t StartY, FONT_INFO *MyFont,uint8_t PixelState);
void GFX_Draw_DOT_FACTORY_String(GFX_ImagePlane *Image,char *String,int16_t StartX, int16_t StartY, FONT_INFO *MyFont,uint8_t PixelState);
GFX_ImagePlane *GFX_RenderQRCode(const char *FormatString,...);

int16_t   GFX_printf(GFX_ImagePlane *Image,int16_t StartX, int16_t StartY, const GFX_Font *MyFont, uint8_t PixelState,uint16_t CharacterSpacing, const char *FormatString,...);
int16_t GFX_DrawRightAlignedCenteredString_CustomSpacing(GFX_ImagePlane *Image,char *String,int16_t StartY, const GFX_Font *MyFont,uint8_t PixelState,uint16_t CustomCharacterSpacing,int16_t RightSideBuffer);

#ifdef INLINE_ImagePlane_PUT
static inline void GFX_ImagePlane_Put(GFX_ImagePlane   *BP, uint16_t X,uint16_t Y, uint8_t Value)
{
    uint16_t Offset;
    uint8_t Mask;
    uint16_t MemWidthInBytes;
    //Properly Implement Rounding for odd bit plane sizes
    MemWidthInBytes  = (BP->SizeX)>>3;

    if(BP->SizeX &0x7)
        MemWidthInBytes++;

    Offset = (Y * (MemWidthInBytes)) + (X>>3);
    Mask = 0x01 << (X & 0x07);

    if(Value)
        {
            BP->ImagePlaneSpace[Offset] |= Mask;
        }
    else
        {
            BP->ImagePlaneSpace[Offset] &= ~Mask;
        }
}
#endif

#ifdef INLINE_ImagePlane_GET
static uint8_t GFX_ImagePlane_Get(GFX_ImagePlane   *BP, uint16_t X,uint16_t Y)
{
    uint32_t Offset;
    uint8_t Mask;
    uint32_t MemWidthInBytes;
    //Properly Implement Rounding for odd bit plane sizes
    MemWidthInBytes  = (BP->SizeX)>>3;

    if(BP->SizeX & 0x7)
        MemWidthInBytes++;

    Offset = (Y * MemWidthInBytes) + (X>>3);
    Mask = 0x01 << (X & 0x07);

    if((BP->ImagePlaneSpace[Offset])&Mask)
        {
            return TRUE;
        }
    else
        {
            return FALSE;
        }
}
#endif

#ifdef INLINE_PUT_PIXEL
static inline void GFX_PutPixel(GFX_ImagePlane *Image, int16_t x, int16_t y, uint8_t PixelState)
{
    if((x<Image->SizeX) && (y<Image->SizeY) && (x>=0) && (y>=0))
        {
            GFX_ImagePlane_Put(Image,x,y,PixelState);
        }
}
#endif

#ifdef INLINE_GET_PIXEL
static inline uint8_t GFX_GetPixel(GFX_ImagePlane *Image, int16_t x, int16_t y)
{
    uint8_t PixelColor = 0;

    if((x<Image->SizeX) && (y<Image->SizeY) && (x>=0) && (y>=0))
        {
            PixelColor = GFX_ImagePlane_Get(Image,x,y);
        }

    return PixelColor;
}
#endif

#ifdef INLINE_GET_ImagePlane_BYTE
static inline  uint8_t GFX_ImagePlane_GetByte(GFX_ImagePlane   *BP, uint16_t X,uint16_t Y)
{
    uint32_t Offset;
    uint32_t MemWidthInBytes;

    if(X > BP->SizeX)  //Make sure we aren't asking for a chunk greater than the width of one row
        {
            X = (BP->SizeX);
        }

    X &= ~((uint16_t)0x7); // Make sure we are on a byte boundary
    //Properly Implement Rounding for odd bit plane sizes
    MemWidthInBytes  = (BP->SizeX)>>3;

    if(BP->SizeX & 0x7)
        MemWidthInBytes++;

    Offset = (Y * MemWidthInBytes) + (X>>3);
    return BP->ImagePlaneSpace[Offset];
}
#endif


extern const FIXED_1_14 Fixed_1_14_SineTable [256];
extern const FIXED_7_8 Fixed_7_8_SineTable [256];

#define GET_INT_FROM_FIXED_7_8(A)				(int16_t)(A>>8)
#define INT_TO_FIXED_7_8(A)					((FIXED_7_8)(A)<<8)
#define GET_FRAC_FROM_FIXED_7_8(A)				(FIXED_7_8)(A&0xFF)

#define GET_INT_FROM_FIXED_9_6_(A)				(int16_t)(A>>6)
#define INT_TO_FIXED_9_6(A)					((FIXED_9_6)(A)<<6)
#define GET_FRAC_FROM_FIXED_6_9(A)				(FIXED_9_6)(A&0x3F)

FIXED_7_8 GET_INT_FROM_FIXED_7_8_ROUND_TO_NEAREST(FIXED_7_8 A);
FIXED_7_8 FLOAT_TO_FIXED_7_8(float A);
int16_t FMul_15_0to0_15(int16_t A,int16_t B);
int16_t FMul_7_8to0_15(int16_t A,int16_t B);
int16_t FMul_15_0to7_8(int16_t A,int16_t B);
int16_t FMul_15_0to1_14(int16_t A,int16_t B);
int16_t ABS(int16_t A);



#endif


