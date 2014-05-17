#include "GFX.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

GFX_ImagePlane GFX_BackBuffer;
uint8_t BackBufferStorageSpace[((GFX_BACKBUFFER_SIZE_X)>>3) * GFX_PHYSICAL_SCREEN_SIZE_Y];
char GFX_StringBuf[GFX_MAX_PRINTF_BUF_LENGTH];


void GFX_Init()
{
    GFX_InitImagePlane(&GFX_BackBuffer,  GFX_PHYSICAL_SCREEN_SIZE_X, GFX_PHYSICAL_SCREEN_SIZE_Y, BackBufferStorageSpace);
    GFX_ImagePlane_Clear(&GFX_BackBuffer);
    GFX_InitPhysicalScreen();
#ifdef GFX_ENABLE_QRCODE_SUPPORT
    GFX_InitImagePlane(&QRCodePlane,  GFX_MAX_QRCODE_SIZE, GFX_MAX_QRCODE_SIZE, QRCodeStorageSpace);
#endif
}

#ifndef INLINE_PUT_PIXEL
void GFX_PutPixel(GFX_ImagePlane *Image, int16_t x, int16_t y, uint8_t PixelState)
{
    if((x<Image->SizeX) && (y<Image->SizeY) && (x>=0) && (y>=0))
        {
            GFX_ImagePlane_Put(Image,x,y,PixelState);
        }
}
#endif

#ifndef INLINE_GET_PIXEL
uint8_t GFX_GetPixel(GFX_ImagePlane *Image, int16_t x, int16_t y)
{
    uint8_t PixelColor = 0;

    if((x<Image->SizeX) && (y<Image->SizeY) && (x>=0) && (y>=0))
        {
            PixelColor = GFX_ImagePlane_Get(Image,x,y);
        }

    return PixelColor;
}
#endif

void GFX_DrawHline(GFX_ImagePlane *Image, int16_t XStart, int16_t XStop, int16_t Y, uint8_t PixelState)
{
    int16_t LineStart;
    int16_t LineStop;
    uint16_t i;

    if((Y<Image->SizeY)  && (Y>=0))
        {
            if(XStart>XStop)
                {
                    LineStart = XStop;
                    LineStop = XStart;
                }
            else
                {
                    LineStart = XStart;
                    LineStop = XStop;
                }

            if(LineStart<0)
                {
                    LineStart = 0;
                }

            if(LineStop>Image->SizeX)
                {
                    LineStop = 	Image->SizeX-1;
                }

            if(LineStart == LineStop)
                {
                    GFX_PutPixel(Image,LineStart,Y,PixelState);
                }
            else
                {
                    for(i=LineStart; i<=LineStop ; i++)
                        {
                            GFX_PutPixel(Image,i,Y,PixelState);
                        }
                }
        }
}


void GFX_DrawFilledBox(GFX_ImagePlane *Image, GFXDisplayBox *Box, uint8_t PixelState)
{
    uint16_t i;

    for(i=Box->P1.Y ; i<Box->P2.Y+1 ; i++)
        {
            GFX_DrawHline(Image, Box->P1.X, Box->P2.X,i,PixelState);
        }
}


void GFX_DrawBox(GFX_ImagePlane *Image, GFXDisplayBox *Box, uint8_t PixelState)
{
    GFX_DrawHline(Image, Box->P1.X, Box->P2.X,Box->P1.Y,PixelState);
    GFX_DrawHline(Image, Box->P1.X, Box->P2.X,Box->P2.Y,PixelState);
    GFX_DrawVline(Image, Box->P1.Y, Box->P2.Y,Box->P1.X,PixelState);
    GFX_DrawVline(Image, Box->P1.Y, Box->P2.Y,Box->P2.X,PixelState);
}

