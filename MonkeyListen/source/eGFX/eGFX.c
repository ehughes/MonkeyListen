
#include "eGFX.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include "math.h"
#include "eGFX_DataTypes.h"


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


//Note:  All Graphics routines should call Put/Get Pixel for fundamental operations!
//Routines should not access Image storage space directly!

void eGFX_PutPixel(eGFX_ImagePlane *Image,
                   int16_t x,
                   int16_t y,
                   eGFX_PixelState PS)
{
    uint32_t Offset;
    uint8_t Mask;
    uint32_t MemWidthInBytes;

    if((x<Image->SizeX) && (y<Image->SizeY) && (x>=0) && (y>=0))
    {
        switch(Image->Type)
        {
            case  eGFX_IMAGE_PLANE_1BPP:
                //Properly Implement Rounding  for odd bit plane sizes
				MemWidthInBytes = eGFX_CALCULATE_1BPP_BUFFER_ROW_BYTE_SIZE(Image->SizeX);

                Offset = (y * (MemWidthInBytes)) + (x>>3);
                Mask = 0x01 << (x & 0x07);

                if(PS)
                {
                    Image->Data[Offset] |= Mask;
                }
                else
                {
                    Image->Data[Offset] &= ~Mask;
                }

                break;

            case  eGFX_IMAGE_PLANE_4BPP:
                //Properly Implement Rounding  for odd bit plane sizes
				MemWidthInBytes = eGFX_CALCULATE_4BPP_BUFFER_ROW_BYTE_SIZE(Image->SizeX);

                Offset = (y * (MemWidthInBytes)) + (x>>1);

                if(x&0x01)
                {
                    Image->Data[Offset] &= ~0x0F;
                    Image->Data[Offset] |= ((uint8_t)(PS&0xF));
                }
                else
                {
                    Image->Data[Offset] &= ~0xF0;
                    Image->Data[Offset] |= ((uint8_t)(PS&0xF)<<4);
                }

                break;

            case  eGFX_IMAGE_PLANE_8BPP:
                MemWidthInBytes  = Image->SizeX;
                Offset = (y * MemWidthInBytes) + x;
                Image->Data[Offset] = ((uint8_t)PS);
                break;

            case  eGFX_IMAGE_PLANE_16BPP_565:
                MemWidthInBytes  = Image->SizeX <<1;
                Offset = (y * MemWidthInBytes) + (x << 1);
                *(uint16_t *)(Image->Data + Offset) = (uint16_t)(PS);
                break;

            case  eGFX_IMAGE_PLANE_24BPP:
    
                MemWidthInBytes  = Image->SizeX * 3;
                Offset = (y * MemWidthInBytes) + (x*3);
                memcpy(&Image->Data[Offset],&PS,3);
    
            case  eGFX_IMAGE_PLANE_32BPP:
                MemWidthInBytes  = Image->SizeX << 2;
                Offset = (y * MemWidthInBytes) + (x<<2);
                *(uint32_t *)(Image->Data + Offset) = (uint32_t)(PS);
               break;
				
            default:
                break;
        }
    }
}


eGFX_PixelState eGFX_GetPixel(const eGFX_ImagePlane *Image,
                         int16_t x,
                         int16_t y)
{
	uint32_t Offset = 0;
    uint8_t Mask;
    uint32_t MemWidthInBytes;
	eGFX_PixelState PS = 0;

    if((x<Image->SizeX) && (y<Image->SizeY) && (x>=0) && (y>=0))
    {
        switch(Image->Type)
        {
            case  eGFX_IMAGE_PLANE_1BPP:
                //Properly Implement Rounding  for odd bit plane sizes
				MemWidthInBytes = eGFX_CALCULATE_1BPP_BUFFER_ROW_BYTE_SIZE(Image->SizeX);

                Offset = (y * (MemWidthInBytes)) + (x>>3);
                Mask = 0x01 << (x & 0x07);

                if(Image->Data[Offset] & Mask)
                    PS = eGFX_PIXEL_ON;
                else
                    PS = eGFX_PIXEL_OFF;

                break;

            case  eGFX_IMAGE_PLANE_4BPP:
                //Properly Implement Rounding  for odd bit plane sizes
				MemWidthInBytes = eGFX_CALCULATE_4BPP_BUFFER_ROW_BYTE_SIZE(Image->SizeX);

				Offset = (y * (MemWidthInBytes)) + (x>>1);

                if(x&0x01)
                    PS = Image->Data[Offset] & 0x0F;
                else
                    PS = ((Image->Data[Offset])>>4) & 0x0F;

                break;

            case  eGFX_IMAGE_PLANE_8BPP:
                MemWidthInBytes  = Image->SizeX;
                Offset = (y * MemWidthInBytes) + x;
                PS = Image->Data[Offset];
                break;

            case  eGFX_IMAGE_PLANE_16BPP_565:
                MemWidthInBytes  = Image->SizeX <<1;
                Offset = (y * MemWidthInBytes) + (x << 1);
                PS = *(uint16_t *)(Image->Data + Offset);
                break;
            

            case  eGFX_IMAGE_PLANE_24BPP:
                MemWidthInBytes  = Image->SizeX * 3;
                Offset = (y * MemWidthInBytes) + (x*3);
                PS  = ((int32_t)Image->Data[Offset]);
                PS |= ((int32_t)Image->Data[Offset+1])<<8;
                PS |= ((int32_t)Image->Data[Offset+2])<<16;
                break;

            case  eGFX_IMAGE_PLANE_32BPP:
                MemWidthInBytes  = Image->SizeX << 2;
                Offset = (y * MemWidthInBytes) + (x<<2);
                PS = *(uint32_t *)(Image->Data + Offset);
                break;
				

            default:
                break;
        }
    }

    return PS;
}


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
                    eGFX_PixelState PS)
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
            eGFX_PutPixel(Image,LineStart,Y,PS);
        }
        else
        {
            for(i=LineStart; i<=LineStop ; i++)
            {
                eGFX_PutPixel(Image,i,Y,PS);
            }
        }
    }
}


void eGFX_DrawFilledBox(eGFX_ImagePlane *Image,
                        eGFX_Box *Box,
                        eGFX_PixelState PS)
{
    uint16_t i;

    for(i=Box->P1.Y ; i<Box->P2.Y+1 ; i++)
    {
        eGFX_DrawHline(Image, Box->P1.X, Box->P2.X,i,PS);
    }
}


void eGFX_DrawBox(eGFX_ImagePlane *Image,
                  eGFX_Box *Box,
                  eGFX_PixelState PS)
{
    eGFX_DrawHline(Image, Box->P1.X, Box->P2.X,Box->P1.Y,PS);
    eGFX_DrawHline(Image, Box->P1.X, Box->P2.X,Box->P2.Y,PS);
    eGFX_DrawVline(Image, Box->P1.Y, Box->P2.Y,Box->P1.X,PS);
    eGFX_DrawVline(Image, Box->P1.Y, Box->P2.Y,Box->P2.X,PS);
}

void eGFX_DrawVline(eGFX_ImagePlane *Image,
                    int16_t YStart,
                    int16_t YStop,
                    int16_t X,
                    eGFX_PixelState PS)
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
            eGFX_PutPixel(Image,X,i,PS);
        }
    }
}

void eGFX_DrawLine(eGFX_ImagePlane *Image,
                   int16_t X1,
                   int16_t Y1,
                   int16_t X2,
                   int16_t Y2,
                   eGFX_PixelState PS)
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

    eGFX_PutPixel(Image, StopX,StopY, PS);

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
            eGFX_PutPixel(Image,X,Y,PS);
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
            eGFX_PutPixel(Image,X,Y,PS);

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

void eGFX_DrawListPrimitive(eGFX_ImagePlane *Image,
                            eGFX_ListPrimitive *LP ,
                            eGFX_PixelState PS)
{
    uint8_t Points;
    uint8_t i;

    if(LP->NumPoints >1)
    {
        switch(LP->DrawMode)
        {
            default:
            case eGFX_LIST_PRIMITIVE_CLOSED:
                for(i=1; i<LP->NumPoints; i++)
                {
                    eGFX_DrawLine(Image,(int16_t)(LP->PointList[i-1].X + LP->Center.X),(int16_t)(LP->PointList[i-1].Y + LP->Center.Y),
                                  (int16_t)(LP->PointList[i].X + LP->Center.X),(int16_t)(LP->PointList[i].Y + LP->Center.Y),PS);
                }

                eGFX_DrawLine(Image,(int16_t)(LP->PointList[LP->NumPoints-1].X + LP->Center.X),(int16_t)(LP->PointList[LP->NumPoints-1].Y + LP->Center.Y),
                              (int16_t)(LP->PointList[0].X + LP->Center.X),(int16_t)(LP->PointList[0].Y + LP->Center.Y),PS);
                break;

            case eGFX_LIST_PRIMITIVE_CONNECTED:
                for(i=1; i<LP->NumPoints; i++)
                {
                    eGFX_DrawLine(Image,(int16_t)(LP->PointList[i-1].X + LP->Center.X),(int16_t)(LP->PointList[i-1].Y + LP->Center.Y),
                                  (int16_t)(LP->PointList[i].X + LP->Center.X),(int16_t)(LP->PointList[i].Y + LP->Center.Y),PS);
                }

                break;

            case eGFX_LIST_PRIMITIVE_DISCONNECTED:
                Points = LP->NumPoints>>1;

                for(i=0; i<Points; i++)
                {
                    eGFX_DrawLine(Image,(int16_t)(LP->PointList[i*2].X + LP->Center.X),(int16_t)(LP->PointList[i*2].Y + LP->Center.Y),
                                  (int16_t)(LP->PointList[i*2+1].X + LP->Center.X),(int16_t)(LP->PointList[i*2+1].Y + LP->Center.Y),PS);
                }

                break;

            case eGFX_LIST_PRIMITIVE_CLOSED_YFLIPPED:
                for(i=1; i<LP->NumPoints; i++)
                {
                    eGFX_DrawLine(Image,(int16_t)(LP->PointList[i-1].X + LP->Center.X),(int16_t)(-LP->PointList[i-1].Y + LP->Center.Y),
                                  (int16_t)(LP->PointList[i].X + LP->Center.X),(int16_t)(-LP->PointList[i].Y + LP->Center.Y),PS);
                }

                eGFX_DrawLine(Image,(int16_t)(LP->PointList[LP->NumPoints-1].X + LP->Center.X),(int16_t)(-LP->PointList[LP->NumPoints-1].Y + LP->Center.Y),
                              (int16_t)(LP->PointList[0].X + LP->Center.X),(int16_t)(-LP->PointList[0].Y + LP->Center.Y),PS);
                break;

            case eGFX_LIST_PRIMITIVE_CONNECTED_YFLIPPED:
                for(i=1; i<LP->NumPoints; i++)
                {
                    eGFX_DrawLine(Image,(int16_t)(LP->PointList[i-1].X + LP->Center.X),(int16_t)(-LP->PointList[i-1].Y + LP->Center.Y),
                                  (int16_t)(LP->PointList[i].X + LP->Center.X),(int16_t)(-LP->PointList[i].Y + LP->Center.Y),PS);
                }

                break;

            case eGFX_LIST_PRIMITIVE_DISCONNECTED_YFLIPPED:
                Points = LP->NumPoints>>1;

                for(i=0; i<Points; i++)
                {
                    eGFX_DrawLine(Image,(int16_t)(LP->PointList[i*2].X + LP->Center.X),(int16_t)(-LP->PointList[i*2].Y + LP->Center.Y),
                                  (int16_t)(LP->PointList[i*2+1].X + LP->Center.X),(int16_t)(-LP->PointList[i*2+1].Y + LP->Center.Y),PS);
                }

                break;
        }
    }
}

