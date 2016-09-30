#include "stdlib.h"
#include "fsl_device_registers.h"
#include "eGFX_DataTypes.h"
#include "eGFX_Driver.h"
#include "eGFX.h"

#define FRDM_K66F

#ifdef FRDM_K22F

#define SSD1325_DC_GPIO		GPIOA
#define SSD1325_DC_PORT 	PORTA
#define SSD1325_DC_GPIO_BIT		2

#define SSD1325_RW_GPIO		GPIOA
#define SSD1325_RW_PORT 	PORTA
#define SSD1325_RW_GPIO_BIT	4

#define SSD1325_E_GPIO		GPIOB
#define SSD1325_E_PORT 		PORTB
#define SSD1325_E_GPIO_BIT	18


#define SSD1325_RESET_GPIO		GPIOE
#define SSD1325_RESET_PORT 		PORTE
#define SSD1325_RESET_GPIO_BIT	 0

#define SSD1325_CS_GPIO			GPIOE
#define SSD1325_CS_PORT 		PORTE
#define SSD1325_CS_GPIO_BIT			1

#define SSD1325_D0_GPIO			GPIOC
#define SSD1325_D0_PORT 		PORTC
#define SSD1325_D0_GPIO_BIT			3

#define SSD1325_D1_GPIO			GPIOC
#define SSD1325_D1_PORT 		PORTC
#define SSD1325_D1_GPIO_BIT			6

#define SSD1325_D2_GPIO			GPIOB
#define SSD1325_D2_PORT 		PORTB
#define SSD1325_D2_GPIO_BIT			19

#define SSD1325_D3_GPIO			GPIOA
#define SSD1325_D3_PORT 		PORTA
#define SSD1325_D3_GPIO_BIT			1

#define SSD1325_D4_GPIO			GPIOD
#define SSD1325_D4_PORT 		PORTD
#define SSD1325_D4_GPIO_BIT			4

#define SSD1325_D5_GPIO			GPIOD
#define SSD1325_D5_PORT 		PORTD
#define SSD1325_D5_GPIO_BIT			6

#define SSD1325_D6_GPIO			GPIOD
#define SSD1325_D6_PORT 		PORTD
#define SSD1325_D6_GPIO_BIT			7

#define SSD1325_D7_GPIO			GPIOD
#define SSD1325_D7_PORT 		PORTD
#define SSD1325_D7_GPIO_BIT			5

#endif

#ifdef FRDM_K66F

#define SSD1325_DC_GPIO		        GPIOC
#define SSD1325_DC_PORT 	        PORTC
#define SSD1325_DC_GPIO_BIT		    8

#define SSD1325_RW_GPIO			    GPIOC
#define SSD1325_RW_PORT 		    PORTC
#define SSD1325_RW_GPIO_BIT		    12

#define SSD1325_E_GPIO		        GPIOC
#define SSD1325_E_PORT 		        PORTC
#define SSD1325_E_GPIO_BIT	        5

#define SSD1325_RESET_GPIO		    GPIOC
#define SSD1325_RESET_PORT 		    PORTC
#define SSD1325_RESET_GPIO_BIT	    11

#define SSD1325_CS_GPIO			    GPIOC
#define SSD1325_CS_PORT 		    PORTC
#define SSD1325_CS_GPIO_BIT			10

#define SSD1325_D0_GPIO			GPIOC
#define SSD1325_D0_PORT 		PORTC
#define SSD1325_D0_GPIO_BIT			2

#define SSD1325_D1_GPIO			GPIOA
#define SSD1325_D1_PORT 		PORTA
#define SSD1325_D1_GPIO_BIT			25

#define SSD1325_D2_GPIO			GPIOB
#define SSD1325_D2_PORT 		PORTB
#define SSD1325_D2_GPIO_BIT			18

#define SSD1325_D3_GPIO			GPIOB
#define SSD1325_D3_PORT 		PORTB
#define SSD1325_D3_GPIO_BIT			10

#define SSD1325_D4_GPIO			GPIOD
#define SSD1325_D4_PORT 		PORTD
#define SSD1325_D4_GPIO_BIT			0

#define SSD1325_D5_GPIO			GPIOD
#define SSD1325_D5_PORT 		PORTD
#define SSD1325_D5_GPIO_BIT			2

#define SSD1325_D6_GPIO			GPIOD
#define SSD1325_D6_PORT 		PORTD
#define SSD1325_D6_GPIO_BIT			3

#define SSD1325_D7_GPIO			GPIOD
#define SSD1325_D7_PORT 		PORTD
#define SSD1325_D7_GPIO_BIT			1



#endif

#define SSD1325_SET_DC			SSD1325_DC_GPIO->PSOR = 1<<SSD1325_DC_GPIO_BIT;
#define SSD1325_CLEAR_DC		SSD1325_DC_GPIO->PCOR = 1<<SSD1325_DC_GPIO_BIT;

#define SSD1325_SET_RW			SSD1325_RW_GPIO->PSOR = 1<<SSD1325_RW_GPIO_BIT;
#define SSD1325_CLEAR_RW		SSD1325_RW_GPIO->PCOR = 1<<SSD1325_RW_GPIO_BIT;

