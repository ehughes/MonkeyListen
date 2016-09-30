#include "GFX.h"
#include "Derivative.h"
#include "stdlib.h"



//************************************************************************
//Functions/Macros for porting to other CPUS...
//************************************************************************

#define SSD1325_SET_DC			GPIOD_PSOR = 1<<4;
#define SSD1325_CLEAR_DC		GPIOD_PCOR = 1<<4;

#define SSD1325_SET_RW			GPIOC_PSOR = 1<<8;
#define SSD1325_CLEAR_RW		GPIOC_PCOR = 1<<8;

#define SSD1325_SET_E			GPIOA_PSOR = 1<<1;
#define SSD1325_CLEAR_E			GPIOA_PCOR = 1<<1;

#define SSD1325_SET_RESET		GPIOB_PSOR = 1<<3;
#define SSD1325_CLEAR_RESET		GPIOB_PCOR = 1<<3;

#define SSD1325_SET_CS			GPIOB_PSOR = 1<<2;
#define SSD1325_CLEAR_CS		GPIOB_PCOR = 1<<2;




//This function set the data pins D0-D7 as Inputs
void SetSSD1325_DataBusAsInputs()
{
    GPIOC_PDDR &= ~(1<<3);	//D0	PTC3
    GPIOC_PDDR &= ~(1<<4);	//D1	PTC4
    GPIOA_PDDR &= ~(1<<12);	//D2	PTA12
    GPIOA_PDDR &= ~(1<<2);	//D3    PTA2
    GPIOC_PDDR &= ~(1<<2);	//D4	PTC2
    GPIOD_PDDR &= ~(1<<2);	//D5	PTD2
    GPIOD_PDDR &= ~(1<<3);	//D6	PTD3
    GPIOD_PDDR &= ~(1<<1);	//D7	PTD1
}

//This function set the data pins D0-D7 as outputs
void SetSSD1325_DataBusAsOutputs()
{
    GPIOC_PDDR |= 1<<3;	//D0	PTC3
    GPIOC_PDDR |= 1<<4;	//D1	PTC4
    GPIOA_PDDR |= 1<<12;	//D2	PTA12
    GPIOA_PDDR |= 1<<2;	//D3    PTA2
    GPIOC_PDDR |= 1<<2;	//D4	PTC2
    GPIOD_PDDR |= 1<<2;	//D5	PTD2
    GPIOD_PDDR |= 1<<3;	//D6	PTD3
    GPIOD_PDDR |= 1<<1;	//D7	PTD1
}


//Need for read/write strobe setup timing
//Just need a pinch to get a 1uS cycle timing
inline void SSD1325_Delay()
{
    uint16_t t;

    for(t=0; t<1; t++)
        {
        }
}


//This should any pin I/O setup
void InitSSD1325_Interface()
{
    PORTD_PCR4 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    PORTC_PCR8 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    PORTA_PCR1 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    PORTB_PCR3 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    PORTB_PCR2 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    PORTC_PCR3 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    PORTC_PCR4 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    PORTA_PCR12 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    PORTA_PCR2 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    PORTC_PCR2 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    PORTD_PCR2 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    PORTD_PCR3 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    PORTD_PCR1 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    GPIOD_PDDR |= 1<<4;	//DC Signal
    GPIOC_PDDR |= 1<<8; //RW Signal
    GPIOA_PDDR |= 1<<1;	//E Signal
    GPIOB_PDDR |= (1<<3) | (1<<2);	//Reset and CS signal
    SSD1325_SET_RESET;
    SSD1325_SET_CS;
    SSD1325_CLEAR_DC;
    SSD1325_CLEAR_RW;
    SSD1325_CLEAR_E;
    SetSSD1325_DataBusAsInputs();
}