void eGFX_DrawScaledListPrimitive(eGFX_ImagePlane *Image,
                                  eGFX_ListPrimitive *LP ,
                                  FIXED_7_8 Scale,
                                  eGFX_PixelState PS)
{
    uint8_t Points;
    uint8_t i;
    eGFX_Point TransformedPoint[2];

    if(LP->NumPoints >1)
    {
        switch(LP->DrawMode)
        {
            default:
            case eGFX_LIST_PRIMITIVE_CLOSED:
                for(i=1; i<LP->NumPoints; i++)
                {
                    TransformedPoint[0].X = eGFX_FMul_15_0to7_8(LP->PointList[i-1].X,Scale) 	+ LP->Center.X;
                    TransformedPoint[0].Y = eGFX_FMul_15_0to7_8(LP->PointList[i-1].Y,Scale)   + LP->Center.Y;
                    TransformedPoint[1].X = eGFX_FMul_15_0to7_8(LP->PointList[i].X,Scale)     + LP->Center.X;
                    TransformedPoint[1].Y = eGFX_FMul_15_0to7_8(LP->PointList[i].Y,Scale)     + LP->Center.Y;
                    eGFX_DrawLine(Image,TransformedPoint[0].X,TransformedPoint[0].Y,
                                  TransformedPoint[1].X,TransformedPoint[1].Y,PS);
                }

                TransformedPoint[0].X = eGFX_FMul_15_0to7_8(LP->PointList[LP->NumPoints-1].X,Scale)   + LP->Center.X;
                TransformedPoint[0].Y = eGFX_FMul_15_0to7_8(LP->PointList[LP->NumPoints-1].Y,Scale)   + LP->Center.Y;
                TransformedPoint[1].X = eGFX_FMul_15_0to7_8(LP->PointList[0].X,Scale)				 + LP->Center.X;
                TransformedPoint[1].Y = eGFX_FMul_15_0to7_8(LP->PointList[0].Y,Scale)				 + LP->Center.Y;
                eGFX_DrawLine(Image,TransformedPoint[0].X,TransformedPoint[0].Y,
                              TransformedPoint[1].X,TransformedPoint[1].Y,PS);
                break;

            case eGFX_LIST_PRIMITIVE_CONNECTED:
                for(i=1; i<LP->NumPoints; i++)
                {
                    TransformedPoint[0].X = eGFX_FMul_15_0to7_8(LP->PointList[i-1].X,Scale) 	+ LP->Center.X;
                    TransformedPoint[0].Y = eGFX_FMul_15_0to7_8(LP->PointList[i-1].Y,Scale)   + LP->Center.Y;
                    TransformedPoint[1].X = eGFX_FMul_15_0to7_8(LP->PointList[i].X,Scale)     + LP->Center.X;
                    TransformedPoint[1].Y = eGFX_FMul_15_0to7_8(LP->PointList[i].Y,Scale)     + LP->Center.Y;
                    eGFX_DrawLine(Image,TransformedPoint[0].X,TransformedPoint[0].Y,
                                  TransformedPoint[1].X,TransformedPoint[1].Y,PS);
                }

                break;

            case eGFX_LIST_PRIMITIVE_DISCONNECTED:
                Points = LP->NumPoints>>1;

                for(i=0; i<Points; i++)
                {
                    TransformedPoint[0].X = eGFX_FMul_15_0to7_8(LP->PointList[i*2].X,Scale)		+ LP->Center.X;
                    TransformedPoint[0].Y = eGFX_FMul_15_0to7_8(LP->PointList[i*2].Y,Scale)		+ LP->Center.Y;
                    TransformedPoint[1].X = eGFX_FMul_15_0to7_8(LP->PointList[(i*2)+1].X,Scale)   + LP->Center.X;
                    TransformedPoint[1].Y = eGFX_FMul_15_0to7_8(LP->PointList[(i*2)+1].Y,Scale)   + LP->Center.Y;
                    eGFX_DrawLine(Image,TransformedPoint[0].X,TransformedPoint[0].Y,
                                  TransformedPoint[1].X,TransformedPoint[1].Y,PS);
                }

                break;
        }
    }
}

void eGFX_DrawRotatedListPrimitive(eGFX_ImagePlane *Image,
                                   eGFX_ListPrimitive *LP ,
                                   uint8_t Angle,
                                   eGFX_PixelState PS)
{
    uint8_t Points;
    uint8_t i;
    eGFX_Point TransformedPoint[2];

    if(LP->NumPoints >1)
    {
        //If we are drawing non connected points,  there must be an even number of them.
        switch(LP->DrawMode)
        {
            case eGFX_LIST_PRIMITIVE_DISCONNECTED:
                Points = LP->NumPoints>>1;

                for(i=0; i<Points; i++)
                {
                    TransformedPoint[0].X = eGFX_FMul_15_0to1_14((LP->PointList[i*2].X), eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                            eGFX_FMul_15_0to1_14(LP->PointList[i*2].Y, eGFX_Fixed_1_14_SineTable[Angle]);
                    TransformedPoint[0].Y = eGFX_FMul_15_0to1_14(LP->PointList[i*2].X, eGFX_Fixed_1_14_SineTable[Angle ]) +
                                            eGFX_FMul_15_0to1_14(LP->PointList[i*2].Y, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]);
                    TransformedPoint[1].X = eGFX_FMul_15_0to1_14(LP->PointList[(i*2)+1].X, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                            eGFX_FMul_15_0to1_14(LP->PointList[(i*2)+1].Y, eGFX_Fixed_1_14_SineTable[Angle]);
                    TransformedPoint[1].Y = eGFX_FMul_15_0to1_14(LP->PointList[(i*2)+1].X, eGFX_Fixed_1_14_SineTable[Angle]) +
                                            eGFX_FMul_15_0to1_14(LP->PointList[(i*2)+1].Y, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]);
                    TransformedPoint[0].X += LP->Center.X;
                    TransformedPoint[0].Y += LP->Center.Y;
                    TransformedPoint[1].X += LP->Center.X;
                    TransformedPoint[1].Y += LP->Center.Y;
                    eGFX_DrawLine(Image,(TransformedPoint[0].X + LP->Center.X),(TransformedPoint[0].Y  + LP->Center.Y),
                                  (TransformedPoint[1].X + LP->Center.X),(TransformedPoint[1].Y + LP->Center.Y),PS);
                }

                break;

            case eGFX_LIST_PRIMITIVE_CONNECTED:
                for(i=1; i<LP->NumPoints; i++)
                {
                    TransformedPoint[0].X = (eGFX_FMul_15_0to1_14(LP->PointList[i-1].X, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                             eGFX_FMul_15_0to1_14(LP->PointList[i-1].Y, eGFX_Fixed_1_14_SineTable[Angle])  );
                    TransformedPoint[0].Y = ( eGFX_FMul_15_0to1_14(LP->PointList[i-1].X, eGFX_Fixed_1_14_SineTable[Angle]) +
                                              eGFX_FMul_15_0to1_14(LP->PointList[i-1].Y, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff] ) );
                    TransformedPoint[1].X = (eGFX_FMul_15_0to1_14(LP->PointList[i].X, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                             eGFX_FMul_15_0to1_14(LP->PointList[i].Y, eGFX_Fixed_1_14_SineTable[Angle]) );
                    TransformedPoint[1].Y = (eGFX_FMul_15_0to1_14(LP->PointList[i].X, eGFX_Fixed_1_14_SineTable[Angle]) +
                                             eGFX_FMul_15_0to1_14(LP->PointList[i].Y, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]));
                    TransformedPoint[0].X +=  LP->Center.X;
                    TransformedPoint[0].Y +=  LP->Center.Y;
                    TransformedPoint[1].X +=  LP->Center.X;
                    TransformedPoint[1].Y +=  LP->Center.Y;
                    eGFX_DrawLine(Image,(TransformedPoint[0].X),(TransformedPoint[0].Y),
                                  (TransformedPoint[1].X),(TransformedPoint[1].Y),PS);
                }

                break;

            default:
            case eGFX_LIST_PRIMITIVE_CLOSED:
                for(i=1; i<LP->NumPoints; i++)
                {
                    TransformedPoint[0].X = (eGFX_FMul_15_0to1_14(LP->PointList[i-1].X, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                             eGFX_FMul_15_0to1_14(LP->PointList[i-1].Y, eGFX_Fixed_1_14_SineTable[Angle])  );
                    TransformedPoint[0].Y = ( eGFX_FMul_15_0to1_14(LP->PointList[i-1].X, eGFX_Fixed_1_14_SineTable[Angle]) +
                                              eGFX_FMul_15_0to1_14(LP->PointList[i-1].Y, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff] ) );
                    TransformedPoint[1].X = (eGFX_FMul_15_0to1_14(LP->PointList[i].X, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                             eGFX_FMul_15_0to1_14(LP->PointList[i].Y, eGFX_Fixed_1_14_SineTable[Angle]) );
                    TransformedPoint[1].Y = (eGFX_FMul_15_0to1_14(LP->PointList[i].X, eGFX_Fixed_1_14_SineTable[Angle]) +
                                             eGFX_FMul_15_0to1_14(LP->PointList[i].Y, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]));
                    TransformedPoint[0].X +=  LP->Center.X;
                    TransformedPoint[0].Y +=  LP->Center.Y;
                    TransformedPoint[1].X +=  LP->Center.X;
                    TransformedPoint[1].Y +=  LP->Center.Y;
                    eGFX_DrawLine(Image,(TransformedPoint[0].X),(TransformedPoint[0].Y),
                                  (TransformedPoint[1].X),(TransformedPoint[1].Y),PS);
                }

                TransformedPoint[0].X = (eGFX_FMul_15_0to1_14(LP->PointList[LP->NumPoints-1].X, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                         eGFX_FMul_15_0to1_14(LP->PointList[LP->NumPoints-1].Y, eGFX_Fixed_1_14_SineTable[Angle])  );
                TransformedPoint[0].Y = ( eGFX_FMul_15_0to1_14(LP->PointList[LP->NumPoints-1].X, eGFX_Fixed_1_14_SineTable[Angle]) +
                                          eGFX_FMul_15_0to1_14(LP->PointList[LP->NumPoints-1].Y, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff] ) );
                TransformedPoint[1].X = (eGFX_FMul_15_0to1_14(LP->PointList[0].X, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                         eGFX_FMul_15_0to1_14(LP->PointList[0].Y, eGFX_Fixed_1_14_SineTable[Angle]) );
                TransformedPoint[1].Y = (eGFX_FMul_15_0to1_14(LP->PointList[0].X, eGFX_Fixed_1_14_SineTable[Angle]) +
                                         eGFX_FMul_15_0to1_14(LP->PointList[0].Y, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]));
                TransformedPoint[0].X +=  LP->Center.X;
                TransformedPoint[0].Y +=  LP->Center.Y;
                TransformedPoint[1].X +=  LP->Center.X;
                TransformedPoint[1].Y +=  LP->Center.Y;
                eGFX_DrawLine(Image,(TransformedPoint[0].X),(TransformedPoint[0].Y),
                              (TransformedPoint[1].X),(TransformedPoint[1].Y),PS);
                break;
        }
    }
}

