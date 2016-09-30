#include "board.h"
#include "fsl_i2c.h"
#include "fsl_dac.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "stdint.h"
#include "fsl_sai.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "MonkeyListenConfig.h"
#include "FRDM-K66F_AUDIO.h"

#define AUDIO_I2C_CLK_SRC 		I2C1_CLK_SRC
#define I2C_BAUDRATE 				100000U

i2c_master_handle_t g_m_handle;
sai_handle_t g_saiTxHandle;
sai_handle_t g_saiRxHandle;
sai_config_t user_config;
sai_transfer_t sendXfer;

volatile bool txFinished;
volatile bool rxFinished;
volatile bool completionFlag = false;
volatile bool nakFlag = false;
volatile bool screenactive = false;
int32_t pingpongbuff1[256], pingpongbuff2[256];

static void i2c_master_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData)
{
    /* Signal transfer success when received success status. */
    if (status == kStatus_Success)
    {
        completionFlag = true;
    }
    /* Signal transfer success when received success status. */
    if (status == kStatus_I2C_Nak)
    {
        nakFlag = true;
    }
}

void SAI_UserCallback(sai_handle_t *handle, status_t status, void *userData)
{
    userData = userData;
    if (kStatus_SAI_TxIdle == status)
    {
        txFinished = true;
    }
    if(kStatus_SAI_RxIdle==status){
    	rxFinished=true;
    }
}

static bool I2C_WriteAudioReg(I2C_Type *base, uint8_t device_addr, uint8_t reg_addr, uint8_t value)
{

	toggleScreen(0);

    i2c_master_transfer_t masterXfer;
    memset(&masterXfer, 0, sizeof(masterXfer));

    masterXfer.slaveAddress = device_addr;
    masterXfer.direction = kI2C_Write;
    masterXfer.subaddress = reg_addr;
    masterXfer.subaddressSize = 1;
    masterXfer.data = &value;
    masterXfer.dataSize = 1;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    /*  direction=write : start+device_write;cmdbuff;xBuff; */
    /*  direction=receive : start+device_write;cmdbuff;repeatStart+device_read;xBuff; */

    I2C_MasterTransferNonBlocking(I2C1, &g_m_handle, &masterXfer);

    /*  wait for transfer completed. */
    while ((!nakFlag) && (!completionFlag))
    {
    }

    nakFlag = false;

    if (completionFlag == true)
    {
        completionFlag = false;
        return true;
    }
    else
    {
        return false;
    }
}

bool I2C_ReadAudioRegs(I2C_Type *base, uint8_t device_addr, uint8_t reg_addr, uint8_t *rxBuff, uint32_t rxSize)
{
	if(screenactive)
		toggleScreen(false);

    i2c_master_transfer_t masterXfer;
    memset(&masterXfer, 0, sizeof(masterXfer));
    masterXfer.slaveAddress = device_addr;
    masterXfer.direction = kI2C_Read;
    masterXfer.subaddress = reg_addr;
    masterXfer.subaddressSize = 1;
    masterXfer.data = rxBuff;
    masterXfer.dataSize = rxSize;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    /*  direction=write : start+device_write;cmdbuff;xBuff; */
    /*  direction=receive : start+device_write;cmdbuff;repeatStart+device_read;xBuff; */

    I2C_MasterTransferNonBlocking(I2C1, &g_m_handle, &masterXfer);

    /*  wait for transfer completed. */
    while ((!nakFlag) && (!completionFlag))
    {
    }

    nakFlag = false;

    if (completionFlag == true)
    {
        completionFlag = false;
        return true;
    }
    else
    {
        return false;
    }
}

void toggleScreen(uint8_t onoff)
{
	port_pin_config_t pinConfig = {0};
	if(onoff){//enable screen
		pinConfig.pullSelect = 0;
		pinConfig.driveStrength=1;
		pinConfig.openDrainEnable=kPORT_OpenDrainDisable;
		pinConfig.mux=kPORT_MuxAsGpio;
		screenactive=true;
	}else{//enable I2C1
		pinConfig.pullSelect = kPORT_PullUp;
		pinConfig.mux = kPORT_MuxAlt2;
		pinConfig.openDrainEnable = kPORT_OpenDrainEnable;
		screenactive=false;
	}

	PORT_SetPinConfig(PORTC, 11U, &pinConfig);
	PORT_SetPinConfig(PORTC, 10U, &pinConfig);
}