//This function should put data on D0-D7
//This function is need in case the data pins are not in nice, orderly location ie. P0.0 to P0.7
//   Each bit in the data byte will be test and the GPIO will be set/Cleared
void PutSSD1325_DataBits(uint8_t Data)
{
    SetSSD1325_DataBusAsOutputs();

    if(Data & 1<<0)
        GPIOC_PSOR = 1<<3;	//D0	PTC3
    else
        GPIOC_PCOR = 1<<3;	//D0	PTC3

    if(Data & 1<<1)
        GPIOC_PSOR = 1<<4;	//D1	PTC4
    else
        GPIOC_PCOR = 1<<4;	//D1	PTC4

    if(Data & 1<<2)
        GPIOA_PSOR = 1<<12;	//D2	PTA12
    else
        GPIOA_PCOR = 1<<12;	//D2	PTA12

    if(Data & 1<<3)
        GPIOA_PSOR = 1<<2;	//D3    PTA2
    else
        GPIOA_PCOR = 1<<2;	//D3    PTA2

    if(Data & 1<<4)
        GPIOC_PSOR = 1<<2;	//D4	PTC2
    else
        GPIOC_PCOR = 1<<2;	//D4	PTC2

    if(Data & 1<<5)
        GPIOD_PSOR = 1<<2;	//D5	PTD2
    else
        GPIOD_PCOR = 1<<2;	//D5	PTD2

    if(Data & 1<<6)
        GPIOD_PSOR = 1<<3;	//D6	PTD3
    else
        GPIOD_PCOR = 1<<3;	//D6	PTD3

    if(Data & 1<<7)
        GPIOD_PSOR = 1<<1;	//D7	PTD1
    else
        GPIOD_PCOR = 1<<1;	//D7	PTD1
}

//This function should return the data at D0-D7
//It is currently implemented in case the IO are not in a nice orderly arrangement
uint8_t GetSSD1325_DataBits()
{
    uint8_t Data = 0;
    SetSSD1325_DataBusAsInputs();

    if(GPIOC_PDIR & 1<<3)	//D0	PTC3
        Data |= 1<<0;

    if(GPIOC_PDIR & 1<<4)	//D1	PTC4
        Data |= 1<<1;

    if(GPIOA_PDIR & 1<<12)	//D2	PTA12
        Data |= 1<<2;

    if(GPIOA_PDIR & 1<<2)	//D3    PTA2
        Data |= 1<<3;

    if(GPIOC_PDIR & 1<<2)	//D4	PTC2
        Data |= 1<<4;

    if(GPIOD_PDIR & 1<<2)	//D5	PTD2
        Data |= 1<<5;

    if(GPIOD_PDIR & 1<<3)	//D6	PTD3
        Data |= 1<<6;

    if(GPIOD_PDIR & 1<<1)	//D7	PTD1
        Data |= 1<<7;

    return Data;
}

void SSD1325_Write(uint8_t Data)
{
    SetSSD1325_DataBusAsOutputs();
    PutSSD1325_DataBits(Data);
    SSD1325_Delay();
    SSD1325_SET_E;
    SSD1325_Delay();
    SSD1325_CLEAR_E;
}

void SSD1325_WriteCommandBytes(uint8_t *Data, uint16_t Length)
{
    uint16_t i;
    SSD1325_CLEAR_DC;
    SSD1325_CLEAR_RW;
    SSD1325_CLEAR_CS;
    SSD1325_Delay();

    for(i=0; i<Length; i++)
        {
            SSD1325_Write(Data[i]);
            SSD1325_Delay();
        }

    SSD1325_Delay();
    SSD1325_SET_CS;
}

void SSD1325_WriteDataBytes(uint8_t *Data, uint16_t Length)
{
    uint16_t i;
    SSD1325_SET_DC;
    SSD1325_CLEAR_RW;
    SSD1325_CLEAR_CS;

    for(i=0; i<Length; i++)
        {
            SSD1325_Write(Data[i]);
        }

    SSD1325_SET_CS;
}


#define SSD1325_CMD_DISPLAY_MODE_NORMAL			0xA4
#define SSD1325_CMD_DISPLAY_MODE_ALL_ON			0xA5
#define SSD1325_CMD_DISPLAY_MODE_ALL_OFF		0xA6
#define SSD1325_CMD_DISPLAY_MODE_INVERSE		0xA7

#define SSD1325_CMD_DISPLAY_OFF					0xAE
#define SSD1325_CMD_DISPLAY_ON					0xAF