void eGFX_DrawScaledRotatedListPrimitive(eGFX_ImagePlane *Image,
        eGFX_ListPrimitive *LP ,
        uint8_t Angle,
        FIXED_7_8 Scale,
        eGFX_PixelState PS)
{
    uint8_t i;
    eGFX_Point TransformedPoint[2];

    if(LP->NumPoints >1)
    {
        switch(LP->DrawMode)
        {
            default:
            case eGFX_LIST_PRIMITIVE_CLOSED:
                for(i=1; i<LP->NumPoints; i++)
                {
                    TransformedPoint[0].X = (eGFX_FMul_15_0to1_14(LP->PointList[i-1].X, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                             eGFX_FMul_15_0to1_14(LP->PointList[i-1].Y, eGFX_Fixed_1_14_SineTable[Angle])  );
                    TransformedPoint[0].Y = ( eGFX_FMul_15_0to1_14(LP->PointList[i-1].X, eGFX_Fixed_1_14_SineTable[Angle]) +
                                              eGFX_FMul_15_0to1_14(LP->PointList[i-1].Y, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff] ) );
                    TransformedPoint[1].X = (eGFX_FMul_15_0to1_14(LP->PointList[i].X, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                             eGFX_FMul_15_0to1_14(LP->PointList[i].Y, eGFX_Fixed_1_14_SineTable[Angle]) );
                    TransformedPoint[1].Y = (eGFX_FMul_15_0to1_14(LP->PointList[i].X, eGFX_Fixed_1_14_SineTable[Angle]) +
                                             eGFX_FMul_15_0to1_14(LP->PointList[i].Y, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]));
                    TransformedPoint[0].X = eGFX_FMul_15_0to7_8(TransformedPoint[0].X,Scale)  +    LP->Center.X;
                    TransformedPoint[0].Y = eGFX_FMul_15_0to7_8(TransformedPoint[0].Y,Scale)  +    LP->Center.Y;
                    TransformedPoint[1].X = eGFX_FMul_15_0to7_8(TransformedPoint[1].X,Scale)  +    LP->Center.X;
                    TransformedPoint[1].Y = eGFX_FMul_15_0to7_8(TransformedPoint[1].Y,Scale)  +    LP->Center.Y;
                    eGFX_DrawLine(Image,(TransformedPoint[0].X),(TransformedPoint[0].Y),
                                  (TransformedPoint[1].X),(TransformedPoint[1].Y),PS);
                }

                TransformedPoint[0].X = (eGFX_FMul_15_0to1_14(LP->PointList[LP->NumPoints-1].X, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                         eGFX_FMul_15_0to1_14(LP->PointList[LP->NumPoints-1].Y, eGFX_Fixed_1_14_SineTable[Angle])  );
                TransformedPoint[0].Y = ( eGFX_FMul_15_0to1_14(LP->PointList[LP->NumPoints-1].X, eGFX_Fixed_1_14_SineTable[Angle]) +
                                          eGFX_FMul_15_0to1_14(LP->PointList[LP->NumPoints-1].Y, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff] ) );
                TransformedPoint[1].X = (eGFX_FMul_15_0to1_14(LP->PointList[0].X, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                         eGFX_FMul_15_0to1_14(LP->PointList[0].Y, eGFX_Fixed_1_14_SineTable[Angle]) );
                TransformedPoint[1].Y = (eGFX_FMul_15_0to1_14(LP->PointList[0].X, eGFX_Fixed_1_14_SineTable[Angle]) +
                                         eGFX_FMul_15_0to1_14(LP->PointList[0].Y, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]));
                TransformedPoint[0].X = eGFX_FMul_15_0to7_8(TransformedPoint[0].X,Scale)  +    LP->Center.X;
                TransformedPoint[0].Y = eGFX_FMul_15_0to7_8(TransformedPoint[0].Y,Scale)  +    LP->Center.Y;
                TransformedPoint[1].X = eGFX_FMul_15_0to7_8(TransformedPoint[1].X,Scale)  +    LP->Center.X;
                TransformedPoint[1].Y = eGFX_FMul_15_0to7_8(TransformedPoint[1].Y,Scale)  +    LP->Center.Y;
                eGFX_DrawLine(Image,(TransformedPoint[0].X),(TransformedPoint[0].Y),
                              (TransformedPoint[1].X),(TransformedPoint[1].Y),PS);
                break;

            case eGFX_LIST_PRIMITIVE_DISCONNECTED:
                for(i=0; i<LP->NumPoints>>1; i++)
                {
                    TransformedPoint[0].X = (eGFX_FMul_15_0to1_14(LP->PointList[i*2].X, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                             eGFX_FMul_15_0to1_14(LP->PointList[i*2].Y, eGFX_Fixed_1_14_SineTable[Angle])  );
                    TransformedPoint[0].Y = ( eGFX_FMul_15_0to1_14(LP->PointList[i*2].X, eGFX_Fixed_1_14_SineTable[Angle]) +
                                              eGFX_FMul_15_0to1_14(LP->PointList[i*2].Y, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff] ) );
                    TransformedPoint[1].X = (eGFX_FMul_15_0to1_14(LP->PointList[i*2+1].X, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                             eGFX_FMul_15_0to1_14(LP->PointList[i*2+1].Y, eGFX_Fixed_1_14_SineTable[Angle]) );
                    TransformedPoint[1].Y = (eGFX_FMul_15_0to1_14(LP->PointList[i*2+1].X, eGFX_Fixed_1_14_SineTable[Angle]) +
                                             eGFX_FMul_15_0to1_14(LP->PointList[i*2+1].Y, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]));
                    TransformedPoint[0].X = eGFX_FMul_15_0to7_8(TransformedPoint[0].X,Scale)  +    LP->Center.X;
                    TransformedPoint[0].Y = eGFX_FMul_15_0to7_8(TransformedPoint[0].Y,Scale)  +    LP->Center.Y;
                    TransformedPoint[1].X = eGFX_FMul_15_0to7_8(TransformedPoint[1].X,Scale)  +    LP->Center.X;
                    TransformedPoint[1].Y = eGFX_FMul_15_0to7_8(TransformedPoint[1].Y,Scale)  +    LP->Center.Y;
                    eGFX_DrawLine(Image,(TransformedPoint[0].X),(TransformedPoint[0].Y),
                                  (TransformedPoint[1].X),(TransformedPoint[1].Y),PS);
                }

                break;

            case eGFX_LIST_PRIMITIVE_CONNECTED:
                for(i=1; i<LP->NumPoints; i++)
                {
                    TransformedPoint[0].X = (eGFX_FMul_15_0to1_14(LP->PointList[i-1].X, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                             eGFX_FMul_15_0to1_14(LP->PointList[i-1].Y, eGFX_Fixed_1_14_SineTable[Angle])  );
                    TransformedPoint[0].Y = ( eGFX_FMul_15_0to1_14(LP->PointList[i-1].X, eGFX_Fixed_1_14_SineTable[Angle]) +
                                              eGFX_FMul_15_0to1_14(LP->PointList[i-1].Y, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff] ) );
                    TransformedPoint[1].X = (eGFX_FMul_15_0to1_14(LP->PointList[i].X, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]) -
                                             eGFX_FMul_15_0to1_14(LP->PointList[i].Y, eGFX_Fixed_1_14_SineTable[Angle]) );
                    TransformedPoint[1].Y = (eGFX_FMul_15_0to1_14(LP->PointList[i].X, eGFX_Fixed_1_14_SineTable[Angle]) +
                                             eGFX_FMul_15_0to1_14(LP->PointList[i].Y, eGFX_Fixed_1_14_SineTable[(Angle + 64)&0xff]));
                    TransformedPoint[0].X = eGFX_FMul_15_0to7_8(TransformedPoint[0].X,Scale)  +    LP->Center.X;
                    TransformedPoint[0].Y = eGFX_FMul_15_0to7_8(TransformedPoint[0].Y,Scale)  +    LP->Center.Y;
                    TransformedPoint[1].X = eGFX_FMul_15_0to7_8(TransformedPoint[1].X,Scale)  +    LP->Center.X;
                    TransformedPoint[1].Y = eGFX_FMul_15_0to7_8(TransformedPoint[1].Y,Scale)  +    LP->Center.Y;
                    eGFX_DrawLine(Image,(TransformedPoint[0].X),(TransformedPoint[0].Y),
                                  (TransformedPoint[1].X),(TransformedPoint[1].Y),PS);
                }

                break;
        }
    }
}

// midpoint circle algorithm
void eGFX_DrawCircle(eGFX_ImagePlane *RC,
                     int16_t x0,
                     int16_t y0,
                     int16_t radius,
                     eGFX_PixelState PS)
{
    int16_t f = 1 - radius;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * radius;
    int16_t x = 0;
    int16_t y = radius;

	if (radius == 0)
		return;

    eGFX_PutPixel(RC, x0, y0 + radius, PS);
    eGFX_PutPixel(RC, x0, y0 - radius, PS);
    eGFX_PutPixel(RC, x0 + radius, y0, PS);
    eGFX_PutPixel(RC, x0 - radius, y0, PS);


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
        eGFX_PutPixel(RC, x0 + x, y0 + y, PS);
        eGFX_PutPixel(RC, x0 - x, y0 + y, PS);
        eGFX_PutPixel(RC, x0 + x, y0 - y, PS);
        eGFX_PutPixel(RC, x0 - x, y0 - y, PS);
        eGFX_PutPixel(RC, x0 + y, y0 + x, PS);
        eGFX_PutPixel(RC, x0 - y, y0 + x, PS);
        eGFX_PutPixel(RC, x0 + y, y0 - x, PS);
        eGFX_PutPixel(RC, x0 - y, y0 - x, PS);
    }
}