void GFX_DrawVline(GFX_ImagePlane *Image, int16_t YStart, int16_t YStop, int16_t X, uint8_t PixelState)
{
    int16_t LineStart;
    int16_t LineStop;
    int16_t i;

    if((X<Image->SizeX) && (X>=0))
        {
            if(YStart>YStop)
                {
                    LineStart = YStop;
                    LineStop = YStart;
                }
            else
                {
                    LineStart = YStart;
                    LineStop = YStop;
                }

            if(LineStart<0)
                {
                    LineStart = 0;
                }

            if(LineStop>Image->SizeY)
                {
                    LineStop = 	Image->SizeY-1;
                }

            for(i=LineStart; i<=LineStop ; i++)
                {
                    GFX_PutPixel(Image,X,i,PixelState);
                }
        }
}
void GFX_DrawLine(GFX_ImagePlane *Image, int16_t X1,int16_t Y1, int16_t X2,int16_t Y2, uint8_t PixelState)
{
    //A simple Implementation of Bresenham's line Algorithm
    int16_t StartX,StopX,StartY,StopY;
    int16_t dX,dY;
    int16_t Y_Numerator;
    int16_t X_Numerator;
    int16_t Y;
    int16_t X;
    int16_t i;
    uint8_t YDir = 0;

    //First Make sure that it is left to right
    //If not them flop them
    if(X2>X1)
        {
            StartX = X1;
            StopX = X2;
            StartY = Y1;
            StopY = Y2;
        }
    else
        {
            StartX = X2;
            StopX = X1;
            StartY = Y2;
            StopY = Y1;
        }

    GFX_PutPixel(Image, StopX,StopY, PixelState);

    if(StopY>=StartY)
        {
            dY = StopY - StartY;
            YDir = 0;
        }
    else
        {
            dY = StartY - StopY;
            YDir = 1;
        }

    dX = StopX - StartX;

    //Now, if the slope is less greater than one,  we need to swap all X/Y operations
    if(dY<=dX)
        {
            //Slope is less than one, proceed at normal and step along the x axis
            Y=StartY;   //start the whole part of the Y value at the starting pixeel.
            X=StartX;
            //We need to start the numerator of the fraction half way through the fraction so evertyhing rounds at
            //fraction midpoint
            Y_Numerator = dX>>1;   //The fraction demonimator is assumeed to be dX

            // out fixed point Y value is  Y + (Y_Numerator / dX)
            //Every time we step the X coordinate by one, we need to step
            //out Y coordinate by dY/dX.  We do this by just adding dY to our
            //numerator.  When the numerator gets bigger than the
            //denomiator, the increment the whole part by one and decrement the numerator
            //by the denominator
            for(i=0; i<dX; i++)
                {
                    GFX_PutPixel(Image,X,Y,PixelState);
                    X++;
                    //Now do all the fractional stuff
                    Y_Numerator += dY;

                    if(Y_Numerator >= dX)
                        {
                            Y_Numerator-=dX;

                            if(StopY > StartY)
                                {
                                    Y++;
                                }
                            else
                                {
                                    Y--;
                                }
                        }
                }
        }
    else
        {
            //Same as before by step along the y axis.
            Y=StartY;
            X=StartX;
            X_Numerator = dY>>1;

            for(i=0; i<dY; i++)
                {
                    GFX_PutPixel(Image,X,Y,PixelState);

                    //Now do all the fractional stuff
                    if(YDir)
                        {
                            Y--;
                        }
                    else
                        {
                            Y++;
                        }

                    X_Numerator += dX;

                    if(X_Numerator >= dY)
                        {
                            X_Numerator-=dY;

                            if(StopX > StartX)
                                {
                                    X++;
                                }
                            else
                                {
                                    X--;
                                }
                        }
                }
        }
}
//Used for drawing characters from Dot factory generated code
//http://www.pavius.net/2009/07/the-dot-factory-an-lcd-font-and-image-generator/
int16_t GFX_Draw_DOT_FACTORY_Character(GFX_ImagePlane *Image,uint8_t Character,int16_t StartX, int16_t StartY, FONT_INFO *MyFont,uint8_t PixelState)
{
    uint8_t i,j,x,y;
    uint16_t BytesPerLine;
    uint16_t CharacterStartOffset;
    uint16_t CharacterWidth;
    uint16_t LineOffset;
    uint8_t CharacterOffset;
    x = StartX;
    y = StartY;

    //'render' the space character
    if(Character == 0x20)
        {
            return StartX += MyFont->spacePixels;
        }

    if(Character == 0x09)
        {
            return StartX += MyFont->spacePixels * GFX_SPACES_PER_TAB;
        }

    //first do some boundary checking on the characters
    if(Character<(uint8_t)MyFont->startChar)
        Character = MyFont->startChar;

    if(Character>(uint8_t)MyFont->endChar)
        Character = MyFont->endChar;

    //Now re-map stand ascii character to the the one offset from the start character in the font
    CharacterOffset = Character - (uint8_t)MyFont->startChar;
    //Calculate the bytes per line...
    BytesPerLine = (MyFont->charInfo[CharacterOffset].widthBits >> 3);

    if((MyFont->charInfo[CharacterOffset].widthBits&0x7)) //Round up if needed
        BytesPerLine++;

    CharacterStartOffset = MyFont->charInfo[CharacterOffset].offset;
    CharacterWidth = MyFont->charInfo[CharacterOffset].widthBits;

    for(i=0; i<MyFont->heightPixels; i++)
        {
            x = StartX;
            LineOffset = CharacterStartOffset + (i*BytesPerLine);

            for(j=0; j<CharacterWidth; j++)
                {
                    if(GFX_GetBitFromArray((uint8_t *)&MyFont->data[LineOffset],j))
                        {
                            GFX_PutPixel(Image,x,y,1);
                        }

                    x++;
                }

            y++;
        }

    return StartX + CharacterWidth + 1;
}
void GFX_Draw_DOT_FACTORY_String(GFX_ImagePlane *Image,char *String,int16_t StartX, int16_t StartY, FONT_INFO *MyFont,uint8_t PixelState)
{
    uint8_t Ptr = 0;
    uint8_t NextChar;
    NextChar = String[Ptr];

    while((NextChar!=0) && (Ptr <GFX_MAX_STRING_LEN))
        {
            StartX = GFX_Draw_DOT_FACTORY_Character(Image,NextChar,StartX,StartY,MyFont,PixelState);
            Ptr++;
            NextChar = String[Ptr];
        }
}
int16_t GFX_ScaledDrawCharacter(GFX_ImagePlane *Image,uint8_t Character,int16_t StartX, int16_t StartY, const GFX_Font *MyFont,float ScaleFactor,uint8_t PixelState)
{
    //GFX_Font does not contain any of the ASCII control codes
    if(Character<0x20)
        return StartX;

    Character = Character - 0x20;

    if(Character == 0x00)
        {
            return StartX + MyFont->PixelsPerSpace; // No need to render anything....
        }
    else
        {
            GFX_ScaledBlit(Image,MyFont->CharacterSprites[Character],StartX,StartY,ScaleFactor,PixelState);
        }

    StartX += MyFont->CharacterSprites[Character]->SizeX * (int16_t)(ScaleFactor+.5);
    return StartX;
}
int16_t GFX_DrawCharacter(GFX_ImagePlane *Image,uint8_t Character,int16_t StartX, int16_t StartY, const GFX_Font *MyFont,uint8_t PixelState)
{
    //GFX_Font does not contain any of the ASCII control codes
    if(Character<0x20)
        return StartX;

    Character = Character - 0x20;

    if(Character == 0x00)
        {
            return StartX + MyFont->PixelsPerSpace; // No need to render anything....
        }
    else
        {
            GFX_Blit(Image,MyFont->CharacterSprites[Character],StartX,StartY,PixelState);
        }

    StartX += MyFont->CharacterSprites[Character]->SizeX;
    return StartX;
}



int16_t GFX_GetStringWidth(char *String,const GFX_Font *MyFont)
{
    uint8_t Ptr = 0;
    uint8_t NextChar;
    int16_t StringSize = 0;
    NextChar = String[Ptr];
    Ptr++;

    while((NextChar!=0) && (Ptr <GFX_MAX_STRING_LEN))
        {
            if(NextChar>=0x20 && NextChar<0x80)
                {
                    //Remember that we don't endcode the wide space characters in the FONT table!
                    StringSize += MyFont->CharacterSprites[NextChar - 0x20]->SizeX + MyFont->InterCharacterSpacing;
                    NextChar = String[Ptr];
                    Ptr++;
                }
        }

    return StringSize - MyFont->InterCharacterSpacing; //Remove the last inter character space!
}


int16_t GFX_GetStringWidth_CustomSpacing(char *String,const GFX_Font *MyFont,uint16_t Spacing)
{
    uint8_t Ptr = 0;
    uint8_t NextChar;
    int16_t StringSize = 0;
    NextChar = String[Ptr];
    Ptr++;

    while((NextChar!=0) && (Ptr <GFX_MAX_STRING_LEN))
        {
            if(NextChar>=0x20 && NextChar<0x80)
                {
                    //Remember that we don't endcode the wide space characters in the FONT table!
                    StringSize += MyFont->CharacterSprites[NextChar - 0x20]->SizeX + Spacing;
                    NextChar = String[Ptr];
                    Ptr++;
                }
        }

    return StringSize - Spacing; //Remove the last inter character space!
}


void GFX_DrawHorizontalCenteredString(GFX_ImagePlane *Image,char *String,int16_t StartY, const GFX_Font *MyFont,uint8_t PixelState)
{
    int16_t StartX = 0;
    StartX = ((int16_t)Image->SizeX - GFX_GetStringWidth(String,MyFont))>>1;
    GFX_DrawString(Image,String,StartX,StartY,MyFont,PixelState);
}

int16_t GFX_DrawRightAlignedCenteredString_CustomSpacing(GFX_ImagePlane *Image,char *String,int16_t StartY, const GFX_Font *MyFont,uint8_t PixelState,uint16_t CustomCharacterSpacing,int16_t RightSideBuffer)
{
    int16_t StartX = 0;
    StartX = ((int16_t)Image->SizeX - GFX_GetStringWidth_CustomSpacing(String,MyFont,CustomCharacterSpacing) - RightSideBuffer);
    GFX_DrawString_CustomSpacing(Image,String,StartX,StartY,MyFont,PixelState,CustomCharacterSpacing);
    return StartX;
}