void SSD1325_DisplayModeNormal()
{
    uint8_t Cmd = SSD1325_CMD_DISPLAY_MODE_NORMAL;
    SSD1325_WriteCommandBytes(&Cmd, 1);
}

void SSD1325_DisplayModeAllOn()
{
    uint8_t Cmd = SSD1325_CMD_DISPLAY_MODE_ALL_ON;
    SSD1325_WriteCommandBytes(&Cmd, 1);
}

void SSD1325_DisplayModeAllOff()
{
    uint8_t Cmd = SSD1325_CMD_DISPLAY_MODE_ALL_OFF;
    SSD1325_WriteCommandBytes(&Cmd, 1);
}

void SSD1325_DisplayModeInverse()
{
    uint8_t Cmd = SSD1325_CMD_DISPLAY_MODE_INVERSE;
    SSD1325_WriteCommandBytes(&Cmd, 1);
}

void SSD1325_DisplayOn()
{
    uint8_t Cmd = SSD1325_CMD_DISPLAY_ON;
    SSD1325_WriteCommandBytes(&Cmd, 1);
}

void SSD1325_DisplayOff()
{
    uint8_t Cmd = SSD1325_CMD_DISPLAY_OFF;
    SSD1325_WriteCommandBytes(&Cmd, 1);
}

#define SSD1325_CMD_SET_COLUMN_ADDRESS		0x15
#define SSD1325_CMD_SET_ROW_ADDRESS			0x75

void SSD1325_SetColumnStartEnd(uint8_t Start, uint8_t End)
{
    uint8_t Cmd[3];
    Cmd[0] = SSD1325_CMD_SET_COLUMN_ADDRESS;
    Cmd[1] = Start;
    Cmd[2] = End;
    SSD1325_WriteCommandBytes(&Cmd[0], 3);
}

void SSD1325_SetRowStartEnd(uint8_t Start, uint8_t End)
{
    uint8_t Cmd[3];
    Cmd[0] = SSD1325_CMD_SET_ROW_ADDRESS;
    Cmd[1] = Start;
    Cmd[2] = End;
    SSD1325_WriteCommandBytes(&Cmd[0], 3);
}

#define SSD1325_CMD_SET_MUX_RATIO		0xA8

void SSD1325_SetMuxRatio(uint8_t Ratio)
{
    uint8_t Cmd[2];
    Cmd[0] = SSD1325_CMD_SET_MUX_RATIO;
    Cmd[1] = Ratio;
    SSD1325_WriteCommandBytes(&Cmd[0], 2);
}

#define SSD1325_CMD_SET_START_LINE		0xA1

void SSD1325_SetDisplayStartLine(uint8_t Line)
{
    uint8_t Cmd[2];
    Cmd[0] = SSD1325_CMD_SET_START_LINE;
    Cmd[1] = Line;
    SSD1325_WriteCommandBytes(&Cmd[0], 2);
}

#define SSD1325_CMD_SET_OFFSET		0xA2

void SSD1325_SetDisplayOffset(uint8_t Offset)
{
    uint8_t Cmd[2];
    Cmd[0] = SSD1325_CMD_SET_OFFSET;
    Cmd[1] = Offset;
    SSD1325_WriteCommandBytes(&Cmd[0], 2);
}

#define SSD1325_CMD_SET_REMAP	0xA0

void SSD1325_SetRemap(uint8_t Setting)
{
    uint8_t Cmd[2];
    Cmd[0] = SSD1325_CMD_SET_REMAP;
    Cmd[1] = Setting;
    SSD1325_WriteCommandBytes(&Cmd[0], 2);
}

#define SSD1325_CURRENT_RANGE_QUARTER	0x84
#define SSD1325_CURRENT_RANGE_HALF		0x85
#define SSD1325_CURRENT_RANGE_FULL		0x86

void SSD1325_SetCurrentRange(uint8_t C)
{
    uint8_t Cmd[1];
    Cmd[0] = C;
    SSD1325_WriteCommandBytes(&Cmd[0], 1);
}

#define	SSD1325_CMD_SET_FRAME_FREQUENCY		0xB2