void eGFX_DrawFilledCircle(eGFX_ImagePlane *RC,
	int16_t x0,
	int16_t y0,
	int16_t radius,
	eGFX_PixelState PS)
{
	int16_t f = 1 - radius;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * radius;
	int16_t x = 0;
	int16_t y = radius;

	if (radius == 0)
		return;

	eGFX_DrawHline(RC, x0 - radius, x0 + radius , y0, PS);

	eGFX_DrawVline(RC, y0 - radius , y0 + radius , x0, PS);

	while (x < y)
	{
		// ddF_x == 2 * x + 1;
		// ddF_y == -2 * y;
		// f == x*x + y*y - radius*radius + 2*x - y + 1;
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}

		x++;
		ddF_x += 2;
		f += ddF_x;
		
		eGFX_DrawHline(RC, x0 - x, x0 + x, y0 + y, PS);

		eGFX_DrawHline(RC, x0 - x, x0 + x, y0 - y, PS);


		eGFX_DrawHline(RC, x0 - y, x0 + y, y0 + x, PS);

		eGFX_DrawHline(RC, x0 - y, x0 + y, y0 - x, PS);

//		eGFX_PutPixel(RC, x0 + y, y0 + x, PS);
	//	eGFX_PutPixel(RC, x0 - y, y0 + x, PS);
	//	eGFX_PutPixel(RC, x0 + y, y0 - x, PS);
	//	eGFX_PutPixel(RC, x0 - y, y0 - x, PS);
	}
}

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

static char eGFX_StringBuf[eGFX_MAX_PRINTF_BUF_LENGTH];

int16_t eGFX_ScaledDrawCharacter(eGFX_ImagePlane *Image,
								int16_t StartX,
								int16_t StartY,
								uint8_t Character,
								const eGFX_Font *MyFont,
								float ScaleFactor)
                                 
{
    //eGFX_Font does not contain any of the ASCII control codes
    if(Character<0x20)
        return StartX;

    Character = Character - 0x20;

    if(Character == 0x00)
    {
        return StartX +  (uint16_t)((MyFont->CharacterSprites[0]->SizeX * ScaleFactor)+0.5); // No need to render anything....
    }
    else
    {
        eGFX_ScaledBlit(Image,StartX,StartY,MyFont->CharacterSprites[Character],ScaleFactor);
    }

    StartX += MyFont->CharacterSprites[Character]->SizeX * (int16_t)(ScaleFactor+.5);
    return StartX;
}

int16_t eGFX_DrawCharacter(eGFX_ImagePlane *Image,
	int16_t StartX,
	int16_t StartY,
	uint8_t Character,
	const eGFX_Font *MyFont)
{
    //eGFX_Font does not contain any of the ASCII control codes
    if(Character<0x20)
        return StartX;

    Character = Character - 0x20;

    if(Character == 0x00)
    {
		return StartX + (MyFont->CharacterSprites[0]->SizeX);
	}
    else
    {
        eGFX_Blit(Image,StartX,StartY,MyFont->CharacterSprites[Character]);
    }

    StartX += MyFont->CharacterSprites[Character]->SizeX;
    return StartX;
}

int16_t eGFX_DrawCharacterFaded(eGFX_ImagePlane *Image,
                           int16_t StartX,
                           int16_t StartY,
                           uint8_t Character,
                           const eGFX_Font *MyFont,
                           eGFX_PixelState PS)
{
    //eGFX_Font does not contain any of the ASCII control codes
    if(Character<0x20)
        return StartX;

    Character = Character - 0x20;

    if(Character == 0x00)
    {
		return StartX + (MyFont->CharacterSprites[0]->SizeX);
	}
    else
    {
        eGFX_BlitFaded(Image,StartX,StartY,MyFont->CharacterSprites[Character],PS);
    }

    StartX += MyFont->CharacterSprites[Character]->SizeX;
    return StartX;
}


int16_t eGFX_DrawCharacterColored(eGFX_ImagePlane *Image,
                           int16_t StartX,
                           int16_t StartY,
                           uint8_t Character,
                           const eGFX_Font *MyFont,
                           eGFX_PixelState Color)
{
    //eGFX_Font does not contain any of the ASCII control codes
    if(Character<0x20)
        return StartX;

    Character = Character - 0x20;

    if(Character == 0x00)
    {
		return StartX + (MyFont->CharacterSprites[0]->SizeX);
	}
    else
    {
        eGFX_BlitColored(Image,StartX,StartY,MyFont->CharacterSprites[Character],Color);
    }

    StartX += MyFont->CharacterSprites[Character]->SizeX;
    return StartX;
}


int16_t eGFX_DrawCharacterShaded(eGFX_ImagePlane *Image,
	int16_t StartX,
	int16_t StartY,
	uint8_t Character,
	const eGFX_Font *MyFont,
	float Shading)
{
	//eGFX_Font does not contain any of the ASCII control codes
	if (Character<0x20)
		return StartX;

	Character = Character - 0x20;

	if (Character == 0x00)
	{
		return StartX + (MyFont->CharacterSprites[0]->SizeX);
	}
	else
	{
		eGFX_BlitShaded(Image, StartX, StartY, MyFont->CharacterSprites[Character], Shading);
	}

	StartX += MyFont->CharacterSprites[Character]->SizeX;
	return StartX;
}

int16_t eGFX_GetStringWidth(char *String,
                            const eGFX_Font *MyFont)
{
    uint8_t Ptr = 0;
    uint8_t NextChar;
    int16_t StringSize = 0;
    NextChar = String[Ptr];
    Ptr++;

    while((NextChar!=0) && (Ptr <eGFX_MAX_STRING_LEN))
    {
        if(NextChar>=0x20 && NextChar<0x80)
        {
            //Remember that we don't endcode the wide space characters in the FONT table!
            StringSize += MyFont->CharacterSprites[NextChar - 0x20]->SizeX + MyFont->InterCharacterSpacing;
            NextChar = String[Ptr];
            Ptr++;
        }
    }

    return StringSize - MyFont->InterCharacterSpacing; //Remove the last intra character space!
}


int16_t eGFX_GetStringWidth_CustomSpacing(char *String,
        const eGFX_Font *MyFont,
        uint16_t Spacing)
{
    uint8_t Ptr = 0;
    uint8_t NextChar;
    int16_t StringSize = 0;
    NextChar = String[Ptr];
    Ptr++;

    while((NextChar!=0) && (Ptr <eGFX_MAX_STRING_LEN))
    {
        if(NextChar>=0x20 && NextChar<0x80)
        {
            StringSize += MyFont->CharacterSprites[NextChar - 0x20]->SizeX + Spacing;
            NextChar = String[Ptr];
            Ptr++;
        }
    }

    return StringSize - Spacing; //Remove the last inter character space!
}


void eGFX_DrawHorizontalCenteredString(eGFX_ImagePlane *Image,
										int16_t StartY,
										char *String,
										const eGFX_Font *MyFont)
{
    int16_t StartX = 0;
    StartX = ((int16_t)Image->SizeX - eGFX_GetStringWidth(String,MyFont))>>1;
    eGFX_DrawString(Image,String,StartX,StartY,MyFont);
}



int16_t eGFX_DrawRightAlignedCenteredString_CustomSpacing(eGFX_ImagePlane *Image,
        int16_t StartY,
        char *String,
        const eGFX_Font *MyFont,
        uint16_t CustomCharacterSpacing,
        int16_t RightSideBuffer)
{
    int16_t StartX = 0;
    StartX = ((int16_t)Image->SizeX - eGFX_GetStringWidth_CustomSpacing(String,MyFont,CustomCharacterSpacing) - RightSideBuffer);
    eGFX_DrawString_CustomSpacing(Image,StartX,StartY,String,MyFont,CustomCharacterSpacing);
    return StartX;
}

void eGFX_DrawHorizontalCenteredString_CustomSpacing(eGFX_ImagePlane *Image,
        int16_t StartY,
        char *String,
        const eGFX_Font *MyFont,
        uint16_t Spacing)
{
    int16_t StartX = 0;
    StartX = ((int16_t)Image->SizeX - eGFX_GetStringWidth_CustomSpacing(String,MyFont,Spacing))>>1;
    eGFX_DrawString_CustomSpacing(Image,StartX,StartY,String,MyFont,Spacing);
}


void eGFX_ScaledDrawString(eGFX_ImagePlane *Image,
	int16_t StartX,
	int16_t StartY,
	char *String,
	const eGFX_Font *MyFont,
	float ScaleFactor)
{
    uint8_t Ptr = 0;
    uint8_t NextChar;
    NextChar = String[Ptr];

    while((NextChar!=0) && (Ptr <eGFX_MAX_STRING_LEN))
    {
        StartX = eGFX_ScaledDrawCharacter(Image,StartX,StartY,NextChar,MyFont,ScaleFactor);
        StartX += MyFont->InterCharacterSpacing * (int16_t)(ScaleFactor+.5);
        Ptr++;
        NextChar = String[Ptr];
    }
}


void eGFX_ScaledDrawString_CustomSpacing(eGFX_ImagePlane *Image,
	int16_t StartX,
	int16_t StartY,
	char *String,
	const eGFX_Font *MyFont,
	float ScaleFactor,
	uint16_t Spacing)
{
    uint8_t Ptr = 0;
    uint8_t NextChar;
    NextChar = String[Ptr];

    while((NextChar!=0) && (Ptr <eGFX_MAX_STRING_LEN))
    {
        StartX = eGFX_ScaledDrawCharacter(Image,StartX,StartY,NextChar,MyFont,ScaleFactor);
        StartX += (int16_t)((ScaleFactor * Spacing) +.5);
        Ptr++;
        NextChar = String[Ptr];
    }
}

int16_t  eGFX_DrawString(eGFX_ImagePlane *Image,
	char *String,
	int16_t StartX,
	int16_t StartY,

	const eGFX_Font *MyFont)
{
    uint8_t Ptr = 0;
    uint8_t NextChar;
    NextChar = String[Ptr];

    while((NextChar!=0) && (Ptr <eGFX_MAX_STRING_LEN))
    {
        StartX = eGFX_DrawCharacter(Image,StartX,StartY,NextChar,MyFont);
        StartX += MyFont->InterCharacterSpacing;
        Ptr++;
        NextChar = String[Ptr];
    }

    return StartX + 1;
}

int16_t  eGFX_DrawStringFaded(eGFX_ImagePlane *Image,
	char *String,
	int16_t StartX,
	int16_t StartY,
	const eGFX_Font *MyFont, 
	eGFX_PixelState PS)
{
    uint8_t Ptr = 0;
    uint8_t NextChar;
    NextChar = String[Ptr];

    while((NextChar!=0) && (Ptr <eGFX_MAX_STRING_LEN))
    {
        StartX = eGFX_DrawCharacterFaded(Image,StartX,StartY,NextChar,MyFont,PS);
        StartX += MyFont->InterCharacterSpacing;
        Ptr++;
        NextChar = String[Ptr];
    }

    return StartX + 1;
}

int16_t  eGFX_DrawStringShaded(eGFX_ImagePlane *Image,
	char *String,
	int16_t StartX,
	int16_t StartY,
	const eGFX_Font *MyFont,
	float Shading)
{
	uint8_t Ptr = 0;
	uint8_t NextChar;
	NextChar = String[Ptr];

	while ((NextChar != 0) && (Ptr <eGFX_MAX_STRING_LEN))
	{
		StartX = eGFX_DrawCharacterShaded(Image, StartX, StartY, NextChar, MyFont, Shading);
		StartX += MyFont->InterCharacterSpacing;
		Ptr++;
		NextChar = String[Ptr];
	}

	return StartX + 1;
}