static void InitSAI(uint32_t SampleRate)
{ //samplerate measured in khz
	uint32_t div, mclk, num, denom;


	CLOCK_EnableClock(kCLOCK_Sai0);
	I2S0->RCSR &= ~(3<<30); //ensure receiver is disabled
	I2S0->RCSR |= (1<<28);  //enable receiver bit clock

	I2S0->TCR2 &= ~(3<<30); //set tx to asynchronous mode
	I2S0->RCR2 &= ~(3<<30); //set receiver to synchronous mode
	I2S0->RCR2 |= (1<<30);
	 //swap bit clock source
	I2S0->RCR2 &= ~(3<<28);
	I2S0->TCR2 &= ~(3<<28);
	I2S0->RCR2 |= (1<<29);
	I2S0->TCR2 |= (1<<29);

	I2S0->RCR2 |= (1<<25);

	I2S0->RCR2 &= ~(3<<26);
	I2S0->RCR2 |= (1<<26);	//select master clock 1
	I2S0->RCR2 |= (1<<24); //generate bit clock internally as master


	switch(SampleRate)
	{
		case 11025:
		case 22050:
		case 44100:
		case 88200:
			num=255;
			denom=2721;
			mclk=11285670;
		break;

		default:
		case 16000:
			num=63;
			denom=624;
			mclk=12287840;
			break;
	}

	div=	(uint32_t)(((float)mclk/((float)SampleRate*(float)64.0)) + 0.5);

	div /=2;
	div -=1;

	I2S0->MCR |= (1<<30);
	I2S0->MCR &= ~(3<<24);
	I2S0->MDR=(num<<12)+denom;
	I2S0->RCR2 &=~(255);
	I2S0->RCR2 |= div;


	I2S0->TCSR &= ~(3<<30); //disable tx
	I2S0->TCSR |= (1<<28);


	I2S0->TCR2 |=(1<<30);
	I2S0->TCR2 &= ~(3<<26);
	I2S0->TCR2 |= (1<<26);	//select master clock 1
	I2S0->TCR2 |= (1<<24); //generate bit clock internally as master
	//I2S0->TCR2 |= (1<<25); //invert bit polarity
	I2S0->TCR2 &=~(255);
	I2S0->TCR2 |= div;

	//Set frame config
	I2S0->RCR4 &= ~(3<<24); //disable fifo packing
	I2S0->TCR4 &= ~(3<<24);
	//set 2 words per frame
	I2S0->RCR4 &= ~(31<<16);
	I2S0->RCR4 |= (1<<16);

	I2S0->TCR4 &= ~(31<<16);
	I2S0->TCR4 |= (1<<16);

	//set sync width to 1 bits
	I2S0->RCR4 |= (31<<8);
	I2S0->TCR4 |= (31<<8);

	//MSB first
	I2S0->RCR4 |=(1<<4);
	I2S0->TCR4 |=(1<<4);

	I2S0->RCR4 |=(1<<3);
	I2S0->TCR4 |=(1<<3);

	I2S0->RCR4 |=3;
	I2S0->TCR4 |=3;

	//set word length 32bits
	//I2S0->RCR5 &= ~(31);
	//I2S0->TCR5 &= ~(31);

	I2S0->RCR5 = (31<<24) | (31<<16) | (31<<8);
	I2S0->TCR5 =  (31<<24) | (31<<16) | (31<<8);
	//first word triggers flag

    //enable fifo warning interrupt
    //I2S0->TCSR |=1<<9;
    //set watermark to 3?
    I2S0->TCR1 = 3;
    I2S0->TCR3 |=(1<<16);
    //enable receive channel
    I2S0->RCR3 |= (1<<16);
    I2S0->RCR1 = 4;
    //NVIC_EnableIRQ(I2S0_Rx_IRQn);
    I2S0->RCSR |= (1<<8);
    I2S0->RCSR |= (1<<12);
    I2S0->RCSR &= ~(31);
    I2S0->RCSR |= 0;
    NVIC_EnableIRQ(I2S0_Rx_IRQn);

	//enable tx rx
	I2S0->RCSR |= (1<<31);
	I2S0->TCSR |= (1<<31);

}

bool active=1, buffready=0;

/*
void dothingswithdata(int32_t *buff){
	int i;
	float temp, last;
	//eGFX_ImagePlane_Clear(&eGFX_BackBuffer);

	last=(((float)buff[1] / (float)(0x7fffff))*256.0f) + 32.0f;

	for(i=3;i<256;i+=2){

		temp=(((float)buff[i] / (float)(0x7fffff))*256.0f) + 32.0f;
		//eGFX_DrawLine(&eGFX_BackBuffer,(i/2)-1,(int16_t)last, i/2, temp, 0xF);
		last=temp;
	}
	//  eGFX_Dump(&eGFX_BackBuffer);
	  buffready=false;
}
*/

volatile uint8_t CaptureActive = 0;
volatile uint8_t CurrentSample = 0;

volatile int32_t MicBuffer1[AUDIO_CAPTURE_BUFFER_SIZE];
volatile int32_t MicBuffer2[AUDIO_CAPTURE_BUFFER_SIZE];

volatile int32_t *Backbuffer;
volatile int32_t *Activebuffer;

volatile uint8_t BufferPtr = 0;