void SSD1325_SetFrameFrequency(unsigned char d)
{
    uint8_t Cmd[2];
    Cmd[0] = SSD1325_CMD_SET_FRAME_FREQUENCY;
    Cmd[1] = d;
    SSD1325_WriteCommandBytes(&Cmd[0], 2);
}

#define	SSD1325_CMD_SET_MASTER_CONFIG	0xAD

void SSD1325_MasterConfig(unsigned char MC)
{
    uint8_t Cmd[2];
    Cmd[0] = SSD1325_CMD_SET_MASTER_CONFIG;
    Cmd[1] = MC | 0x02;
    SSD1325_WriteCommandBytes(&Cmd[0], 2);
}

#define	SSD1325_CMD_PHASE_LENGTH	0xB1

void SSD1325_SetPhaseLength(unsigned char PL)
{
    uint8_t Cmd[2];
    Cmd[0] = SSD1325_CMD_PHASE_LENGTH;
    Cmd[1] = PL;
    SSD1325_WriteCommandBytes(&Cmd[0], 2);
}


#define	SSD1325_CMD_PRECHARGE_VOLTAGE	0xBC

void SSD1325_SetPrechargeVoltage(unsigned char PV)
{
    uint8_t Cmd[2];
    Cmd[0] = SSD1325_CMD_PRECHARGE_VOLTAGE;
    Cmd[1] = PV;
    SSD1325_WriteCommandBytes(&Cmd[0], 2);
}

#define	SSD1325_CMD_PRECHARGE_COMPENSATION	0xB4

void SSD1325_SetPrechargeCompensation(unsigned char a, unsigned char b)
{
    uint8_t Length = 2;
    uint8_t Cmd[2];
    //   Default => 0x00 (No Compensation)
    Cmd[0] = SSD1325_CMD_PRECHARGE_COMPENSATION;
    Cmd[1] = b;
    SSD1325_WriteCommandBytes(&Cmd[0], Length);

    if(a == 0x20)
        {
            Cmd[0] = 0xB0;
            Cmd[1]= 0x08 | a;
            SSD1325_WriteCommandBytes(&Cmd[0], Length);
        }
}

#define	SSD1325_CMD_VCOMH	0xBE

void SSD1325_SetVCOMH(unsigned char d)
{
    uint8_t Cmd[2];
    Cmd[0] =  SSD1325_CMD_VCOMH;
    Cmd[1] = d;
    SSD1325_WriteCommandBytes(&Cmd[0], 2);
}

#define	SSD1325_CMD_VSL		0xBF

void SSD1325_SetVSL(unsigned char d)
{
    uint8_t Cmd[2];
    Cmd[0] = SSD1325_CMD_VSL;
    Cmd[1] = d | 0x02;
    SSD1325_WriteCommandBytes(&Cmd[0], 2);
}

#define	SSD1325_CMD_DISPLAY_CLOCK		0xB3

void SSD1325_SetDisplayClock(unsigned char d)
{
    uint8_t Cmd[2];
    Cmd[0] = SSD1325_CMD_DISPLAY_CLOCK;
    Cmd[1] = d;
    SSD1325_WriteCommandBytes(&Cmd[0], 2);
}


void SSD1325_Set_GrayScaleTable()
{
    uint8_t Cmd[9];
    Cmd[0] = 0xB8;			// Set Gray Scale Table
    Cmd[1] =0x01;			//   Gray Scale Level 1
    Cmd[2] =0x11;			//   Gray Scale Level 3 & 2
    Cmd[3] =0x22;			//   Gray Scale Level 5 & 4
    Cmd[4] =0x32;			//   Gray Scale Level 7 & 6
    Cmd[5] =0x43;			//   Gray Scale Level 9 & 8
    Cmd[6] =0x54;			//   Gray Scale Level 11 & 10
    Cmd[7] =0x65;			//   Gray Scale Level 13 & 12
    Cmd[8] =0x76;			//   Gray Scale Level 15 & 14
    SSD1325_WriteCommandBytes(&Cmd[0], 9);
}