int16_t  eGFX_DrawStringColored(eGFX_ImagePlane *Image,
	char *String,
	int16_t StartX,
	int16_t StartY,
	const eGFX_Font *MyFont,
	uint32_t Color)
{
	uint8_t Ptr = 0;
	uint8_t NextChar;
	NextChar = String[Ptr];

	while ((NextChar != 0) && (Ptr <eGFX_MAX_STRING_LEN))
	{
		StartX = eGFX_DrawCharacterColored(Image, StartX, StartY, NextChar, MyFont, Color);
		StartX += MyFont->InterCharacterSpacing;
		Ptr++;
		NextChar = String[Ptr];
	}

	return StartX + 1;
}


int16_t  eGFX_printf(eGFX_ImagePlane *Image,
                     int16_t StartX,
                     int16_t StartY,
                     const eGFX_Font *MyFont,
                     char *FormatString,...)
{
    int16_t End;
    va_list argptr;
    va_start(argptr,FormatString);
#ifdef WIN32
    sprintf_s((char *)eGFX_StringBuf,eGFX_MAX_PRINTF_BUF_LENGTH,FormatString,argptr);
#else
    vsnprintf((char *)eGFX_StringBuf,eGFX_MAX_PRINTF_BUF_LENGTH,FormatString,argptr);
#endif
    va_end(argptr);
    End = StartX + eGFX_GetStringWidth(eGFX_StringBuf,MyFont) + 1;
	eGFX_DrawString(Image, eGFX_StringBuf, StartX, StartY, MyFont);
    return End;
}

int16_t  eGFX_printf_HorizontalCentered_Colored(eGFX_ImagePlane *Image,
                                         int16_t StartY,
                                        const eGFX_Font *MyFont,
                                        uint32_t Color,
                                        char *FormatString,...)
{
    int16_t End;
    int16_t StartX;
    va_list argptr;
    va_start(argptr,FormatString);
#ifdef WIN32
    sprintf_s((char *)eGFX_StringBuf,eGFX_MAX_PRINTF_BUF_LENGTH,FormatString,argptr);
#else
    vsnprintf((char *)eGFX_StringBuf,eGFX_MAX_PRINTF_BUF_LENGTH,FormatString,argptr);
#endif
    va_end(argptr);
    
    StartX = ((int16_t)Image->SizeX - eGFX_GetStringWidth(eGFX_StringBuf,MyFont))>>1;
    End = StartX + eGFX_GetStringWidth(eGFX_StringBuf,MyFont) + 1;
    
	eGFX_DrawStringColored(Image, eGFX_StringBuf, StartX, StartY, MyFont,Color);
    return End;
}



int16_t eGFX_DrawString_CustomSpacing(eGFX_ImagePlane *Image,
                                      int16_t StartX,
                                      int16_t StartY,
                                      char *String,
                                      const eGFX_Font *MyFont,
                                      uint16_t Spacing)
{
    uint8_t Ptr = 0;
    uint8_t NextChar;
    NextChar = String[Ptr];

    while((NextChar!=0) && (Ptr <eGFX_MAX_STRING_LEN))
    {
        StartX = eGFX_DrawCharacter(Image,StartX,StartY,NextChar,MyFont);
        StartX += Spacing;
        Ptr++;
        NextChar = String[Ptr];
    }

    return StartX+1;
}

//x,y is a the upper left corner of the blit, not the center
void eGFX_Blit(eGFX_ImagePlane *Destination,
	int16_t x,
	int16_t y,
	const eGFX_ImagePlane *Sprite)
{
    uint16_t i,j;

    for(j=0; j<Sprite->SizeY; j++)
    {
        for(i=0; i<Sprite->SizeX; i++)
        {
			eGFX_PutPixel(Destination, x + i, y + j, eGFX_GetPixel(Sprite, i, j));
        }

    }
}

//Blits a sprite.   x,y is the center of the image to place, not the upper left corner

void eGFX_CenteredBlit(eGFX_ImagePlane *Destination,
	int16_t x,
	int16_t y,
	const eGFX_ImagePlane *Sprite)
{
	uint16_t i, j;

	uint16_t X_Offset;
	uint16_t Y_Offset;

	X_Offset = (Sprite->SizeX + 1) >> 1;
	Y_Offset = (Sprite->SizeY + 1) >> 1;

	for (j = 0; j<Sprite->SizeY; j++)
	{
		for (i = 0; i<Sprite->SizeX; i++)
		{
			eGFX_PutPixel(Destination, x + i - X_Offset,
									   y + j - Y_Offset,
										eGFX_GetPixel(Sprite, i, j));
		}

	}
}

//x,y is a the upper left corner of the blit, not the center
void eGFX_BlitFaded(eGFX_ImagePlane *Destination,
               int16_t x,
               int16_t y,
               const eGFX_ImagePlane *Sprite,
               eGFX_PixelState PS)
{
    uint16_t i,j;

    for(j=0; j<Sprite->SizeY; j++)
    {
        for(i=0; i<Sprite->SizeX; i++)
        {
			eGFX_PutPixel(Destination, x + i, y + j, (eGFX_GetPixel(Sprite, i, j) * PS)>>8);
        }

    }
}

//x,y is a the upper left corner of the blit, not the center
void eGFX_BlitShaded(eGFX_ImagePlane *Destination,
	int16_t x,
	int16_t y,
	const eGFX_ImagePlane *Sprite,
	float Shading)
{
	uint16_t i, j;

	if (Shading < 0.0)
		Shading = 0;
	if (Shading >1.0)
		Shading = 1.0;

	for (j = 0; j<Sprite->SizeY; j++)
	{
		for (i = 0; i<Sprite->SizeX; i++)
		{
			eGFX_PutPixel(Destination, x + i, y + j,(eGFX_PixelState) (eGFX_GetPixel(Sprite, i, j) * Shading));
		}

	}
}

//x,y is a the upper left corner of the blit, not the center
void eGFX_BlitColored(eGFX_ImagePlane *Destination,
	int16_t x,
	int16_t y,
	const eGFX_ImagePlane *Sprite,
	uint32_t Color)
{
	uint16_t i, j;

	for (j = 0; j<Sprite->SizeY; j++)
	{
		for (i = 0; i<Sprite->SizeX; i++)
		{
            if(eGFX_GetPixel(Sprite, i, j))
                eGFX_PutPixel(Destination, x + i, y + j,Color);
		}

	}
}



//Draws a sprite that is scaled.   x,y is a center point, not a corner
void eGFX_ScaledBlit(eGFX_ImagePlane *Destination,
					int16_t x,
					int16_t y,
					const eGFX_ImagePlane *Sprite,
					float ScaleFactor)
{
    uint16_t i,j;
    uint16_t DestinationSizeX;
    uint16_t DestinationSizeY;
	float StepX;
	float StepY;
    float SourcePixelX;
    float SourcePixelY;
	int16_t CenterOffsetX;
	int16_t CenterOffsetY;


	if (ScaleFactor <= 0.0)
		return;
	
	DestinationSizeX = (uint16_t)((ScaleFactor * (float)Sprite->SizeX)+0.5);
    DestinationSizeY = (uint16_t)((ScaleFactor * (float)Sprite->SizeY)+0.5);

	CenterOffsetX = (uint16_t)(((ScaleFactor * (float)Sprite->SizeX)/2.0) + 0.5);
	CenterOffsetY = (uint16_t)(((ScaleFactor * (float)Sprite->SizeY)/2.0) + 0.5);

	StepX = (float)Sprite->SizeX / (float)DestinationSizeX;
	StepY = (float)Sprite->SizeY / (float)DestinationSizeY;

	SourcePixelY = 0;

    for(j=0; j<DestinationSizeY; j++)
    {
        SourcePixelX = 0;

        for(i=0; i<DestinationSizeX; i++)
        {
			eGFX_PutPixel(Destination, 
							x + i - CenterOffsetX,
							y + j - CenterOffsetY,
							eGFX_GetPixel(Sprite,
										 (int16_t)(SourcePixelX + 0.5),
										 (int16_t)(SourcePixelY + 0.5)
										 )
							);
         
			SourcePixelX += StepX;
        }

       
		SourcePixelY += StepY;
    }
}

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

uint8_t eGFX_GetBitFromArray(uint8_t *DataArray,uint16_t Bit)
{
    uint8_t Mask;
    Mask = 1 << (7-(Bit&0x7));

    if(DataArray[Bit>>3] & Mask)
        return 1;
    else
        return 0;
}

//This funntion grabs an 8-bit chunk from each row of an image plane.   Assumes we are byte aligned and is really only used for 1BPP and 4BPP
//Image types to assist a driver
//X is the starting pixel.   For 1BPP It will be byte aligned. I.e.  requesting X = 12 will get you the byte for X starting at 8.
//for 4BPP, requesting X = 13 will get you the byte for X starting for 12

uint8_t eGFX_ImagePlane_GetByte(eGFX_ImagePlane   *Image,
                                uint16_t X,
                                uint16_t Y)
{
    uint32_t Offset;
    uint32_t MemWidthInBytes;


    switch(Image->Type)
    {
    	case eGFX_IMAGE_PLANE_4BPP:

    		  if(X > Image->SizeX)   //Make sure we aren't asking for a chunk greater than the width of one row
    		    {
    	  		        X = (Image->SizeX - 1);
    		    }
    		  if(Y > Image->SizeY)
    		    {
    		  		  X = (Image->SizeY- - 1);
    		    }

    		    X &= ~((uint16_t)0x1); // Make sure we are on a byte boundary

    		    //Properly Implement Rounding for odd bit plane sizes
    		      MemWidthInBytes  = (Image->SizeX)>>1;

    		      if(Image->SizeX & 0x1)
    		  	        MemWidthInBytes++;

    		      Offset = (Y * MemWidthInBytes) + (X>>1);
    		      return Image->Data[Offset];


    		break;

    	case eGFX_IMAGE_PLANE_1BPP:

    		  if(X > Image->SizeX)   //Make sure we aren't asking for a chunk greater than the width of one row
    		    {
    		        X = (Image->SizeX - 1);
    		    }

    		  if(Y > Image->SizeY)
    		  {
    			  X = (Image->SizeY- - 1);
    		  }

    		    X &= ~((uint16_t)0x7); // Make sure we are on a byte boundary
    		    //Properly Implement Rounding for odd bit plane sizes
    		    MemWidthInBytes  = (Image->SizeX)>>3;

    		    if(Image->SizeX & 0x7)
    		        MemWidthInBytes++;

    		    Offset = (Y * MemWidthInBytes) + (X>>3);
    		    return Image->Data[Offset];

    		    break;

    	default:

    		// Other image planes not supported
    		return 0;

    		break;
    }
}

void eGFX_ImagePlaneInit(eGFX_ImagePlane   *Image,uint8_t * Store, int16_t SizeX,int16_t SizeY,uint8_t Type)
{
	Image->Data = Store;
	Image->SizeX = SizeX;
	Image->SizeY = SizeY;
	Image->Type = Type;

}