#define SSD1325_SET_E			SSD1325_E_GPIO->PSOR = 1<<SSD1325_E_GPIO_BIT;
#define SSD1325_CLEAR_E			SSD1325_E_GPIO->PCOR = 1<<SSD1325_E_GPIO_BIT;

#define SSD1325_SET_RESET		SSD1325_RESET_GPIO->PSOR = 1<<SSD1325_RESET_GPIO_BIT;
#define SSD1325_CLEAR_RESET		SSD1325_RESET_GPIO->PCOR = 1<<SSD1325_RESET_GPIO_BIT;

#define SSD1325_SET_CS			SSD1325_CS_GPIO->PSOR = 1<<SSD1325_CS_GPIO_BIT;
#define SSD1325_CLEAR_CS		SSD1325_CS_GPIO->PCOR = 1<<SSD1325_CS_GPIO_BIT;



//This function set the data pins D0-D7 as Inputs
void SetSSD1325_DataBusAsInputs()
{
	SSD1325_D0_GPIO->PDDR &= ~(1<<SSD1325_D0_GPIO_BIT);
	SSD1325_D1_GPIO->PDDR &= ~(1<<SSD1325_D1_GPIO_BIT);
	SSD1325_D2_GPIO->PDDR &= ~(1<<SSD1325_D2_GPIO_BIT);
	SSD1325_D3_GPIO->PDDR &= ~(1<<SSD1325_D3_GPIO_BIT);
	SSD1325_D4_GPIO->PDDR &= ~(1<<SSD1325_D4_GPIO_BIT);
	SSD1325_D5_GPIO->PDDR &= ~(1<<SSD1325_D5_GPIO_BIT);
	SSD1325_D6_GPIO->PDDR &= ~(1<<SSD1325_D6_GPIO_BIT);
	SSD1325_D7_GPIO->PDDR &= ~(1<<SSD1325_D7_GPIO_BIT);
}

//This function set the data pins D0-D7 as outputs
void SetSSD1325_DataBusAsOutputs()
{
	SSD1325_D0_GPIO->PDDR |= (1<<SSD1325_D0_GPIO_BIT);
	SSD1325_D1_GPIO->PDDR |= (1<<SSD1325_D1_GPIO_BIT);
	SSD1325_D2_GPIO->PDDR |= (1<<SSD1325_D2_GPIO_BIT);
	SSD1325_D3_GPIO->PDDR |= (1<<SSD1325_D3_GPIO_BIT);
	SSD1325_D4_GPIO->PDDR |= (1<<SSD1325_D4_GPIO_BIT);
	SSD1325_D5_GPIO->PDDR |= (1<<SSD1325_D5_GPIO_BIT);
	SSD1325_D6_GPIO->PDDR |= (1<<SSD1325_D6_GPIO_BIT);
	SSD1325_D7_GPIO->PDDR |= (1<<SSD1325_D7_GPIO_BIT);
}


//Need for read/write strobe setup timing
//Just need a pinch to get a 1uS cycle timing
 void SSD1325_Delay()
{
    uint32_t t;

    for(t=0; t<10; t++)
        {
        }
}



