/************ Include *********************/
#include "MPC5604B.h"
#include "IntcInterrupts.h"
#include "config.h"
#include "SBC.h"
#include "Mode_manager.h"
#include "MPC5604B_LED.h"
#include "BCM_appli.h"
#include "DCM_appli.h"
#include "pin.h"
#include "adc.h"
#include "define.h"
#include "spi_drv.h"
#include "pit.h"

/*************** Private function prototype **********/

/*************** Public function            **********/

// configure the adc, configure the pin AD_PIN for analog input, and perform continuously a software triggered conversion.
void adc_example()
{
	int result;
	
	result = setupAdc();
	result =  setupPin(AD_PIN);
	enableADC();
	
	while(1)
	{
		result = analogRead(AD_PIN);
	}
}


void adc_wtch_isr()
{
	unsigned long flag = ADC.WTISR.R;
}


// configure PIT, CTU and ADC to trigger a conversion on AD_PIN every 100ms
void ctu_trigger_example()
{
	int result;
		
	result = setupAdc();
	result = setupPin(AD_PIN);
	
	setup_CTU_PIT(0); 	// link the PIT to the ADC channel 0 (PB4) through the CTU
	setupChannelPIT(3, 100);  // use PIT_3, the only to be linked to the CTU. period =100ms.
	startChannelPIT(3);
	
	enableADC();
	
}

void adc_watchdog_example()
{
	int result;
		
	result = setupAdc();
	result =  setupPin(AD_PIN);
	
	result = setupAnalogWatchdog(AD_PIN, 700, 100, 0); // high_treshold =700, low = 100, first watchdog (0)
	result = setupAnalogWatchdogISR(adc_wtch_isr);
	startAnalogWatchdog(0);
	
	enableADC();
	
	
	while(1)
		{
			result = analogRead(AD_PIN);
		}
}


 int main(void) {

	 
	 disableWatchdog();
	 initModesAndClock();
	 
	 /*
	 SIU.PCR[68].R = 0x0200;// LED EN Sortie
	 SIU.PCR[69].R = 0x0200;
	 SIU.PCR[70].R = 0x0200;
	 SIU.PCR[65].R = 0x0100; // S2 en entree
	 SIU.PCR[64].R = 0x0100;
	 SIU.PCR[66].R = 0x0100;
	 while(1) {
		 if(SIU.GPDI[65].B.PDI == 1) {
			SIU.GPDO[68].R = 1; 
		 }
		 else SIU.GPDO[68].R = 0;
		 
		 if(SIU.GPDI[66].B.PDI == 1){
			 SIU.GPDO[70].R= 1;}
		 else SIU.GPDO[70].R = 0;
		 
		 if(SIU.GPDI[64].B.PDI == 1)
			 
		 {   SIU.GPDO[69].R = 1;
		 }
			else SIU.GPDO[69].R = 0;
		
	 }   */

	// adc_example(); // To test
	 adc_watchdog_example();
}
	 
				





 
	
	
	