void I2S0_Rx_IRQHandler()
{
	if(I2S0->RCSR & ~(1<<20))
	{
		I2S0->RCSR|=1<<20;

        //Store the Started Sample... We are assuming that the ADC willbe done by now
        Backbuffer[CurrentSample] = (int32_t)I2S0->RDR[0]>>8;

        I2S0->TDR[0] = I2S0->RDR[0];

        CurrentSample++;

        if(CurrentSample == AUDIO_CAPTURE_BUFFER_SIZE)

            {
                CurrentSample = 0;
                CaptureActive = 0;
            }
	}
}


void InitAudio(uint32_t SampleRate)
{
	uint32_t sourceClock = 0;
	int i;
    i2c_master_config_t masterConfig;
    /*
     * masterConfig.baudRate_Bps = 100000U;
     * masterConfig.enableHighDrive = false;
     * masterConfig.enableStopHold = false;
     * masterConfig.glitchFilterWidth = 0U;
     * masterConfig.enableMaster = true;
     */
    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = I2C_BAUDRATE;

    sourceClock = CLOCK_GetFreq(AUDIO_I2C_CLK_SRC);

    I2C_MasterInit(I2C1, &masterConfig, sourceClock);

    I2C_MasterTransferCreateHandle(I2C1, &g_m_handle, i2c_master_callback, NULL);


    //CONFIG INPUT
	I2C_WriteAudioReg(I2C1, 0x1A, 0x23, 0x88);//enable master bias
	I2C_WriteAudioReg(I2C1, 0x1A, 0x22, 0x05); //set sample rate to 16khz
	I2C_WriteAudioReg(I2C1, 0x1A, 0x27, 0x00); //disable internal pll
	for(i=0;i<100000;i++);//delay before ldo is enabled
	I2C_WriteAudioReg(I2C1, 0x1A, 0x90, 0x80); //enable ldo?
	I2C_WriteAudioReg(I2C1, 0x1A, 0x94, 0x02); //enable autoresync
    I2C_WriteAudioReg(I2C1, 0x1A, 0x29, 0xC8); //set up clocks
	I2C_WriteAudioReg(I2C1, 0x1A, 0x38, 0x88); //enable adc filters
	I2C_WriteAudioReg(I2C1, 0x1A, 0x62, 0xAA); //enable mic1 and mic2 bias
	I2C_WriteAudioReg(I2C1, 0x1A, 0x63, 0x88); //enable mic1 amp
	I2C_WriteAudioReg(I2C1, 0x1A, 0x65, 0xA8); //enable input mixer unmuted
	I2C_WriteAudioReg(I2C1, 0x1A, 0x39, 0x02); //6db gain on mic 1
	I2C_WriteAudioReg(I2C1, 0x1A, 0x34, 0x05); //gain on mixL
	I2C_WriteAudioReg(I2C1, 0x1A, 0x66, 0xA8);
	I2C_WriteAudioReg(I2C1, 0x1A, 0x32, 0x80); //route mic to mixer
	I2C_WriteAudioReg(I2C1, 0x1A, 0x67, 0xA0); //enable adc
	I2C_WriteAudioReg(I2C1, 0x1A, 0x68, 0xA0);


    InitSAI(SampleRate);

	//I2C_WriteAudioReg(I2C1, 0x1A, 0x2B, 0x10);
	//uint8_t buff=1;
	//wait for offset to clear
	//while(buff){
	//I2C_ReadAudioRegs(I2C1, 0x1A, 0x2B, &buff, 1);
	//}
	//I2C_WriteAudioReg(I2C1, 0x1A, 0x2B, 0x8B);

	/*CONFIG OUTPUT*/
/*
	//enable dac
	I2C_WriteAudioReg(I2C1, 0x1A, 0x69, 0x80);//turn dac-l on
	I2C_WriteAudioReg(I2C1, 0x1A, 0x6A, 0x80);//turn dac-r on
	I2C_WriteAudioReg(I2C1, 0x1A, 0x44, 0x88);//highpass filter enabled.
	//enable charge pump?
	I2C_WriteAudioReg(I2C1, 0x1A, 0x47, 0xC1);
	//Route DAC to output mixer
	I2C_WriteAudioReg(I2C1, 0x1A, 0x4B, 0x08);//left mixer
	I2C_WriteAudioReg(I2C1, 0x1A, 0x4C, 0x08); //right mixer
	//turn headphone stuff on
	I2C_WriteAudioReg(I2C1, 0x1A, 0x6B, 0x89);//left headphone
	I2C_WriteAudioReg(I2C1, 0x1A, 0x6C, 0x89);//right headphone
	//turn on mixers
	I2C_WriteAudioReg(I2C1, 0x1A, 0x6E, 0x98);//left mixer
	I2C_WriteAudioReg(I2C1, 0x1A, 0x6F, 0x98);//right mixer

*/

}
