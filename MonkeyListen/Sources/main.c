#include "derivative.h" 
#include "drivers/mcg/mcg.h"
#include "GFX/GFX.h"
#include "cpu/arm_cm4.h"
#include "drivers/adc16/adc16.h"
#include "arm_math.h"

/***
 *       _____             __ _       
 *      / ____|           / _(_)      
 *     | |     ___  _ __ | |_ _  __ _ 
 *     | |    / _ \| '_ \|  _| |/ _` |
 *     | |___| (_) | | | | | | | (_| |
 *      \_____\___/|_| |_|_| |_|\__, |
 *                               __/ |
 *                              |___/ 
 */

#define  MODE_SPECTROGRAM				0
#define  MODE_TIME_DOMAIN_PLUS_FFT		1

//Select the MonkeyListen Mode here!

//#define MONKEY_LISTEN_MODE	MODE_TIME_DOMAIN_PLUS_FFT
#define MONKEY_LISTEN_MODE	MODE_SPECTROGRAM


//Pick A Sample Rate.   8KHz works well for general viewing as most "voice" artifacts are quite low. (our FFT is 128 Points)
#define SAMPLE_RATE		8000


/***
 *                        _ _         _____ _                    _____                    ____         __  __          
 *         /\            | (_)       |  __ (_)                  |  __ \                  |  _ \       / _|/ _|         
 *        /  \  _   _  __| |_  ___   | |__) | _ __   __ _ ______| |__) |__  _ __   __ _  | |_) |_   _| |_| |_ ___ _ __ 
 *       / /\ \| | | |/ _` | |/ _ \  |  ___/ | '_ \ / _` |______|  ___/ _ \| '_ \ / _` | |  _ <| | | |  _|  _/ _ \ '__|
 *      / ____ \ |_| | (_| | | (_) | | |   | | | | | (_| |      | |  | (_) | | | | (_| | | |_) | |_| | | | ||  __/ |   
 *     /_/    \_\__,_|\__,_|_|\___/  |_|   |_|_| |_|\__, |      |_|   \___/|_| |_|\__, | |____/ \__,_|_| |_| \___|_|   
 *                                                   __/ |                         __/ |                               
 *                                                  |___/                         |___/                                
 */
volatile uint8_t CaptureActive = 0;
volatile uint8_t CurrentSample = 0;

volatile int16_t MicBuffer1[128];
volatile int16_t MicBuffer2[128];

volatile int16_t *Backbuffer;
volatile int16_t *Activebuffer;

volatile uint8_t BufferPtr = 0;


/***
 *      _____   _____ _____    ____         __  __              
 *     |  __ \ / ____|  __ \  |  _ \       / _|/ _|             
 *     | |  | | (___ | |__) | | |_) |_   _| |_| |_ ___ _ __ ___ 
 *     | |  | |\___ \|  ___/  |  _ <| | | |  _|  _/ _ \ '__/ __|
 *     | |__| |____) | |      | |_) | |_| | | | ||  __/ |  \__ \
 *     |_____/|_____/|_|      |____/ \__,_|_| |_| \___|_|  |___/
 *                                                              
 *                                                              
 */

volatile q15_t MicFFT[256];
volatile q15_t MicFFT_Mag[128];
const q15_t Hanning[128];
uint8_t DisplayBuf[64];

/***
 *       _____        _______ _      _                                 _ _         _____                        _ _             
 *      / ____|      |__   __(_)    | |                 /\            | (_)       |  __ \                      | (_)            
 *     | (___  _   _ ___| |   _  ___| | __  ______     /  \  _   _  __| |_  ___   | |__) |___  ___ ___  _ __ __| |_ _ __   __ _ 
 *      \___ \| | | / __| |  | |/ __| |/ / |______|   / /\ \| | | |/ _` | |/ _ \  |  _  // _ \/ __/ _ \| '__/ _` | | '_ \ / _` |
 *      ____) | |_| \__ \ |  | | (__|   <            / ____ \ |_| | (_| | | (_) | | | \ \  __/ (_| (_) | | | (_| | | | | | (_| |
 *     |_____/ \__, |___/_|  |_|\___|_|\_\          /_/    \_\__,_|\__,_|_|\___/  |_|  \_\___|\___\___/|_|  \__,_|_|_| |_|\__, |
 *              __/ |                                                                                                      __/ |
 *             |___/                                                                                                      |___/ 
 */