void eGFX_ImagePlane_Clear(eGFX_ImagePlane   *Image)
{
    uint32_t PlaneSpaceSize = 0;
    uint32_t i = 0;

    switch(Image->Type)
    {
        case  eGFX_IMAGE_PLANE_1BPP:
            PlaneSpaceSize = eGFX_CALCULATE_1BPP_IMAGE_STORAGE_SPACE_SIZE(Image->SizeX,Image->SizeY);

            for(i=0; i<PlaneSpaceSize; i++)
            {
                Image->Data[i] = 0;
            }

            break;

        case  eGFX_IMAGE_PLANE_4BPP:
            PlaneSpaceSize = eGFX_CALCULATE_4BPP_IMAGE_STORAGE_SPACE_SIZE(Image->SizeX,Image->SizeY);

            for(i=0; i<PlaneSpaceSize; i++)
            {
                Image->Data[i] = 0;
            }

            break;

        case  eGFX_IMAGE_PLANE_8BPP:
            PlaneSpaceSize = eGFX_CALCULATE_8BPP_IMAGE_STORAGE_SPACE_SIZE(Image->SizeX,Image->SizeY);

            for(i=0; i<PlaneSpaceSize; i++)
            {
                Image->Data[i] = 0;
            }

            break;

        case  eGFX_IMAGE_PLANE_16BPP_565:
            PlaneSpaceSize = eGFX_CALCULATE_16BPP_IMAGE_STORAGE_SPACE_SIZE(Image->SizeX,Image->SizeY);

            for(i=0; i<PlaneSpaceSize>>1; i++)
            {
                 ((uint16_t *)(Image->Data))[i] = 0;
            }

            break;
 
        case  eGFX_IMAGE_PLANE_24BPP:
            PlaneSpaceSize = eGFX_CALCULATE_24BPP_IMAGE_STORAGE_SPACE_SIZE(Image->SizeX,Image->SizeY);
                            
            for(i=0; i<PlaneSpaceSize; i++)
            {
                Image->Data[i] = 0;
            }
        
            break;
        
        case  eGFX_IMAGE_PLANE_32BPP:
         
            PlaneSpaceSize = eGFX_CALCULATE_32BPP_IMAGE_STORAGE_SPACE_SIZE(Image->SizeX,Image->SizeY);

            for(i=0; i<PlaneSpaceSize; i++)
            {
                Image->Data[i] = 0;
            }

            break;
			
        default:
            break;
    }
}

/***
 *      ______ _              _   _____      _       _      ____                       _   _
 *     |  ____(_)            | | |  __ \    (_)     | |    / __ \                     | | (_)
 *     | |__   ___  _____  __| | | |__) |__  _ _ __ | |_  | |  | |_ __   ___ _ __ __ _| |_ _  ___  _ __  ___
 *     |  __| | \ \/ / _ \/ _` | |  ___/ _ \| | '_ \| __| | |  | | '_ \ / _ \ '__/ _` | __| |/ _ \| '_ \/ __|
 *     | |    | |>  <  __/ (_| | | |  | (_) | | | | | |_  | |__| | |_) |  __/ | | (_| | |_| | (_) | | | \__ \
 *     |_|    |_/_/\_\___|\__,_| |_|   \___/|_|_| |_|\__|  \____/| .__/ \___|_|  \__,_|\__|_|\___/|_| |_|___/
 *                                                               | |
 *                                                               |_|
 */

const FIXED_1_14 eGFX_Fixed_1_14_SineTable[256] = { 0x0000, 0x0192, 0x0324, 0x04B5, 0x0646, 0x07D6, 0x0964, 0x0AF1,
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

const FIXED_7_8 eGFX_Fixed_7_8_SineTable[256] = { 0x0000, 0x0006, 0x000D, 0x0013, 0x0019, 0x001F, 0x0026, 0x002C,
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

FIXED_7_8 eGFX_GET_INT_FROM_FIXED_7_8_ROUND_TO_NEAREST(FIXED_7_8 A)
{
    FIXED_7_8 RetVal = 0;

    if(A&0x0080)
        RetVal = 1;

    RetVal += A>>8;
    return RetVal;
}

FIXED_7_8 eGFX_FLOAT_TO_FIXED_7_8(float A)
{
    if(A>(float)((FIXED_7_8)(32767)>>8))
		return 	(FIXED_7_8)((float)(32767) / (float)(256));
    else if(A<(float)((FIXED_7_8)(-32768)>>8))
		return (FIXED_7_8)((float)(-32768) / (float)256);
    else
        return (FIXED_7_8)(A * 256);
}

int16_t eGFX_FMul_15_0to0_15(int16_t A, int16_t B)
{
    return  (((long)(A) * (long)(B))>>15);
}

int16_t eGFX_FMul_7_8to0_15(int16_t A, int16_t B)
{
    return  (((long)(A) * (long)(B))>>15);
}

int16_t eGFX_FMul_15_0to7_8(int16_t A,int16_t B)
{
    return  (((long)(A) * (long)(B))>>8);
}

int16_t eGFX_FMul_15_0to1_14(int16_t A,int16_t B)
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

    return  (int16_t)temp;
}

int16_t eGFX_ABS(int16_t A)
{
    if(A<0)
        return -A;
    else
        return A;
}


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
 
//Statically Allocate the animator list
void *eGFX_Animators[eGFX_MAX_ANIMATORS];

static uint32_t eGFX_AnimatorEndIndex = 0;

//Adds an animator object to the list.  returns TRUE if successful.
uint32_t eGFX_AddAnimator(void * Animator)
{
	if(eGFX_AnimatorEndIndex < eGFX_MAX_ANIMATORS)
	{
		eGFX_Animators[eGFX_AnimatorEndIndex] = Animator;
		eGFX_AnimatorEndIndex++;
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}
void eGFX_ClearAnimators()
{
	eGFX_AnimatorEndIndex = 0;
}

uint32_t eGFX_GetAnimatorCount()
{
	return eGFX_AnimatorEndIndex;
}

uint32_t eGFX_AnimatorsComplete()
{
	uint32_t RetVal = TRUE;
	uint32_t i;
	
	for(i=0;i<eGFX_AnimatorEndIndex;i++)
	{
		if(((eGFX_ObjectHeader *)(eGFX_Animators[i]))->State == eGFX_OBJECT_STATE_ANIMATION_ACTIVE)
		{
			RetVal = FALSE;
		}
	}
	
	return RetVal;
}

void eGFX_PauseAnimators()
{

	uint32_t i;

	for (i = 0; i<eGFX_AnimatorEndIndex; i++)
	{
		if (eGFX_Animators[i]!=NULL)
			((eGFX_ObjectHeader *)(eGFX_Animators[i]))->State = eGFX_OBJECT_STATE_INACTIVE;
	}

}

void eGFX_StartAnimators()
{

	uint32_t i;

	for (i = 0; i<eGFX_AnimatorEndIndex; i++)
	{
		if (eGFX_Animators[i] != NULL)
			((eGFX_ObjectHeader *)(eGFX_Animators[i]))->State = eGFX_OBJECT_STATE_ANIMATION_ACTIVE;
	}
}



/***
*      ___     _     _   ___   ___ _            _
*     | _ \___(_)_ _| |_| __| | _ |_)___ ___ __| |_
*     |  _/ _ \ | ' \  _| _|  | _ \ (_-</ -_) _|  _|
*     |_| \___/_|_||_\__|_|   |___/_/__/\___\__|\__|
*
*/

//Initializes the data struct
uint32_t eGFX_InitPointF_BisectAnimator(eGFX_PointF_BisectAnimator *P,
								  eGFX_PointF Target,
								  eGFX_PointF *Current,
							      uint32_t LastStep,			//Maximum Allow Steps before forced convergence
								  float FractionalBisect) //The amount of percentage of the distance the point should move each step. Values <=0 or >= 1 are set to 0.5

{
	if (P != NULL)
	{
		P->Target = Target;
		P->Current = Current;
		P->LastStep = LastStep;
		P->FractionalBisect = FractionalBisect;

		if (P->FractionalBisect <= 0.0f)
			P->FractionalBisect = 0.5f;
		else if (P->FractionalBisect >= 1.0f)
			P->FractionalBisect = 0.5f;

		P->Type = eGFX_OBJECT_POINTF_BISECT_ANIMATOR;
		P->State = eGFX_OBJECT_STATE_ANIMATION_ACTIVE;
		P->CurrentStep = 0;

		return TRUE;
	}
	else
		return FALSE;
}

//Initializes and adds a point bisect animator to the list.
//returns true if successful, false if not
uint32_t eGFX_AddPointF_BisectAnimator(eGFX_PointF_BisectAnimator *P,
									eGFX_PointF Target,
									eGFX_PointF *Current,
									uint32_t LastStep,			//Maximum Allow Steps before forced convergence
									float FractionalBisect) //The amount of percentage of the distance the point should move each step.   0 - 255 maps to 0 to 99.6%.   Zero is invalid
{
	if (P != NULL)
	{
		eGFX_InitPointF_BisectAnimator(P, Target, Current, LastStep, FractionalBisect);

		return eGFX_AddAnimator(P);
	}
	else
		return FALSE;

}

void eGFX_ProcessPointF_BisectAnimator(eGFX_PointF_BisectAnimator *P)
{

	if (P->Current == NULL)
		return;

	if (P->State != eGFX_OBJECT_STATE_INACTIVE)
	{
		//Move a fraction of a distance towards the target
		P->Current->X += (P->Target.X - P->Current->X) * P->FractionalBisect;
		P->Current->Y += (P->Target.Y - P->Current->Y) * P->FractionalBisect;

		P->CurrentStep++;

		//See if we have converged or beyond the max number of steps
		if ((P->CurrentStep >= P->LastStep) ||
			((fabs((P->Target.Y) - (P->Current->Y)) <= 0.5) && (fabs((P->Target.X) - (P->Current->X)) <= 0.5)))
		{
			*(P->Current) = P->Target;

			P->State = eGFX_OBJECT_STATE_INACTIVE;
		}
	}
}

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
	float FractionalBisect) //The amount of percentage of the distance the point should move each step. Values <=0 or >= 1 are set to 0.5

{
	if (P != NULL)
	{
		P->Target = Target;
		P->Current = Current;
		P->LastStep = LastStep;
		P->FractionalBisect = FractionalBisect;

		if (P->FractionalBisect <= 0.0f)
			P->FractionalBisect = 0.5f;
		else if (P->FractionalBisect >= 1.0f)
			P->FractionalBisect = 0.5f;

		P->Type = eGFX_OBJECT_SCALARF_BISECT_ANIMATOR;
		P->State = eGFX_OBJECT_STATE_ANIMATION_ACTIVE;
		P->CurrentStep = 0;

		return TRUE;
	}
	else
		return FALSE;
}

//Initializes and adds a point bisect animator to the list.
//returns true if successful, false if not
uint32_t eGFX_AddScalarF_BisectAnimator(eGFX_ScalarF_BisectAnimator *P,
	float Target,
	float *Current,
	uint32_t LastStep,			//Maximum Allow Steps before forced convergence
	float FractionalBisect) //The amount of percentage of the distance the point should move each step.   0 - 255 maps to 0 to 99.6%.   Zero is invalid
{
	if (P != NULL)
	{
		eGFX_InitScalarF_BisectAnimator(P, Target, Current, LastStep, FractionalBisect);

		return eGFX_AddAnimator(P);
	}
	else
		return FALSE;

}

void eGFX_ProcessScalarF_BisectAnimator(eGFX_ScalarF_BisectAnimator *P)
{

	if (P->Current == NULL)
		return;

	if (P->State != eGFX_OBJECT_STATE_INACTIVE)
	{
		//Move a fraction of a distance towards the target
		*P->Current += (P->Target - (*P->Current)) * P->FractionalBisect;
		
		P->CurrentStep++;

		//See if we have converged or beyond the max number of steps
		if ((P->CurrentStep >= P->LastStep) ||
			((fabs((P->Target) - (*P->Current)) <= 0.5))
			)
		{
			*(P->Current) = P->Target;

			P->State = eGFX_OBJECT_STATE_INACTIVE;
		}
	}
}


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
										float Increment) 