void InitSSD1325_IO()
{

	//Enable Clocks to all Ports
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK |
	             SIM_SCGC5_PORTB_MASK |
	             SIM_SCGC5_PORTC_MASK |
	             SIM_SCGC5_PORTD_MASK |
	             SIM_SCGC5_PORTE_MASK;

	SSD1325_DC_GPIO->PDDR    |= (1<<SSD1325_DC_GPIO_BIT);
	SSD1325_RW_GPIO->PDDR    |= (1<<SSD1325_RW_GPIO_BIT);
	SSD1325_E_GPIO->PDDR     |= (1<<SSD1325_E_GPIO_BIT);
	SSD1325_RESET_GPIO->PDDR |= (1<<SSD1325_RESET_GPIO_BIT);
	SSD1325_CS_GPIO->PDDR    |= (1<<SSD1325_CS_GPIO_BIT);

	SSD1325_DC_PORT->PCR[SSD1325_DC_GPIO_BIT]       = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;

	SSD1325_RW_PORT->PCR[SSD1325_RW_GPIO_BIT]       = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	SSD1325_E_PORT->PCR[SSD1325_E_GPIO_BIT]         = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	SSD1325_RESET_PORT->PCR[SSD1325_RESET_GPIO_BIT] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	SSD1325_CS_PORT->PCR[SSD1325_CS_GPIO_BIT]       = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;

	SSD1325_D0_PORT->PCR[SSD1325_D0_GPIO_BIT] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	SSD1325_D1_PORT->PCR[SSD1325_D1_GPIO_BIT] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	SSD1325_D2_PORT->PCR[SSD1325_D2_GPIO_BIT] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	SSD1325_D3_PORT->PCR[SSD1325_D3_GPIO_BIT] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	SSD1325_D4_PORT->PCR[SSD1325_D4_GPIO_BIT] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	SSD1325_D5_PORT->PCR[SSD1325_D5_GPIO_BIT] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	SSD1325_D6_PORT->PCR[SSD1325_D6_GPIO_BIT] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	SSD1325_D7_PORT->PCR[SSD1325_D7_GPIO_BIT] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;

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
    	SSD1325_D0_GPIO->PSOR = 1<<SSD1325_D0_GPIO_BIT;
    else
    	SSD1325_D0_GPIO->PCOR = 1<<SSD1325_D0_GPIO_BIT;

    if(Data & 1<<1)
    	SSD1325_D1_GPIO->PSOR = 1<<SSD1325_D1_GPIO_BIT;
    else
    	SSD1325_D1_GPIO->PCOR = 1<<SSD1325_D1_GPIO_BIT;

    if(Data & 1<<2)
    	SSD1325_D2_GPIO->PSOR = 1<<SSD1325_D2_GPIO_BIT;
    else
    	SSD1325_D2_GPIO->PCOR = 1<<SSD1325_D2_GPIO_BIT;

    if(Data & 1<<3)
    	SSD1325_D3_GPIO->PSOR = 1<<SSD1325_D3_GPIO_BIT;
    else
    	SSD1325_D3_GPIO->PCOR = 1<<SSD1325_D3_GPIO_BIT;

    if(Data & 1<<4)
    	SSD1325_D4_GPIO->PSOR = 1<<SSD1325_D4_GPIO_BIT;
    else
    	SSD1325_D4_GPIO->PCOR = 1<<SSD1325_D4_GPIO_BIT;

    if(Data & 1<<5)
    	SSD1325_D5_GPIO->PSOR = 1<<SSD1325_D5_GPIO_BIT;
    else
    	SSD1325_D5_GPIO->PCOR = 1<<SSD1325_D5_GPIO_BIT;

    if(Data & 1<<6)
    	SSD1325_D6_GPIO->PSOR = 1<<SSD1325_D6_GPIO_BIT;
    else
    	SSD1325_D6_GPIO->PCOR = 1<<SSD1325_D6_GPIO_BIT;

    if(Data & 1<<7)
    	SSD1325_D7_GPIO->PSOR = 1<<SSD1325_D7_GPIO_BIT;
    else
    	SSD1325_D7_GPIO->PCOR = 1<<SSD1325_D7_GPIO_BIT;
}

//This function should return the data at D0-D7
//It is currently implemented in case the IO are not in a nice orderly arrangement
uint8_t GetSSD1325_DataBits()
{
    uint8_t Data = 0;
    SetSSD1325_DataBusAsInputs();

    if(SSD1325_D0_GPIO->PDIR & 1<<(SSD1325_D0_GPIO_BIT))
        Data |= 1<<0;

    if(SSD1325_D1_GPIO->PDIR & 1<<(SSD1325_D0_GPIO_BIT))
        Data |= 1<<1;

    if(SSD1325_D2_GPIO->PDIR & 1<<(SSD1325_D0_GPIO_BIT))
        Data |= 1<<2;

    if(SSD1325_D3_GPIO->PDIR & 1<<(SSD1325_D0_GPIO_BIT))
        Data |= 1<<3;

    if(SSD1325_D4_GPIO->PDIR & 1<<(SSD1325_D0_GPIO_BIT))
        Data |= 1<<4;

    if(SSD1325_D5_GPIO->PDIR & 1<<(SSD1325_D0_GPIO_BIT))
        Data |= 1<<5;

    if(SSD1325_D6_GPIO->PDIR & 1<<(SSD1325_D0_GPIO_BIT))
        Data |= 1<<6;

    if(SSD1325_D7_GPIO->PDIR & (1<<SSD1325_D0_GPIO_BIT))
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

eGFX_ImagePlane eGFX_BackBuffer;

uint8_t BackBufferStore[eGFX_CALCULATE_4BPP_IMAGE_STORAGE_SPACE_SIZE(eGFX_PHYSICAL_SCREEN_SIZE_X, eGFX_PHYSICAL_SCREEN_SIZE_Y)];

void eGFX_InitDriver()
{
     eGFX_ImagePlaneInit(&eGFX_BackBuffer,&BackBufferStore[0],eGFX_PHYSICAL_SCREEN_SIZE_X, eGFX_PHYSICAL_SCREEN_SIZE_Y,eGFX_IMAGE_PLANE_4BPP);

     InitSSD1325_IO();
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
     SSD1325_SetContrastCurrent(0xcc);		// Set Segment Output Current
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


void eGFX_Dump(eGFX_ImagePlane *Image)
{
    uint8_t x,y,Data;
    SSD1325_SET_DC;
    SSD1325_CLEAR_RW;
    SSD1325_CLEAR_CS;

    for(y=0; y<64; y++)
        {
            for(x=0; x<64; x++)
                {
                    Data = eGFX_GetPixel(Image,2*x,y);

                    Data += eGFX_GetPixel(Image,2*x+1,y)<<4;


                    SSD1325_Write(Data);
                }
        }

    SSD1325_SET_CS
}


//***********************************************************************************
//***********************************************************************************
//***********************************************************************************