void InitSysTick()
{
    SYST_RVR = 50000000/SAMPLE_RATE;    // Set the Reload to the interrupt at our desired frequency
    SYST_CVR = 0;       // Reset the Current Value
    SYST_CSR = SysTick_CSR_ENABLE_MASK | SysTick_CSR_TICKINT_MASK | SysTick_CSR_CLKSOURCE_MASK;
    //Important!  Since the Systick is part of the Cortex core and NOT a kinetis peripheral
    // its Interrupt line is not passed through NVIC.   You need to make sure that
    //the SysTickIRQ function is populated in the vector table.  See the kinetis_sysinit.c file
}

//ADC Recording Process
void SysTick_Handler()
{
    if(CaptureActive)
        {
            //Store the Started Sample... We are assuming that the ADC willbe done by now
            Backbuffer[CurrentSample] = (uint16_t)ADC0_RA - (uint16_t)(2048);
            CurrentSample++;

            if(CurrentSample<128)
                {
                    //Start The Next Sample..  We will grab it on the next SysTickIrq
                    StartADC0_SingleEnded(ADC0_SE14);
                }
            else
                {
                    CurrentSample = 0;
                    CaptureActive = 0;
                }
        }
}


#define ENABLE_WINDOW_JUMPER	(GPIOD_PDIR & (1<<5))
#define ENABLE_FFT_JUMPER		(GPIOD_PDIR & (1<<6))

/***
 *      __  __             _                _      _     _               __  __       _       
 *     |  \/  |           | |              | |    (_)   | |             |  \/  |     (_)      
 *     | \  / | ___  _ __ | | _____ _   _  | |     _ ___| |_ ___ _ __   | \  / | __ _ _ _ __  
 *     | |\/| |/ _ \| '_ \| |/ / _ \ | | | | |    | / __| __/ _ \ '_ \  | |\/| |/ _` | | '_ \ 
 *     | |  | | (_) | | | |   <  __/ |_| | | |____| \__ \ ||  __/ | | | | |  | | (_| | | | | |
 *     |_|  |_|\___/|_| |_|_|\_\___|\__, | |______|_|___/\__\___|_| |_| |_|  |_|\__,_|_|_| |_|
 *                                   __/ |                                                    
 *                                  |___/                                                     
 */