void GFX_DrawHorizontalCenteredString_CustomSpacing(GFX_ImagePlane *Image,char *String,int16_t StartY, const GFX_Font *MyFont,uint8_t PixelState,uint16_t Spacing)
{
    int16_t StartX = 0;
    StartX = ((int16_t)Image->SizeX - GFX_GetStringWidth_CustomSpacing(String,MyFont,Spacing))>>1;
    GFX_DrawString_CustomSpacing(Image,String,StartX,StartY,MyFont,PixelState,Spacing);
}


void GFX_ScaledDrawString(GFX_ImagePlane *Image,char *String,int16_t StartX, int16_t StartY, const GFX_Font *MyFont,float ScaleFactor ,uint8_t PixelState)
{
    uint8_t Ptr = 0;
    uint8_t NextChar;
     NextChar = String[Ptr];

    while((NextChar!=0) && (Ptr <GFX_MAX_STRING_LEN))
        {
            StartX = GFX_ScaledDrawCharacter(Image,NextChar,StartX,StartY,MyFont,ScaleFactor,PixelState);
            StartX += MyFont->InterCharacterSpacing * (int16_t)(ScaleFactor+.5);
            Ptr++;
            NextChar = String[Ptr];
        }
}


void GFX_ScaledDrawString_CustomSpacing(GFX_ImagePlane *Image,char *String,int16_t StartX, int16_t StartY, const GFX_Font *MyFont,float ScaleFactor ,uint8_t PixelState,uint16_t Spacing)
{
    uint8_t Ptr = 0;
    uint8_t NextChar;
    NextChar = String[Ptr];

    while((NextChar!=0) && (Ptr <GFX_MAX_STRING_LEN))
        {
            StartX = GFX_ScaledDrawCharacter(Image,NextChar,StartX,StartY,MyFont,ScaleFactor,PixelState);
            StartX += (int16_t)((ScaleFactor * Spacing) +.5);
            Ptr++;
            NextChar = String[Ptr];
        }
}


int16_t  GFX_DrawString(GFX_ImagePlane *Image,char *String,int16_t StartX, int16_t StartY, const GFX_Font *MyFont,uint8_t PixelState)
{
    uint8_t Ptr = 0;
    uint8_t NextChar;
   
    NextChar = String[Ptr];

    while((NextChar!=0) && (Ptr <GFX_MAX_STRING_LEN))
        {
            StartX = GFX_DrawCharacter(Image,NextChar,StartX,StartY,MyFont,PixelState);
            StartX += MyFont->InterCharacterSpacing;
            Ptr++;
            NextChar = String[Ptr];
        }

    return StartX + 1;
}

int16_t  GFX_printf(GFX_ImagePlane *Image,int16_t StartX, int16_t StartY, const GFX_Font *MyFont, uint8_t PixelState,uint16_t CharacterSpacing, const char *FormatString,...)
{
    int16_t End;
    va_list argptr;
    va_start(argptr,FormatString);
    snprintf((char *)GFX_StringBuf,GFX_MAX_PRINTF_BUF_LENGTH,FormatString,argptr);
    va_end(argptr);
    End = StartX + GFX_GetStringWidth_CustomSpacing(GFX_StringBuf,MyFont,CharacterSpacing) + 1;
    GFX_DrawString_CustomSpacing(Image,GFX_StringBuf,StartX,StartY,MyFont,PixelState,CharacterSpacing);
    return End;
}