void  SSD1325_SetContrastCurrent(unsigned char d)
{
    uint8_t Cmd[2];
    Cmd[0] = 0x81;			// Set Contrast Value
    Cmd[1] = d;			//   Default => 0x40
    SSD1325_WriteCommandBytes(&Cmd[0], 2);
}


void SSD1325_ScrollRight()
{
    uint8_t Cmd[7];
    Cmd[0] = 0x25;
    
    Cmd[1] =0;
    Cmd[2] = 0;
    Cmd[3] = 63;
    Cmd[4] = 63;
    Cmd[5] = 1;
    Cmd[6] = 0;
            
    SSD1325_WriteCommandBytes(&Cmd[0], 7);
}

void SSD1325_ScrollLeft()
{
    uint8_t Cmd[7];
    Cmd[0] = 0x25;
    
    Cmd[1] =1;
    Cmd[2] = 0;
    Cmd[3] = 63;
    Cmd[4] = 63;
    
    Cmd[5] = 0;
    Cmd[6] = 0;
            
    SSD1325_WriteCommandBytes(&Cmd[0], 7);
}

//Assumes we are column addressing mode.
void SSD_WriteColumn(uint8_t * Data)
{
    uint8_t y;
    
    SSD1325_SET_DC;
    SSD1325_CLEAR_RW;
    SSD1325_CLEAR_CS;

 for(y=0; y<64; y++)
    {
         SSD1325_Write(Data[y]);
    }

    SSD1325_SET_CS;
    
}



//***********************************************************************************
//Linking Functions to The GFX main library
//***********************************************************************************

void GFX_InitPhysicalScreen()
{
    InitSSD1325_Interface();
    SSD1325_SET_RESET;
    SSD1325_Delay();
    SSD1325_CLEAR_RESET;
    SSD1325_Delay();
    SSD1325_SET_RESET;
    SSD1325_Delay();
    //see http://www.newhavendisplay.com/app_notes/OLED_2_7_12864.txt
    SSD1325_DisplayOff();
    SSD1325_SetDisplayClock(0x91);  //Magic number from Newhaven!
    SSD1325_SetMuxRatio(0x3F);		//Magic number from Newhaven!
    SSD1325_SetDisplayOffset(0x4C);		//Magic number from Newhaven!
    SSD1325_SetDisplayStartLine(0);
    SSD1325_MasterConfig(0);			//Magic number from Newhaven!
    SSD1325_SetRemap(0x50);				//Magic number from Newhaven!
    SSD1325_SetCurrentRange(SSD1325_CURRENT_RANGE_FULL);
    SSD1325_Set_GrayScaleTable();
    SSD1325_SetContrastCurrent(0xF);		// Set Segment Output Current
    SSD1325_SetFrameFrequency(0x51);	//Magic number from Newhaven!
    SSD1325_SetPhaseLength(0x55);			//Magic number from Newhaven!
    SSD1325_SetPrechargeVoltage(0x10);			 //Magic number from Newhaven!
    SSD1325_SetPrechargeCompensation(0x20,0x2); //Magic number from Newhaven!
    SSD1325_SetVCOMH(0x1C);
    SSD1325_SetVSL(0xD);
    SSD1325_DisplayModeNormal();
    SSD1325_DisplayOn();
    SSD1325_SetColumnStartEnd(0,63);
    SSD1325_SetRowStartEnd(0,63);
}

void GFX_DumpImagePlaneToPhysicalScreen(GFX_ImagePlane *Image)
{
    uint8_t x,y,Data;
    SSD1325_SET_DC;
    SSD1325_CLEAR_RW;
    SSD1325_CLEAR_CS;

    for(y=0; y<64; y++)
        {
            for(x=0; x<64; x++)
                {
                    Data = 0;

                    if(GFX_GetPixel(Image,2*x,y))
                        Data = 0x0F;

                    if(GFX_GetPixel(Image,(2*x)+1,y))
                        Data += 0xF0;

                    SSD1325_Write(Data);
                }
        }

    SSD1325_SET_CS
}

void GFX_PowerUpScreen()
{
    SSD1325_DisplayOn();
}

void GFX_PowerDownScreen()
{
    SSD1325_DisplayOff();
}

//***********************************************************************************
//***********************************************************************************
//***********************************************************************************