int main(void)
{
	
	int32_t i = 0;
	
	#if  (MONKEY_LISTEN_MODE == MODE_TIME_DOMAIN_PLUS_FFT)
		int16_t Temp;
		int16_t LastY;
	#endif
    
    arm_rfft_instance_q15  		  RealFFT_Instance;
    arm_cfft_radix4_instance_q15 MyComplexFFT_Instance;
    
    //enable Clocks to All the Ports
  
    SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK |
                 SIM_SCGC5_PORTB_MASK |
                 SIM_SCGC5_PORTC_MASK |
                 SIM_SCGC5_PORTD_MASK |
                 SIM_SCGC5_PORTE_MASK;
    
    //Get up and running to 50MHz!
    pll_init(8000000, LOW_POWER, CRYSTAL, 4, 25, 1);
    
    //Setup inputs
    PORTD_PCR(5) = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK ;
    GPIOD_PDDR &= ~(1<<5);
    PORTD_PCR(6) = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK ;
    GPIOD_PDDR &= ~(1<<6);
   
    //Get the ADC running
    InitADC_12Bit();
    InitSysTick();
    EnableInterrupts;
    
    //Initialize the Graphics driver
    GFX_Init();
 
    //Initialize The Ping-pong Buffer.   The IRQ routine will fill up the back buffer while the FFT is done on the foreground buffer
    BufferPtr = 0;
    Activebuffer = MicBuffer1;
    Backbuffer	 = MicBuffer2;
    
    //Intilialize the FFT Structures
    arm_rfft_init_q15(&RealFFT_Instance,
                      &MyComplexFFT_Instance,
                      128,
                      0,
                      1); //Bit Reverse Flag enabled
  
    //Reset the Sample pointer
    CurrentSample = 0;
    
    //Start the Backbuffer Recording
    CaptureActive = 1;
    
    // In spectrogram mode we are skipping the graphics library and writing direct to the display in column mode
	#if (MONKEY_LISTEN_MODE == MODE_SPECTROGRAM)
    	SSD1325_SetRemap(0x50 | 1<<2); 
	#endif
 
    while(1)
        {
    		
    		//If we are here we are ready to process our back buffer
    		
    		//Apply a Hanning window if the jumper is set.
    		//If not, we just keep our rectanglular window
            if(!ENABLE_WINDOW_JUMPER)
                {
                    for(i=0; i<128; i++)
                        {
                            Activebuffer[i] = ((q31_t)Activebuffer[i]  * Hanning[i])>>15;
                        }
                }

            
#if  (MONKEY_LISTEN_MODE == MODE_TIME_DOMAIN_PLUS_FFT)

            /***
             *      _______ _                _____                        _                 ______ ______ _______ 
             *     |__   __(_)              |  __ \                      (_)          _    |  ____|  ____|__   __|
             *        | |   _ _ __ ___   ___| |  | | ___  _ __ ___   __ _ _ _ __    _| |_  | |__  | |__     | |   
             *        | |  | | '_ ` _ \ / _ \ |  | |/ _ \| '_ ` _ \ / _` | | '_ \  |_   _| |  __| |  __|    | |   
             *        | |  | | | | | | |  __/ |__| | (_) | | | | | | (_| | | | | |   |_|   | |    | |       | |   
             *        |_|  |_|_| |_| |_|\___|_____/ \___/|_| |_| |_|\__,_|_|_| |_|         |_|    |_|       |_|   
             *                                                                                                    
             *                                                                                                    
             */
            
            if(ENABLE_FFT_JUMPER)
                {
                    GFX_ImagePlane_Clear(&GFX_BackBuffer);
                    LastY = 32;

                    for(i=0; i<128; i++)
                        {
                    		//Because we have a 64 pixel screen,  we need to scale down our signal to fit.
                    	
                            Temp = Activebuffer[i]>>4;

                            if(i==0)
                                GFX_PutPixel(&GFX_BackBuffer,i,32+Temp,GFX_PIXEL_ON);
                            else
                                GFX_DrawLine(&GFX_BackBuffer,i-1,LastY,i,Temp+32,GFX_PIXEL_ON);

                            LastY = Temp + 32;
                        }

                    GFX_DumpImagePlaneToPhysicalScreen(&GFX_BackBuffer);
                }
            else
                {
                    GFX_ImagePlane_Clear(&GFX_BackBuffer);
                  
                    //Compute the FFT
                    arm_rfft_q15( &RealFFT_Instance, 
                    		       (q15_t *)Activebuffer,
                    		       (q15_t *)MicFFT);

                    //Scale the input before computing magnitude
                    for(i=0; i<256; i++)
                        {
                            MicFFT[i]<<=8;
                        }
                    
                    //FFT function returns the real / imaginary values.   We need to compute the magnitude
                    arm_cmplx_mag_q15((q15_t *)MicFFT,
                    				  (q15_t *)MicFFT_Mag,
											   128);
                   
                    //We are going to only draw the 1st 64 magnitude values as the last 64 are just the conjugates
                    
                    for(i=0; i<64; i++)
                                           {
                                               GFX_DrawVline(&GFX_BackBuffer,64,63-(MicFFT_Mag[i]>>8),(i*2),GFX_PIXEL_ON);
                                               GFX_DrawVline(&GFX_BackBuffer,64,63-(MicFFT_Mag[i]>>8),(i*2) + 1,GFX_PIXEL_ON);
                                           }
                    
                   GFX_DumpImagePlaneToPhysicalScreen(&GFX_BackBuffer);
                }

            
#elif    (MONKEY_LISTEN_MODE == MODE_SPECTROGRAM)
        
							/***
							 *       _____                 _                                       
							 *      / ____|               | |                                      
							 *     | (___  _ __   ___  ___| |_ _ __ ___   __ _ _ __ __ _ _ __ ___  
							 *      \___ \| '_ \ / _ \/ __| __| '__/ _ \ / _` | '__/ _` | '_ ` _ \ 
							 *      ____) | |_) |  __/ (__| |_| | | (_) | (_| | | | (_| | | | | | |
							 *     |_____/| .__/ \___|\___|\__|_|  \___/ \__, |_|  \__,_|_| |_| |_|
							 *            | |                             __/ |                    
							 *            |_|                            |___/                     
							 */
							
            				 //Compute the FFT
                              arm_rfft_q15(  &RealFFT_Instance,
                            		  	  	 (q15_t *)Activebuffer, 
                            		  	  	 (q15_t *)MicFFT);

                              //Scale the input before computing magnitude
                              for(i=0; i<256; i++)
                                  {
                                      MicFFT[i]<<=8;
                                  }
                              
                              //FFT function returns the real / imaginary values.   We need to compute the magnitude
                              arm_cmplx_mag_q15((q15_t *)MicFFT,
                            		  	  	    (q15_t *)MicFFT_Mag,
                            		  	  	    		 128);
                              
                              //We are just going to update the column all the way on the left
                              //The scrolling feature will move the image to the left by 2 ixels so we are going 
                              //map each point to be 2 pixels wide (we we effectively have a 64x64 pixel display)
                              
                              SSD1325_SetColumnStartEnd(63,63);
                        
                              	  for(i=0;i<64;i++)
                                  		{
                                  			DisplayBuf[63-i] = MicFFT_Mag[i]>>9;
                                  			
                                  			//Fill in the low pixel.  Clip to 0xF if the value is great;
                                  			if(DisplayBuf[63-i] > 0xF)
                                  				DisplayBuf[63-i] = 0xF;
                                  			
                                  			//Make the upper 4 bits the same as we mare making 2 pixels the same value
                                  			DisplayBuf[i] |=DisplayBuf[i]<<4;
                                  		                        			
                                  		}
                                  		
                                  		SSD_WriteColumn(&DisplayBuf[0]);
                                 
                                  		SSD1325_ScrollLeft();
                                  	
                                  		
                                  		//Delay a bit
                                  		for(i=0;i<40000;i++)
                                  		{
                                  		}
                             
#else
            
            
#endif
            
            
            //Wait for the Background capture to complete
            while(CaptureActive == 1)
                {
                }

            //Swap the Active/Background Buffer ---> Ping Pong!
            if( BufferPtr == 0)
                {
                    BufferPtr = 1;
                    Activebuffer = MicBuffer2;
                    Backbuffer = MicBuffer1;
                }
            else
                {
                    BufferPtr = 0;
                    Activebuffer = MicBuffer1;
                    Backbuffer = MicBuffer2;
                }

            //Start the background capture on the new buffer
            CaptureActive = 1;
            
        }

    return 0;
}

