#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "eGFX_Driver.h"
#include "eGFX.h"
#include "MonkeyListenConfig.h"
#include "FRDM-K66F_AUDIO.h"

int main(void)

{
  /* Init board hardware. */
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();

  toggleScreen(0);
  InitAudio(AUDIO_SAMPLE_RATE);
  toggleScreen(1);

  eGFX_InitDriver();

  for(;;)
  {
		eGFX_ImagePlane_Clear(&eGFX_BackBuffer);
	    eGFX_DrawLine(&eGFX_BackBuffer,0,31,127,31,0xf);
		eGFX_Dump(&eGFX_BackBuffer);
 }

}