{
	if (P != NULL)
	{
		P->Target = Target;
		P->Current = Current;
		P->LastStep = LastStep;
		P->Increment = Increment;

		P->Type = eGFX_OBJECT_SCALARF_LINEAR_ANIMATOR;
		P->State = eGFX_OBJECT_STATE_ANIMATION_ACTIVE;
		P->CurrentStep = 0;
		
		//Some sanity checking to see if an invalid increment was entered.  If so,  make the animator inactive
		if (P->Increment < 0)
		{
			if (Target > *Current)
			{

				*Current = Target;
				P->State = eGFX_OBJECT_STATE_INACTIVE;
			}
		}

		else if (P->Increment > 0)
		{
			if (Target < *Current)
			{
				*Current = Target;
				P->State = eGFX_OBJECT_STATE_INACTIVE;
			}
			
		}

		else
		{
			*Current = Target;
			P->State = eGFX_OBJECT_STATE_INACTIVE;
		}
		
		return TRUE;
	}
	else
		return FALSE;
}


//Initializes and adds a scalarf linear animator and adds it to the list
//returns true if successful, false if not
uint32_t eGFX_AddScalarF_LinearAnimator(eGFX_ScalarF_LinearAnimator *P,
	float Target,
	float *Current,
	uint32_t LastStep,			//Maximum Allow Steps before forced convergence
	float Increment) 
{
	if (P != NULL)
	{
		eGFX_InitScalarF_LinearAnimator(P, Target, Current, LastStep, Increment);

		return eGFX_AddAnimator(P);
	}
	else
		return FALSE;

}

void eGFX_ProcessScalarF_LinearAnimator(eGFX_ScalarF_LinearAnimator *P)
{

	if (P->Current == NULL)
		return;

	if (P->State != eGFX_OBJECT_STATE_INACTIVE)
	{
		//Move a fraction of a distance towards the target
		*P->Current += P->Increment;

		P->CurrentStep++;

		//See if we have converged or beyond the max number of steps
		if ((P->CurrentStep >= P->LastStep) ||
			((fabs((P->Target) - (*P->Current)) < P->Increment)))
		{
			*(P->Current) = P->Target;

			P->State = eGFX_OBJECT_STATE_INACTIVE;
		}
	}
}

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
	float IncrementY)

{
	if (P != NULL)
	{
		P->Target = Target;
		P->Current = Current;
		P->LastStep = LastStep;
		P->IncrementX = IncrementX;
		P->IncrementY = IncrementY;

		P->Type = eGFX_OBJECT_POINTF_LINEAR_ANIMATOR;
		P->State = eGFX_OBJECT_STATE_ANIMATION_ACTIVE;
		P->CurrentStep = 0;

		//Some sanity checking to see if an invalid increment was entered.  If so,  make the animator inactive
		if (P->IncrementX < 0)
		{
			if (P->Target.X > P->Current->X)
			{
				P->IncrementX = P->IncrementX * - 1.0f;
			}
		}

		else
		{
			if (P->Target.X < P->Current->X)
			{
				P->IncrementX = P->IncrementX * -1.0f;
			}
		}

		if (P->IncrementY < 0)
		{
			if (P->Target.Y > P->Current->Y)
			{
				P->IncrementY = P->IncrementY * -1.0f;
			}
		}

		else
		{
			if (P->Target.Y < P->Current->Y)
			{
				P->IncrementY = P->IncrementY * -1.0f;
			}
		}


		return TRUE;
	}
	else
		return FALSE;
}


//Initializes and adds a scalarf linear animator and adds it to the list
//returns true if successful, false if not
uint32_t eGFX_AddPointF_LinearAnimator(eGFX_PointF_LinearAnimator *P,
	eGFX_PointF Target,
	eGFX_PointF *Current,
	uint32_t LastStep,			//Maximum Allow Steps before forced convergence
	float IncrementX,
	float IncrementY)
{
	if (P != NULL)
	{
		eGFX_InitPointF_LinearAnimator(P, Target, Current, LastStep, IncrementX,IncrementY);

		return eGFX_AddAnimator(P);
	}
	else
		return FALSE;

}

void eGFX_ProcessPointF_LinearAnimator(eGFX_PointF_LinearAnimator *P)
{

	if (P->Current == NULL)
		return;

	if (P->State != eGFX_OBJECT_STATE_INACTIVE)
	{
		
		P->CurrentStep++;

		if (fabs((P->Target.X) - (P->Current->X)) >= fabs(P->IncrementX))
		{
			P->Current->X += P->IncrementX;
		}
		
		if (fabs((P->Target.Y) - (P->Current->Y)) >= fabs(P->IncrementY))
		{
			P->Current->Y += P->IncrementY;
		}

		//See if we have converged or beyond the max number of steps
		if ((P->CurrentStep >= P->LastStep) ||
												(
													(fabs((P->Target.X) - (P->Current->X)) < fabs(P->IncrementX)) && 
													(fabs((P->Target.Y) - (P->Current->Y)) < fabs(P->IncrementY))
												)
			)
			
		{
			*(P->Current) = P->Target;

			P->State = eGFX_OBJECT_STATE_INACTIVE;
		}
	}
}

/***
*      ___                           _        _            _
*     | _ \_ _ ___  __ ___ ______   /_\  _ _ (_)_ __  __ _| |_ ___ _ _ ___
*     |  _/ '_/ _ \/ _/ -_|_-<_-<  / _ \| ' \| | '  \/ _` |  _/ _ \ '_(_-<
*     |_| |_| \___/\__\___/__/__/ /_/ \_\_||_|_|_|_|_\__,_|\__\___/_| /__/
*
*/

void eGFX_ProcessAnimators()
{
	uint32_t i;

	for (i = 0; i<eGFX_AnimatorEndIndex; i++)
	{
		if (eGFX_Animators[i] != NULL)
		{
			switch (((eGFX_ObjectHeader *)(eGFX_Animators[i]))->Type)
			{
			case eGFX_OBJECT_POINTF_BISECT_ANIMATOR:
				eGFX_ProcessPointF_BisectAnimator(eGFX_Animators[i]);
				break;

			case eGFX_OBJECT_SCALARF_LINEAR_ANIMATOR:
				eGFX_ProcessScalarF_LinearAnimator(eGFX_Animators[i]);
				break;

			case eGFX_OBJECT_POINTF_LINEAR_ANIMATOR:
				
				eGFX_ProcessPointF_LinearAnimator(eGFX_Animators[i]);

					break;

			case eGFX_OBJECT_SCALARF_BISECT_ANIMATOR:

				eGFX_ProcessScalarF_BisectAnimator(eGFX_Animators[i]);

				break;

			default:
				break;
			}

		}
	}
}


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

 
//Statically Allocate the Object List
void *eGFX_Objects[eGFX_MAX_OBJECTS];

static uint32_t eGFX_ObjectEndIndex = 0;

//Adds an animator object to the list.  returns TRUE if successful.
uint32_t eGFX_AddObject(void * Object)
{
	if(eGFX_ObjectEndIndex < eGFX_MAX_OBJECTS)
	{
		eGFX_Objects[eGFX_ObjectEndIndex] = Object;
		eGFX_ObjectEndIndex++;
		
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}
void eGFX_ClearObjects()
{
	eGFX_ObjectEndIndex = 0;
}

uint32_t eGFX_GetObjectCount()
{
	return eGFX_ObjectEndIndex;
}
 
/***
*      _____        _   ___
*     |_   _|____ _| |_| __|
*       | |/ -_) \ /  _| _|
*       |_|\___/_\_\\__|_|
*
*/

void eGFX_InitObjectTextF(eGFX_Obj_TextF * TF,
	eGFX_PointF Position,
	eGFX_Font *Font,
	char * String
	)

{
	TF->Type = eGFX_OBJECT_TEXTF;
	TF->State = eGFX_OBJECT_STATE_ACTIVE;

	if (String!=NULL)
		TF->String = String;
	
	TF->Position = Position;
	
	if (Font!=NULL)
		TF->Font = Font;
}


void eGFX_AddObjectTextF(eGFX_Obj_TextF * TF,
	eGFX_PointF Position,
	eGFX_Font *Font,
	char * String
	)

{
	eGFX_InitObjectTextF(TF,
		Position,
		Font,
		String
		);

	eGFX_AddObject(TF);
}


void	eGFX_DrawObject_TextF(eGFX_ImagePlane * Image, eGFX_Obj_TextF *TextF_Object)
{
	eGFX_DrawString(Image,
				TextF_Object->String,
				ROUND_TO_INT16_T(TextF_Object->Position.X),
				ROUND_TO_INT16_T(TextF_Object->Position.Y),
				TextF_Object->Font);
}

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
	)

{
	TF->Type = eGFX_OBJECT_SHADED_TEXTF;
	TF->State = eGFX_OBJECT_STATE_ACTIVE;

	if (String != NULL)
		TF->String = String;

	TF->Position = Position;


	if (Shading > 1.0f)
		Shading = 1.0f;
	if (Shading < 0.0f)
		Shading = 0.0f;

	TF->Shading = Shading;

	if (Font != NULL)
		TF->Font = Font;
}


void eGFX_AddObjectShadedTextF(eGFX_Obj_ShadedTextF * TF,
	eGFX_PointF Position,
	eGFX_Font *Font,
	char * String,
	float Shading
	)

{
	eGFX_InitObjectShadedTextF(TF,
		Position,
		Font,
		String,
		Shading
		);

	eGFX_AddObject(TF);
}


void	eGFX_DrawObject_ShadedTextF(eGFX_ImagePlane * Image, eGFX_Obj_ShadedTextF *ShadedTextF_Object)
{
	eGFX_DrawStringShaded(Image,
		ShadedTextF_Object->String,
		ROUND_TO_INT16_T(ShadedTextF_Object->Position.X),
		ROUND_TO_INT16_T(ShadedTextF_Object->Position.Y),
		ShadedTextF_Object->Font,
		ShadedTextF_Object->Shading);
}