/***
 *      _    _                   _              __          ___           _                  _____            __  __ _      _            _       
 *     | |  | |                 (_)             \ \        / (_)         | |                / ____|          / _|/ _(_)    (_)          | |      
 *     | |__| | __ _ _ __  _ __  _ _ __   __ _   \ \  /\  / / _ _ __   __| | _____      __ | |     ___   ___| |_| |_ _  ___ _  ___ _ __ | |_ ___ 
 *     |  __  |/ _` | '_ \| '_ \| | '_ \ / _` |   \ \/  \/ / | | '_ \ / _` |/ _ \ \ /\ / / | |    / _ \ / _ \  _|  _| |/ __| |/ _ \ '_ \| __/ __|
 *     | |  | | (_| | | | | | | | | | | | (_| |    \  /\  /  | | | | | (_| | (_) \ V  V /  | |___| (_) |  __/ | | | | | (__| |  __/ | | | |_\__ \
 *     |_|  |_|\__,_|_| |_|_| |_|_|_| |_|\__, |     \/  \/   |_|_| |_|\__,_|\___/ \_/\_/    \_____\___/ \___|_| |_| |_|\___|_|\___|_| |_|\__|___/
 *                                        __/ |                                                                                                  
 *                                       |___/                                                                                                   
 */
const q15_t Hanning[128] =
{
    0	,
    20	,
    80	,
    180	,
    320	,
    499	,
    717	,
    973	,
    1267	,
    1597	,
    1965	,
    2367	,
    2804	,
    3273	,
    3775	,
    4308	,
    4871	,
    5461	,
    6078	,
    6721	,
    7387	,
    8075	,
    8784	,
    9511	,
    10255	,
    11014	,
    11786	,
    12569	,
    13362	,
    14162	,
    14967	,
    15776	,
    16587	,
    17397	,
    18204	,
    19007	,
    19804	,
    20592	,
    21370	,
    22136	,
    22887	,
    23623	,
    24341	,
    25039	,
    25717	,
    26371	,
    27001	,
    27606	,
    28182	,
    28730	,
    29247	,
    29734	,
    30187	,
    30607	,
    30991	,
    31341	,
    31653	,
    31928	,
    32165	,
    32364	,
    32523	,
    32643	,
    32723	,
    32763	,
    32763	,
    32723	,
    32643	,
    32523	,
    32364	,
    32165	,
    31928	,
    31653	,
    31341	,
    30991	,
    30607	,
    30187	,
    29734	,
    29247	,
    28730	,
    28182	,
    27606	,
    27001	,
    26371	,
    25717	,
    25039	,
    24341	,
    23623	,
    22887	,
    22136	,
    21370	,
    20592	,
    19804	,
    19007	,
    18204	,
    17397	,
    16587	,
    15776	,
    14967	,
    14162	,
    13362	,
    12569	,
    11786	,
    11014	,
    10255	,
    9511	,
    8784	,
    8075	,
    7387	,
    6721	,
    6078	,
    5461	,
    4871	,
    4308	,
    3775	,
    3273	,
    2804	,
    2367	,
    1965	,
    1597	,
    1267	,
    973	,
    717	,
    499	,
    320	,
    180	,
    80	,
    20	,
    0
};