int16_t GFX_DrawString_CustomSpacing(GFX_ImagePlane *Image,char *String,int16_t StartX, int16_t StartY, const GFX_Font *MyFont,uint8_t PixelState,uint16_t Spacing)
{
    uint8_t Ptr = 0;
    uint8_t NextChar;

    NextChar = String[Ptr];

    while((NextChar!=0) && (Ptr <GFX_MAX_STRING_LEN))
        {
            StartX = GFX_DrawCharacter(Image,NextChar,StartX,StartY,MyFont,PixelState);
            StartX += Spacing;
            Ptr++;
            NextChar = String[Ptr];
        }

    return StartX+1;
}
void GFX_DrawListPrimitive(GFX_ImagePlane *Image,GFXListPrimitive *LP ,uint8_t PixelState)
{
    uint8_t Points;
    uint8_t i;

    if(LP->NumPoints >1)
        {
            switch(LP->DrawMode)
                {
                    default:
                    case GFX_LIST_PRIMITIVE_CLOSED:
                        for(i=1; i<LP->NumPoints; i++)
                            {
                                GFX_DrawLine(Image,(int16_t)(LP->PointList[i-1].X + LP->Center.X),(int16_t)(LP->PointList[i-1].Y + LP->Center.Y),
                                             (int16_t)(LP->PointList[i].X + LP->Center.X),(int16_t)(LP->PointList[i].Y + LP->Center.Y),PixelState);
                            }

                        GFX_DrawLine(Image,(int16_t)(LP->PointList[LP->NumPoints-1].X + LP->Center.X),(int16_t)(LP->PointList[LP->NumPoints-1].Y + LP->Center.Y),
                                     (int16_t)(LP->PointList[0].X + LP->Center.X),(int16_t)(LP->PointList[0].Y + LP->Center.Y),PixelState);
                        break;

                    case GFX_LIST_PRIMITIVE_CONNECTED:
                        for(i=1; i<LP->NumPoints; i++)
                            {
                                GFX_DrawLine(Image,(int16_t)(LP->PointList[i-1].X + LP->Center.X),(int16_t)(LP->PointList[i-1].Y + LP->Center.Y),
                                             (int16_t)(LP->PointList[i].X + LP->Center.X),(int16_t)(LP->PointList[i].Y + LP->Center.Y),PixelState);
                            }

                        break;

                    case GFX_LIST_PRIMITIVE_DISCONNECTED:
                        Points = LP->NumPoints>>1;

                        for(i=0; i<Points; i++)
                            {
                                GFX_DrawLine(Image,(int16_t)(LP->PointList[i*2].X + LP->Center.X),(int16_t)(LP->PointList[i*2].Y + LP->Center.Y),
                                             (int16_t)(LP->PointList[i*2+1].X + LP->Center.X),(int16_t)(LP->PointList[i*2+1].Y + LP->Center.Y),PixelState);
                            }

                        break;

                    case GFX_LIST_PRIMITIVE_CLOSED_YFLIPPED:
                        for(i=1; i<LP->NumPoints; i++)
                            {
                                GFX_DrawLine(Image,(int16_t)(LP->PointList[i-1].X + LP->Center.X),(int16_t)(-LP->PointList[i-1].Y + LP->Center.Y),
                                             (int16_t)(LP->PointList[i].X + LP->Center.X),(int16_t)(-LP->PointList[i].Y + LP->Center.Y),PixelState);
                            }

                        GFX_DrawLine(Image,(int16_t)(LP->PointList[LP->NumPoints-1].X + LP->Center.X),(int16_t)(-LP->PointList[LP->NumPoints-1].Y + LP->Center.Y),
                                     (int16_t)(LP->PointList[0].X + LP->Center.X),(int16_t)(-LP->PointList[0].Y + LP->Center.Y),PixelState);
                        break;

                    case GFX_LIST_PRIMITIVE_CONNECTED_YFLIPPED:
                        for(i=1; i<LP->NumPoints; i++)
                            {
                                GFX_DrawLine(Image,(int16_t)(LP->PointList[i-1].X + LP->Center.X),(int16_t)(-LP->PointList[i-1].Y + LP->Center.Y),
                                             (int16_t)(LP->PointList[i].X + LP->Center.X),(int16_t)(-LP->PointList[i].Y + LP->Center.Y),PixelState);
                            }

                        break;

                    case GFX_LIST_PRIMITIVE_DISCONNECTED_YFLIPPED:
                        Points = LP->NumPoints>>1;

                        for(i=0; i<Points; i++)
                            {
                                GFX_DrawLine(Image,(int16_t)(LP->PointList[i*2].X + LP->Center.X),(int16_t)(-LP->PointList[i*2].Y + LP->Center.Y),
                                             (int16_t)(LP->PointList[i*2+1].X + LP->Center.X),(int16_t)(-LP->PointList[i*2+1].Y + LP->Center.Y),PixelState);
                            }

                        break;
                }
        }
}
void GFX_DrawScaledListPrimitive(GFX_ImagePlane *Image,GFXListPrimitive *LP , FIXED_7_8 Scale, uint8_t PixelState)
{
    uint8_t Points;
    uint8_t i;
    GFXRelativePoint TransformedPoint[2];

    if(LP->NumPoints >1)
        {
            switch(LP->DrawMode)
                {
                    default:
                    case GFX_LIST_PRIMITIVE_CLOSED:
                        for(i=1; i<LP->NumPoints; i++)
                            {
                                TransformedPoint[0].X = FMul_15_0to7_8(LP->PointList[i-1].X,Scale) 	+ LP->Center.X;
                                TransformedPoint[0].Y = FMul_15_0to7_8(LP->PointList[i-1].Y,Scale)   + LP->Center.Y;
                                TransformedPoint[1].X = FMul_15_0to7_8(LP->PointList[i].X,Scale)     + LP->Center.X;
                                TransformedPoint[1].Y = FMul_15_0to7_8(LP->PointList[i].Y,Scale)     + LP->Center.Y;
                                GFX_DrawLine(Image,TransformedPoint[0].X,TransformedPoint[0].Y,
                                             TransformedPoint[1].X,TransformedPoint[1].Y,PixelState);
                            }

                        TransformedPoint[0].X = FMul_15_0to7_8(LP->PointList[LP->NumPoints-1].X,Scale)   + LP->Center.X;
                        TransformedPoint[0].Y = FMul_15_0to7_8(LP->PointList[LP->NumPoints-1].Y,Scale)   + LP->Center.Y;
                        TransformedPoint[1].X = FMul_15_0to7_8(LP->PointList[0].X,Scale)				 + LP->Center.X;
                        TransformedPoint[1].Y = FMul_15_0to7_8(LP->PointList[0].Y,Scale)				 + LP->Center.Y;
                        GFX_DrawLine(Image,TransformedPoint[0].X,TransformedPoint[0].Y,
                                     TransformedPoint[1].X,TransformedPoint[1].Y,PixelState);
                        break;

                    case GFX_LIST_PRIMITIVE_CONNECTED:
                        for(i=1; i<LP->NumPoints; i++)
                            {
                                TransformedPoint[0].X = FMul_15_0to7_8(LP->PointList[i-1].X,Scale) 	+ LP->Center.X;
                                TransformedPoint[0].Y = FMul_15_0to7_8(LP->PointList[i-1].Y,Scale)   + LP->Center.Y;
                                TransformedPoint[1].X = FMul_15_0to7_8(LP->PointList[i].X,Scale)     + LP->Center.X;
                                TransformedPoint[1].Y = FMul_15_0to7_8(LP->PointList[i].Y,Scale)     + LP->Center.Y;
                                GFX_DrawLine(Image,TransformedPoint[0].X,TransformedPoint[0].Y,
                                             TransformedPoint[1].X,TransformedPoint[1].Y,PixelState);
                            }

                        break;

                    case GFX_LIST_PRIMITIVE_DISCONNECTED:
                        Points = LP->NumPoints>>1;

                        for(i=0; i<Points; i++)
                            {
                                TransformedPoint[0].X = FMul_15_0to7_8(LP->PointList[i*2].X,Scale)		+ LP->Center.X;
                                TransformedPoint[0].Y = FMul_15_0to7_8(LP->PointList[i*2].Y,Scale)		+ LP->Center.Y;
                                TransformedPoint[1].X = FMul_15_0to7_8(LP->PointList[(i*2)+1].X,Scale)   + LP->Center.X;
                                TransformedPoint[1].Y = FMul_15_0to7_8(LP->PointList[(i*2)+1].Y,Scale)   + LP->Center.Y;
                                GFX_DrawLine(Image,TransformedPoint[0].X,TransformedPoint[0].Y,
                                             TransformedPoint[1].X,TransformedPoint[1].Y,PixelState);
                            }

                        break;
                }
        }
}
void GFX_DrawRotatedListPrimitive(GFX_ImagePlane *Image,GFXListPrimitive *LP , uint8_t Angle, uint8_t PixelState)
{
    uint8_t Points;
    uint8_t i;
    GFXRelativePoint TransformedPoint[2];

    if(LP->NumPoints >1)
        {
            //If we are drawing non connected points,  there must be an even number of them.
            switch(LP->DrawMode)
                {
                    case GFX_LIST_PRIMITIVE_DISCONNECTED:
                        Points = LP->NumPoints>>1;

                        for(i=0; i<Points; i++)
                            {
                                TransformedPoint[0].X = FMul_15_0to1_14((LP->PointList[i*2].X), Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                                        FMul_15_0to1_14(LP->PointList[i*2].Y, Fixed_1_14_SineTable[Angle]);
                                TransformedPoint[0].Y = FMul_15_0to1_14(LP->PointList[i*2].X, Fixed_1_14_SineTable[Angle ]) +
                                                        FMul_15_0to1_14(LP->PointList[i*2].Y, Fixed_1_14_SineTable[(Angle + 64)&0xff]);
                                TransformedPoint[1].X = FMul_15_0to1_14(LP->PointList[(i*2)+1].X, Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                                        FMul_15_0to1_14(LP->PointList[(i*2)+1].Y, Fixed_1_14_SineTable[Angle]);
                                TransformedPoint[1].Y = FMul_15_0to1_14(LP->PointList[(i*2)+1].X, Fixed_1_14_SineTable[Angle]) +
                                                        FMul_15_0to1_14(LP->PointList[(i*2)+1].Y, Fixed_1_14_SineTable[(Angle + 64)&0xff]);
                                TransformedPoint[0].X += LP->Center.X;
                                TransformedPoint[0].Y += LP->Center.Y;
                                TransformedPoint[1].X += LP->Center.X;
                                TransformedPoint[1].Y += LP->Center.Y;
                                GFX_DrawLine(Image,(TransformedPoint[0].X + LP->Center.X),(TransformedPoint[0].Y  + LP->Center.Y),
                                             (TransformedPoint[1].X + LP->Center.X),(TransformedPoint[1].Y + LP->Center.Y),PixelState);
                            }

                        break;

                    case GFX_LIST_PRIMITIVE_CONNECTED:
                        for(i=1; i<LP->NumPoints; i++)
                            {
                                TransformedPoint[0].X = (FMul_15_0to1_14(LP->PointList[i-1].X, Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                                         FMul_15_0to1_14(LP->PointList[i-1].Y, Fixed_1_14_SineTable[Angle])  );
                                TransformedPoint[0].Y = ( FMul_15_0to1_14(LP->PointList[i-1].X, Fixed_1_14_SineTable[Angle]) +
                                                          FMul_15_0to1_14(LP->PointList[i-1].Y, Fixed_1_14_SineTable[(Angle + 64)&0xff] ) );
                                TransformedPoint[1].X = (FMul_15_0to1_14(LP->PointList[i].X, Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                                         FMul_15_0to1_14(LP->PointList[i].Y, Fixed_1_14_SineTable[Angle]) );
                                TransformedPoint[1].Y = (FMul_15_0to1_14(LP->PointList[i].X, Fixed_1_14_SineTable[Angle]) +
                                                         FMul_15_0to1_14(LP->PointList[i].Y, Fixed_1_14_SineTable[(Angle + 64)&0xff]));
                                TransformedPoint[0].X +=  LP->Center.X;
                                TransformedPoint[0].Y +=  LP->Center.Y;
                                TransformedPoint[1].X +=  LP->Center.X;
                                TransformedPoint[1].Y +=  LP->Center.Y;
                                GFX_DrawLine(Image,(TransformedPoint[0].X),(TransformedPoint[0].Y),
                                             (TransformedPoint[1].X),(TransformedPoint[1].Y),PixelState);
                            }

                        break;

                    default:
                    case GFX_LIST_PRIMITIVE_CLOSED:
                        for(i=1; i<LP->NumPoints; i++)
                            {
                                TransformedPoint[0].X = (FMul_15_0to1_14(LP->PointList[i-1].X, Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                                         FMul_15_0to1_14(LP->PointList[i-1].Y, Fixed_1_14_SineTable[Angle])  );
                                TransformedPoint[0].Y = ( FMul_15_0to1_14(LP->PointList[i-1].X, Fixed_1_14_SineTable[Angle]) +
                                                          FMul_15_0to1_14(LP->PointList[i-1].Y, Fixed_1_14_SineTable[(Angle + 64)&0xff] ) );
                                TransformedPoint[1].X = (FMul_15_0to1_14(LP->PointList[i].X, Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                                         FMul_15_0to1_14(LP->PointList[i].Y, Fixed_1_14_SineTable[Angle]) );
                                TransformedPoint[1].Y = (FMul_15_0to1_14(LP->PointList[i].X, Fixed_1_14_SineTable[Angle]) +
                                                         FMul_15_0to1_14(LP->PointList[i].Y, Fixed_1_14_SineTable[(Angle + 64)&0xff]));
                                TransformedPoint[0].X +=  LP->Center.X;
                                TransformedPoint[0].Y +=  LP->Center.Y;
                                TransformedPoint[1].X +=  LP->Center.X;
                                TransformedPoint[1].Y +=  LP->Center.Y;
                                GFX_DrawLine(Image,(TransformedPoint[0].X),(TransformedPoint[0].Y),
                                             (TransformedPoint[1].X),(TransformedPoint[1].Y),PixelState);
                            }

                        TransformedPoint[0].X = (FMul_15_0to1_14(LP->PointList[LP->NumPoints-1].X, Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                                 FMul_15_0to1_14(LP->PointList[LP->NumPoints-1].Y, Fixed_1_14_SineTable[Angle])  );
                        TransformedPoint[0].Y = ( FMul_15_0to1_14(LP->PointList[LP->NumPoints-1].X, Fixed_1_14_SineTable[Angle]) +
                                                  FMul_15_0to1_14(LP->PointList[LP->NumPoints-1].Y, Fixed_1_14_SineTable[(Angle + 64)&0xff] ) );
                        TransformedPoint[1].X = (FMul_15_0to1_14(LP->PointList[0].X, Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                                 FMul_15_0to1_14(LP->PointList[0].Y, Fixed_1_14_SineTable[Angle]) );
                        TransformedPoint[1].Y = (FMul_15_0to1_14(LP->PointList[0].X, Fixed_1_14_SineTable[Angle]) +
                                                 FMul_15_0to1_14(LP->PointList[0].Y, Fixed_1_14_SineTable[(Angle + 64)&0xff]));
                        TransformedPoint[0].X +=  LP->Center.X;
                        TransformedPoint[0].Y +=  LP->Center.Y;
                        TransformedPoint[1].X +=  LP->Center.X;
                        TransformedPoint[1].Y +=  LP->Center.Y;
                        GFX_DrawLine(Image,(TransformedPoint[0].X),(TransformedPoint[0].Y),
                                     (TransformedPoint[1].X),(TransformedPoint[1].Y),PixelState);
                        break;
                }
        }
}
void GFX_DrawScaledRotatedListPrimitive(GFX_ImagePlane *Image,GFXListPrimitive *LP , uint8_t Angle,FIXED_7_8 Scale, uint8_t PixelState)
{
    uint8_t i;
    GFXRelativePoint TransformedPoint[2];

    if(LP->NumPoints >1)
        {
            switch(LP->DrawMode)
                {
                    default:
                    case GFX_LIST_PRIMITIVE_CLOSED:
                        for(i=1; i<LP->NumPoints; i++)
                            {
                                TransformedPoint[0].X = (FMul_15_0to1_14(LP->PointList[i-1].X, Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                                         FMul_15_0to1_14(LP->PointList[i-1].Y, Fixed_1_14_SineTable[Angle])  );
                                TransformedPoint[0].Y = ( FMul_15_0to1_14(LP->PointList[i-1].X, Fixed_1_14_SineTable[Angle]) +
                                                          FMul_15_0to1_14(LP->PointList[i-1].Y, Fixed_1_14_SineTable[(Angle + 64)&0xff] ) );
                                TransformedPoint[1].X = (FMul_15_0to1_14(LP->PointList[i].X, Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                                         FMul_15_0to1_14(LP->PointList[i].Y, Fixed_1_14_SineTable[Angle]) );
                                TransformedPoint[1].Y = (FMul_15_0to1_14(LP->PointList[i].X, Fixed_1_14_SineTable[Angle]) +
                                                         FMul_15_0to1_14(LP->PointList[i].Y, Fixed_1_14_SineTable[(Angle + 64)&0xff]));
                                TransformedPoint[0].X = FMul_15_0to7_8(TransformedPoint[0].X,Scale)  +    LP->Center.X;
                                TransformedPoint[0].Y = FMul_15_0to7_8(TransformedPoint[0].Y,Scale)  +    LP->Center.Y;
                                TransformedPoint[1].X = FMul_15_0to7_8(TransformedPoint[1].X,Scale)  +    LP->Center.X;
                                TransformedPoint[1].Y = FMul_15_0to7_8(TransformedPoint[1].Y,Scale)  +    LP->Center.Y;
                                GFX_DrawLine(Image,(TransformedPoint[0].X),(TransformedPoint[0].Y),
                                             (TransformedPoint[1].X),(TransformedPoint[1].Y),PixelState);
                            }

                        TransformedPoint[0].X = (FMul_15_0to1_14(LP->PointList[LP->NumPoints-1].X, Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                                 FMul_15_0to1_14(LP->PointList[LP->NumPoints-1].Y, Fixed_1_14_SineTable[Angle])  );
                        TransformedPoint[0].Y = ( FMul_15_0to1_14(LP->PointList[LP->NumPoints-1].X, Fixed_1_14_SineTable[Angle]) +
                                                  FMul_15_0to1_14(LP->PointList[LP->NumPoints-1].Y, Fixed_1_14_SineTable[(Angle + 64)&0xff] ) );
                        TransformedPoint[1].X = (FMul_15_0to1_14(LP->PointList[0].X, Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                                 FMul_15_0to1_14(LP->PointList[0].Y, Fixed_1_14_SineTable[Angle]) );
                        TransformedPoint[1].Y = (FMul_15_0to1_14(LP->PointList[0].X, Fixed_1_14_SineTable[Angle]) +
                                                 FMul_15_0to1_14(LP->PointList[0].Y, Fixed_1_14_SineTable[(Angle + 64)&0xff]));
                        TransformedPoint[0].X = FMul_15_0to7_8(TransformedPoint[0].X,Scale)  +    LP->Center.X;
                        TransformedPoint[0].Y = FMul_15_0to7_8(TransformedPoint[0].Y,Scale)  +    LP->Center.Y;
                        TransformedPoint[1].X = FMul_15_0to7_8(TransformedPoint[1].X,Scale)  +    LP->Center.X;
                        TransformedPoint[1].Y = FMul_15_0to7_8(TransformedPoint[1].Y,Scale)  +    LP->Center.Y;
                        GFX_DrawLine(Image,(TransformedPoint[0].X),(TransformedPoint[0].Y),
                                     (TransformedPoint[1].X),(TransformedPoint[1].Y),PixelState);
                        break;

                    case GFX_LIST_PRIMITIVE_DISCONNECTED:
                        for(i=0; i<LP->NumPoints>>1; i++)
                            {
                                TransformedPoint[0].X = (FMul_15_0to1_14(LP->PointList[i*2].X, Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                                         FMul_15_0to1_14(LP->PointList[i*2].Y, Fixed_1_14_SineTable[Angle])  );
                                TransformedPoint[0].Y = ( FMul_15_0to1_14(LP->PointList[i*2].X, Fixed_1_14_SineTable[Angle]) +
                                                          FMul_15_0to1_14(LP->PointList[i*2].Y, Fixed_1_14_SineTable[(Angle + 64)&0xff] ) );
                                TransformedPoint[1].X = (FMul_15_0to1_14(LP->PointList[i*2+1].X, Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                                         FMul_15_0to1_14(LP->PointList[i*2+1].Y, Fixed_1_14_SineTable[Angle]) );
                                TransformedPoint[1].Y = (FMul_15_0to1_14(LP->PointList[i*2+1].X, Fixed_1_14_SineTable[Angle]) +
                                                         FMul_15_0to1_14(LP->PointList[i*2+1].Y, Fixed_1_14_SineTable[(Angle + 64)&0xff]));
                                TransformedPoint[0].X = FMul_15_0to7_8(TransformedPoint[0].X,Scale)  +    LP->Center.X;
                                TransformedPoint[0].Y = FMul_15_0to7_8(TransformedPoint[0].Y,Scale)  +    LP->Center.Y;
                                TransformedPoint[1].X = FMul_15_0to7_8(TransformedPoint[1].X,Scale)  +    LP->Center.X;
                                TransformedPoint[1].Y = FMul_15_0to7_8(TransformedPoint[1].Y,Scale)  +    LP->Center.Y;
                                GFX_DrawLine(Image,(TransformedPoint[0].X),(TransformedPoint[0].Y),
                                             (TransformedPoint[1].X),(TransformedPoint[1].Y),PixelState);
                            }

                        break;

                    case GFX_LIST_PRIMITIVE_CONNECTED:
                        for(i=1; i<LP->NumPoints; i++)
                            {
                                TransformedPoint[0].X = (FMul_15_0to1_14(LP->PointList[i-1].X, Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                                         FMul_15_0to1_14(LP->PointList[i-1].Y, Fixed_1_14_SineTable[Angle])  );
                                TransformedPoint[0].Y = ( FMul_15_0to1_14(LP->PointList[i-1].X, Fixed_1_14_SineTable[Angle]) +
                                                          FMul_15_0to1_14(LP->PointList[i-1].Y, Fixed_1_14_SineTable[(Angle + 64)&0xff] ) );
                                TransformedPoint[1].X = (FMul_15_0to1_14(LP->PointList[i].X, Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                                         FMul_15_0to1_14(LP->PointList[i].Y, Fixed_1_14_SineTable[Angle]) );
                                TransformedPoint[1].Y = (FMul_15_0to1_14(LP->PointList[i].X, Fixed_1_14_SineTable[Angle]) +
                                                         FMul_15_0to1_14(LP->PointList[i].Y, Fixed_1_14_SineTable[(Angle + 64)&0xff]));
                                TransformedPoint[0].X = FMul_15_0to7_8(TransformedPoint[0].X,Scale)  +    LP->Center.X;
                                TransformedPoint[0].Y = FMul_15_0to7_8(TransformedPoint[0].Y,Scale)  +    LP->Center.Y;
                                TransformedPoint[1].X = FMul_15_0to7_8(TransformedPoint[1].X,Scale)  +    LP->Center.X;
                                TransformedPoint[1].Y = FMul_15_0to7_8(TransformedPoint[1].Y,Scale)  +    LP->Center.Y;
                                GFX_DrawLine(Image,(TransformedPoint[0].X),(TransformedPoint[0].Y),
                                             (TransformedPoint[1].X),(TransformedPoint[1].Y),PixelState);
                            }

                        break;
                }
        }
}
void GFX_Blit(GFX_ImagePlane *Destination,const GFX_ImagePlane *Sprite,int16_t x,int16_t y, uint8_t PixelState)
{
    uint16_t i,j;

    for(j=0; j<Sprite->SizeY; j++)
        {
            for(i=0; i<Sprite->SizeX; i++)
                {
                    if(GFX_ImagePlane_Get((GFX_ImagePlane *)Sprite,i,j) == 1)
                        GFX_PutPixel(Destination,x+i,y+j,PixelState);
                }
        }
}
void GFX_ScaledBlit(GFX_ImagePlane *Destination,const GFX_ImagePlane *Sprite,int16_t x,int16_t y, float ScaleFactor, uint8_t PixelState)
{
    uint16_t i,j;
    uint16_t DestinationSizeX = 0;
    uint16_t DestinationSizeY = 0;
    uint16_t AccumulatorX = 0;
    uint16_t AccumulatorY = 0;
    uint16_t SourcePixelX = 0;
    uint16_t SourcePixelY = 0;
    DestinationSizeX = (uint16_t)((ScaleFactor * (float)Sprite->SizeX)+0.5);
    DestinationSizeY = (uint16_t)((ScaleFactor * (float)Sprite->SizeY)+0.5);

    for(j=0; j<DestinationSizeY; j++)
        {
            SourcePixelX = 0;

            for(i=0; i<DestinationSizeX; i++)
                {
                    if(GFX_ImagePlane_Get((GFX_ImagePlane *)Sprite,SourcePixelX,SourcePixelY) == 1)
                        GFX_PutPixel(Destination,x+i,y+j,PixelState);

                    AccumulatorX += Sprite->SizeX;

                    if(AccumulatorX >= DestinationSizeX)
                        {
                            AccumulatorX -= DestinationSizeX;
                            SourcePixelX++;
                        }
                }

            AccumulatorY += Sprite->SizeY;

            if(AccumulatorY >= DestinationSizeY)
                {
                    AccumulatorY -= DestinationSizeY;
                    SourcePixelY++;
                }
        }
}

// midpoint circle algorithm
void GFX_DrawCircle(GFX_ImagePlane *RC, int16_t x0, int16_t y0, int16_t radius, uint8_t PixelState)
{
    int16_t f = 1 - radius;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * radius;
    int16_t x = 0;
    int16_t y = radius;
    GFX_PutPixel(RC, x0, y0 + radius, PixelState);
    GFX_PutPixel(RC, x0, y0 - radius, PixelState);
    GFX_PutPixel(RC, x0 + radius, y0, PixelState);
    GFX_PutPixel(RC, x0 - radius, y0, PixelState);

    while(x < y)
        {
            // ddF_x == 2 * x + 1;
            // ddF_y == -2 * y;
            // f == x*x + y*y - radius*radius + 2*x - y + 1;
            if(f >= 0)
                {
                    y--;
                    ddF_y += 2;
                    f += ddF_y;
                }

            x++;
            ddF_x += 2;
            f += ddF_x;
            GFX_PutPixel(RC, x0 + x, y0 + y, PixelState);
            GFX_PutPixel(RC, x0 - x, y0 + y, PixelState);
            GFX_PutPixel(RC, x0 + x, y0 - y, PixelState);
            GFX_PutPixel(RC, x0 - x, y0 - y, PixelState);
            GFX_PutPixel(RC, x0 + y, y0 + x, PixelState);
            GFX_PutPixel(RC, x0 - y, y0 + x, PixelState);
            GFX_PutPixel(RC, x0 + y, y0 - x, PixelState);
            GFX_PutPixel(RC, x0 - y, y0 - x, PixelState);
        }
}

uint8_t GFX_GetBitFromArray(uint8_t *DataArray,uint16_t Bit)
{
    uint8_t Mask;
    Mask = 1 << (7-(Bit&0x7));

    if(DataArray[Bit>>3] & Mask)
        return 1;
    else
        return 0;
}
void GFX_InitImagePlane(GFX_ImagePlane   *BP, uint16_t SizeX,uint16_t SizeY,uint8_t *ImagePlaneStorage)
{
    BP->ImagePlaneSpace = ImagePlaneStorage;
    BP->SizeX = SizeX;
    BP->SizeY = SizeY;
}

#ifndef INLINE_ImagePlane_PUT
void GFX_ImagePlane_Put(GFX_ImagePlane   *BP, uint16_t X,uint16_t Y, uint8_t Value)
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


#ifndef INLINE_GET_ImagePlane_BYTE
//this funtion grabs an 8-bit chunch from each row.   Assumes we are byte aligned.
//X is the starting pixel.  It will be byte aligned. I.e.  requesting X = 12 will get you X starting at 8.
//If you are past

uint8_t GFX_ImagePlane_GetByte(GFX_ImagePlane   *BP, uint16_t X,uint16_t Y)
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


#ifndef INLINE_ImagePlane_GET
uint8_t GFX_ImagePlane_Get(GFX_ImagePlane   *BP, uint16_t X,uint16_t Y)
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

void GFX_ImagePlane_Clear(GFX_ImagePlane   *BP)
{
    uint16_t PlaneSpaceSize;
    uint16_t i;
    PlaneSpaceSize = ((BP->SizeX+8)>>3) * BP->SizeY;

    for(i=0; i<PlaneSpaceSize; i++)
        {
            BP->ImagePlaneSpace[i] = 0;
        }
}



const FIXED_1_14 Fixed_1_14_SineTable [256] = { 0x0000,0x0192,0x0324,0x04B5,0x0646,0x07D6,0x0964,0x0AF1,
                                                0x0C7C,0x0E06,0x0F8D,0x1112,0x1294,0x1413,0x1590,0x1709,
                                                0x187E,0x19EF,0x1B5D,0x1CC6,0x1E2B,0x1F8C,0x20E7,0x223D,
                                                0x238E,0x24DA,0x2620,0x2760,0x289A,0x29CE,0x2AFB,0x2C21,
                                                0x2D41,0x2E5A,0x2F6C,0x3076,0x3179,0x3274,0x3368,0x3453,
                                                0x3537,0x3612,0x36E5,0x37B0,0x3871,0x392B,0x39DB,0x3A82,
                                                0x3B21,0x3BB6,0x3C42,0x3CC5,0x3D3F,0x3DAF,0x3E15,0x3E72,
                                                0x3EC5,0x3F0F,0x3F4F,0x3F85,0x3FB1,0x3FD4,0x3FEC,0x3FFB,
                                                0x4000,0x3FFB,0x3FEC,0x3FD4,0x3FB1,0x3F85,0x3F4F,0x3F0F,
                                                0x3EC5,0x3E72,0x3E15,0x3DAF,0x3D3F,0x3CC5,0x3C42,0x3BB6,
                                                0x3B21,0x3A82,0x39DB,0x392B,0x3871,0x37B0,0x36E5,0x3612,
                                                0x3537,0x3453,0x3368,0x3274,0x3179,0x3076,0x2F6C,0x2E5A,
                                                0x2D41,0x2C21,0x2AFB,0x29CE,0x289A,0x2760,0x2620,0x24DA,
                                                0x238E,0x223D,0x20E7,0x1F8C,0x1E2B,0x1CC6,0x1B5D,0x19EF,
                                                0x187E,0x1709,0x1590,0x1413,0x1294,0x1112,0x0F8D,0x0E06,
                                                0x0C7C,0x0AF1,0x0964,0x07D6,0x0646,0x04B5,0x0324,0x0192,
                                                0x0000,0xFE6E,0xFCDC,0xFB4B,0xF9BA,0xF82A,0xF69C,0xF50F,
                                                0xF384,0xF1FA,0xF073,0xEEEE,0xED6C,0xEBED,0xEA70,0xE8F7,
                                                0xE782,0xE611,0xE4A3,0xE33A,0xE1D5,0xE074,0xDF19,0xDDC3,
                                                0xDC72,0xDB26,0xD9E0,0xD8A0,0xD766,0xD632,0xD505,0xD3DF,
                                                0xD2BF,0xD1A6,0xD094,0xCF8A,0xCE87,0xCD8C,0xCC98,0xCBAD,
                                                0xCAC9,0xC9EE,0xC91B,0xC850,0xC78F,0xC6D5,0xC625,0xC57E,
                                                0xC4DF,0xC44A,0xC3BE,0xC33B,0xC2C1,0xC251,0xC1EB,0xC18E,
                                                0xC13B,0xC0F1,0xC0B1,0xC07B,0xC04F,0xC02C,0xC014,0xC005,
                                                0xC000,0xC005,0xC014,0xC02C,0xC04F,0xC07B,0xC0B1,0xC0F1,
                                                0xC13B,0xC18E,0xC1EB,0xC251,0xC2C1,0xC33B,0xC3BE,0xC44A,
                                                0xC4DF,0xC57E,0xC625,0xC6D5,0xC78F,0xC850,0xC91B,0xC9EE,
                                                0xCAC9,0xCBAD,0xCC98,0xCD8C,0xCE87,0xCF8A,0xD094,0xD1A6,
                                                0xD2BF,0xD3DF,0xD505,0xD632,0xD766,0xD8A0,0xD9E0,0xDB26,
                                                0xDC72,0xDDC3,0xDF19,0xE074,0xE1D5,0xE33A,0xE4A3,0xE611,
                                                0xE782,0xE8F7,0xEA70,0xEBED,0xED6C,0xEEEE,0xF073,0xF1FA,
                                                0xF384,0xF50F,0xF69C,0xF82A,0xF9BA,0xFB4B,0xFCDC,0xFE6E
                                              };

const FIXED_7_8 Fixed_7_8_SineTable [256] = { 	0x0000,0x0006,0x000D,0x0013,0x0019,0x001F,0x0026,0x002C,
                                                0x0032,0x0038,0x003E,0x0044,0x004A,0x0050,0x0056,0x005C,
                                                0x0062,0x0068,0x006D,0x0073,0x0079,0x007E,0x0084,0x0089,
                                                0x008E,0x0093,0x0098,0x009D,0x00A2,0x00A7,0x00AC,0x00B1,
                                                0x00B5,0x00B9,0x00BE,0x00C2,0x00C6,0x00CA,0x00CE,0x00D1,
                                                0x00D5,0x00D8,0x00DC,0x00DF,0x00E2,0x00E5,0x00E7,0x00EA,
                                                0x00ED,0x00EF,0x00F1,0x00F3,0x00F5,0x00F7,0x00F8,0x00FA,
                                                0x00FB,0x00FC,0x00FD,0x00FE,0x00FF,0x00FF,0x0100,0x0100,
                                                0x0100,0x0100,0x0100,0x00FF,0x00FF,0x00FE,0x00FD,0x00FC,
                                                0x00FB,0x00FA,0x00F8,0x00F7,0x00F5,0x00F3,0x00F1,0x00EF,
                                                0x00ED,0x00EA,0x00E7,0x00E5,0x00E2,0x00DF,0x00DC,0x00D8,
                                                0x00D5,0x00D1,0x00CE,0x00CA,0x00C6,0x00C2,0x00BE,0x00B9,
                                                0x00B5,0x00B1,0x00AC,0x00A7,0x00A2,0x009D,0x0098,0x0093,
                                                0x008E,0x0089,0x0084,0x007E,0x0079,0x0073,0x006D,0x0068,
                                                0x0062,0x005C,0x0056,0x0050,0x004A,0x0044,0x003E,0x0038,
                                                0x0032,0x002C,0x0026,0x001F,0x0019,0x0013,0x000D,0x0006,
                                                0x0000,0xFFFA,0xFFF3,0xFFED,0xFFE7,0xFFE1,0xFFDA,0xFFD4,
                                                0xFFCE,0xFFC8,0xFFC2,0xFFBC,0xFFB6,0xFFB0,0xFFAA,0xFFA4,
                                                0xFF9E,0xFF98,0xFF93,0xFF8D,0xFF87,0xFF82,0xFF7C,0xFF77,
                                                0xFF72,0xFF6D,0xFF68,0xFF63,0xFF5E,0xFF59,0xFF54,0xFF4F,
                                                0xFF4B,0xFF47,0xFF42,0xFF3E,0xFF3A,0xFF36,0xFF32,0xFF2F,
                                                0xFF2B,0xFF28,0xFF24,0xFF21,0xFF1E,0xFF1B,0xFF19,0xFF16,
                                                0xFF13,0xFF11,0xFF0F,0xFF0D,0xFF0B,0xFF09,0xFF08,0xFF06,
                                                0xFF05,0xFF04,0xFF03,0xFF02,0xFF01,0xFF01,0xFF00,0xFF00,
                                                0xFF00,0xFF00,0xFF00,0xFF01,0xFF01,0xFF02,0xFF03,0xFF04,
                                                0xFF05,0xFF06,0xFF08,0xFF09,0xFF0B,0xFF0D,0xFF0F,0xFF11,
                                                0xFF13,0xFF16,0xFF19,0xFF1B,0xFF1E,0xFF21,0xFF24,0xFF28,
                                                0xFF2B,0xFF2F,0xFF32,0xFF36,0xFF3A,0xFF3E,0xFF42,0xFF47,
                                                0xFF4B,0xFF4F,0xFF54,0xFF59,0xFF5E,0xFF63,0xFF68,0xFF6D,
                                                0xFF72,0xFF77,0xFF7C,0xFF82,0xFF87,0xFF8D,0xFF93,0xFF98,
                                                0xFF9E,0xFFA4,0xFFAA,0xFFB0,0xFFB6,0xFFBC,0xFFC2,0xFFC8,
                                                0xFFCE,0xFFD4,0xFFDA,0xFFE1,0xFFE7,0xFFED,0xFFF3,0xFFFA
                                            };

FIXED_7_8 GET_INT_FROM_FIXED_7_8_ROUND_TO_NEAREST(FIXED_7_8 A)
{
    FIXED_7_8 RetVal = 0;

    if(A&0x0080)
        RetVal = 1;

    RetVal += A>>8;
    return RetVal;
}

FIXED_7_8 FLOAT_TO_FIXED_7_8(float A)
{
    if(A>(float)((FIXED_7_8)(32767)>>8))
        return 	(float)(32767)/(256);
    else if(A<(float)((FIXED_7_8)(-32768)>>8))
        return (float)(-32768/256);
    else
        return (FIXED_7_8)(A * 256);
}

int16_t FMul_15_0to0_15(int16_t A,int16_t B)
{
    return  (((long)(A) * (long)(B))>>15);
}

int16_t FMul_7_8to0_15(int16_t A,int16_t B)
{
    return  (((long)(A) * (long)(B))>>15);
}

int16_t FMul_15_0to7_8(int16_t A,int16_t B)
{
    return  (((long)(A) * (long)(B))>>8);
}

int16_t FMul_15_0to1_14(int16_t A,int16_t B)
{
    long temp;
    temp = (((long)(A) * (long)(B))>>13);

    if(temp&0x01)
        {
            temp = temp>>1;
            temp = temp +1;
        }
    else
        {
            temp = temp>>1;
        }

    return  temp;
}

int16_t ABS(int16_t A)
{
    if(A<0)
        return -A;
    else
        return A;
}