/***
*      _    _          ___
*     | |  (_)_ _  ___| __|
*     | |__| | ' \/ -_) _|
*     |____|_|_||_\___|_|
*
*/

void eGFX_InitObjectLineF(eGFX_Obj_LineF * LF,
	eGFX_PointF Start,
	eGFX_PointF End,
	eGFX_PixelState PS
	)

{
	LF->Type = eGFX_OBJECT_LINEF;
	
	LF->State = eGFX_OBJECT_STATE_ACTIVE;

	LF->Start = Start;

	LF->End = End;

	LF->PixelState = PS;

	
}


void eGFX_AddObjectLineF(eGFX_Obj_LineF * LF,
	eGFX_PointF Start,
	eGFX_PointF End,
	eGFX_PixelState PS
	)

{
	eGFX_InitObjectLineF(LF,
		Start,
		End,
		PS
		);

	eGFX_AddObject(LF);
}


void	eGFX_DrawObject_LineF(eGFX_ImagePlane * Image, eGFX_Obj_LineF *LineF_Object)
{
	eGFX_DrawLine(Image, 
		ROUND_TO_INT16_T(LineF_Object->Start.X),
		ROUND_TO_INT16_T(LineF_Object->Start.Y),
		ROUND_TO_INT16_T(LineF_Object->End.X),
		ROUND_TO_INT16_T(LineF_Object->End.Y),
		LineF_Object->PixelState);

}

/***
*      ___          _ _       ___
*     / __|_ __ _ _(_) |_ ___| __|
*     \__ \ '_ \ '_| |  _/ -_) _|
*     |___/ .__/_| |_|\__\___|_|
*         |_|
*/

void eGFX_InitObjectSpriteF(eGFX_Obj_SpriteF * S,
							eGFX_PointF Position,
							eGFX_ImagePlane *	Sprite
	)

{
	S->Type = eGFX_OBJECT_SPRITEF;
	
	S->State = eGFX_OBJECT_STATE_ACTIVE;

	S->Position = Position;

	S->Sprite = Sprite;
	
}


void eGFX_AddObjectSpriteF(eGFX_Obj_SpriteF * S,
							eGFX_PointF Position,
							eGFX_ImagePlane *	Sprite
							)

{
	eGFX_InitObjectSpriteF(S,
		Position,
		Sprite
		);

	eGFX_AddObject(S);
}


void	eGFX_DrawObject_SpriteF(eGFX_ImagePlane * Image, eGFX_Obj_SpriteF *SpriteF_Object)
{
	eGFX_CenteredBlit(Image,
		ROUND_TO_INT16_T(SpriteF_Object->Position.X),
		ROUND_TO_INT16_T(SpriteF_Object->Position.Y),
		SpriteF_Object->Sprite);

}


/***
*      ___          _ _       ___   ___          _        _
*     / __|_ __ _ _(_) |_ ___| __| / __| __ __ _| |___ __| |
*     \__ \ '_ \ '_| |  _/ -_) _|  \__ \/ _/ _` | / -_) _` |
*     |___/ .__/_| |_|\__\___|_|   |___/\__\__,_|_\___\__,_|
*         |_|
*/

void eGFX_InitObjectScaledSpriteF(eGFX_Obj_ScaledSpriteF * S,
	eGFX_PointF Position,
	eGFX_ImagePlane *	Sprite,
	float Scake
	)

{
	S->Type = eGFX_OBJECT_SCALED_SPRITEF;

	S->State = eGFX_OBJECT_STATE_ACTIVE;

	S->Position = Position;

	S->Sprite = Sprite;

}


void eGFX_AddObjectScaledSpriteF(eGFX_Obj_ScaledSpriteF * S,
	eGFX_PointF Position,
	eGFX_ImagePlane *	Sprite,
	float Scale
	)

{
	eGFX_InitObjectScaledSpriteF(S,
		Position,
		Sprite,
		Scale
		);

	eGFX_AddObject(S);
}


void	eGFX_DrawObject_ScaledSpriteF(eGFX_ImagePlane * Image, eGFX_Obj_ScaledSpriteF *SpriteF_Object)
{
		eGFX_ScaledBlit(Image, 
			ROUND_TO_INT16_T(SpriteF_Object->Position.X),
			ROUND_TO_INT16_T(SpriteF_Object->Position.Y),
			SpriteF_Object->Sprite, 
			SpriteF_Object->Scale);
}

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
							)
{
	CF->Type = eGFX_OBJECT_CIRCLEF;

	CF->State = eGFX_OBJECT_STATE_ACTIVE;

	CF->Position = Position;

	CF->Radius = Radius;

	CF->RenderOption = CircleF_RenderOption;

	CF->PixelState = PS;
}


void eGFX_AddObjectCircleF( eGFX_Obj_CircleF * CF,
							eGFX_PointF Position,
							float Radius,
							eGFX_PixelState PS,
							uint8_t CircleF_RenderOption
							)

{
	eGFX_InitObjectCircleF(CF,
		Position,
		Radius,
		PS,
		CircleF_RenderOption
		);

	eGFX_AddObject(CF);
}


void	eGFX_DrawObject_CircleF(eGFX_ImagePlane * Image, eGFX_Obj_CircleF *CircleF_Object)
{
	if (CircleF_Object->RenderOption == eGFX_CIRCLEF_RENDER_OPTION_OUTLINE)
	{
		eGFX_DrawCircle(Image, ROUND_TO_INT16_T(CircleF_Object->Position.X), ROUND_TO_INT16_T(CircleF_Object->Position.Y), ROUND_TO_INT16_T(CircleF_Object->Radius), CircleF_Object->PixelState);
	}
	else
	{
		eGFX_DrawFilledCircle(Image, ROUND_TO_INT16_T(CircleF_Object->Position.X), ROUND_TO_INT16_T(CircleF_Object->Position.Y), ROUND_TO_INT16_T(CircleF_Object->Radius), CircleF_Object->PixelState);
		
	}

}



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
	)

{
	BF->Type = eGFX_OBJECT_BOXF;

	BF->State = eGFX_OBJECT_STATE_ACTIVE;

	BF->P1 = P1;

	BF->P2 = P2;

	BF->BoxF_RenderOption = BoxF_RenderOption;

	BF->PixelState = PS;

}


void eGFX_AddObjectBoxF(eGFX_Obj_BoxF * BF,
	eGFX_PointF P1,
	eGFX_PointF P2,
	eGFX_PixelState PS,
	uint8_t BoxF_RenderOption
	)

{
	eGFX_InitObjectBoxF(BF,
		P1,
		P2,
		PS,
		BoxF_RenderOption
		);

	eGFX_AddObject(BF);
}


void	eGFX_DrawObject_BoxF(eGFX_ImagePlane * Image, eGFX_Obj_BoxF *BoxF_Object)
{
	int16_t i;
	int16_t Start, End;


	if (BoxF_Object->BoxF_RenderOption == eGFX_BOXF_RENDER_OPTION_OUTLINE)
	{
		eGFX_DrawHline(Image, ROUND_TO_INT16_T(BoxF_Object->P1.X), ROUND_TO_INT16_T(BoxF_Object->P2.X), ROUND_TO_INT16_T(BoxF_Object->P1.Y), BoxF_Object->PixelState);
		eGFX_DrawHline(Image, ROUND_TO_INT16_T(BoxF_Object->P1.X), ROUND_TO_INT16_T(BoxF_Object->P2.X), ROUND_TO_INT16_T(BoxF_Object->P2.Y), BoxF_Object->PixelState);
		eGFX_DrawVline(Image, ROUND_TO_INT16_T(BoxF_Object->P1.Y), ROUND_TO_INT16_T(BoxF_Object->P2.Y), ROUND_TO_INT16_T(BoxF_Object->P1.X), BoxF_Object->PixelState);
		eGFX_DrawVline(Image, ROUND_TO_INT16_T(BoxF_Object->P1.Y), ROUND_TO_INT16_T(BoxF_Object->P2.Y), ROUND_TO_INT16_T(BoxF_Object->P2.X), BoxF_Object->PixelState);
	}
	else
	{
		if ((ROUND_TO_INT16_T(BoxF_Object->P1.Y) > ROUND_TO_INT16_T(BoxF_Object->P2.Y)))
		{
			End = ROUND_TO_INT16_T(BoxF_Object->P1.Y);
			Start = ROUND_TO_INT16_T(BoxF_Object->P2.Y);
		}
		else 
		{
			End = ROUND_TO_INT16_T(BoxF_Object->P2.Y);
			Start = ROUND_TO_INT16_T(BoxF_Object->P1.Y);
		}
		for (i = Start; i < End + 1; i++)
		{
			eGFX_DrawHline(Image, ROUND_TO_INT16_T(BoxF_Object->P1.X), ROUND_TO_INT16_T(BoxF_Object->P2.X), i, BoxF_Object->PixelState);
		}
	}

}

/***
*      ___                     ___  _     _        _
*     |   \ _ _ __ ___ __ __  / _ \| |__ (_)___ __| |_ ___
*     | |) | '_/ _` \ V  V / | (_) | '_ \| / -_) _|  _(_-<
*     |___/|_| \__,_|\_/\_/   \___/|_.__// \___\__|\__/__/
*                                      |__/
*/

void eGFX_DrawObjects(eGFX_ImagePlane *IP)
{
	uint32_t i;

	for (i = 0; i<eGFX_ObjectEndIndex; i++)
	{
		if (eGFX_Objects[i] != NULL)
		{
			if (((eGFX_ObjectHeader *)(eGFX_Objects[i]))->State != eGFX_OBJECT_STATE_INACTIVE)
			{
			
			//object handlers here!
			switch (((eGFX_ObjectHeader *)(eGFX_Objects[i]))->Type)
				{


				case eGFX_OBJECT_TEXTF:
					eGFX_DrawObject_TextF(IP, (eGFX_Obj_TextF *)eGFX_Objects[i]);
					break;

				case eGFX_OBJECT_SHADED_TEXTF:
					eGFX_DrawObject_ShadedTextF(IP, (eGFX_Obj_ShadedTextF *)eGFX_Objects[i]);
					break;

				case eGFX_OBJECT_LINEF:
					eGFX_DrawObject_LineF(IP, (eGFX_Obj_LineF *)eGFX_Objects[i]);
					break;
				case eGFX_OBJECT_SPRITEF:
					eGFX_DrawObject_SpriteF(IP, (eGFX_Obj_SpriteF *)eGFX_Objects[i]);
					break;
				case eGFX_OBJECT_SCALED_SPRITEF:
					eGFX_DrawObject_ScaledSpriteF(IP, (eGFX_Obj_ScaledSpriteF *)eGFX_Objects[i]);
					break;

				case eGFX_OBJECT_BOXF:
					eGFX_DrawObject_BoxF(IP, (eGFX_Obj_BoxF *)eGFX_Objects[i]);
					break;

				case eGFX_OBJECT_CIRCLEF:
					eGFX_DrawObject_CircleF(IP, (eGFX_Obj_CircleF *)eGFX_Objects[i]);
					break;

				default:

					break;
				}
			}

		}


	}
}


