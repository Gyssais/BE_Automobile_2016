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
#include "Buttons_management.h"
#include "pin.h"

/*************** Private function prototype **********/

/*************** Public function            **********/

// configure the adc, configure the pin AD_PIN for analog input, and perform continuously a software triggered conversion.


 int main(void) {
	 disableWatchdog();
	 initModesAndClock();
	 
	 init_LED();
	 init_buttons();
	 
	 while(1) {
		 if(lock_door() == 1) {
			LED_on(1); 
		 }
		 else LED_off(1);
		 
		 if(det_rain() == 1){
			 LED_on(2);
		 }
		 else LED_off(2);
		 
		 if(bat_min() == 1) {   
			 LED_on(3);
		 }
		 else LED_off(3);
	 }
	
	
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
	 		
	 	 }*/

	
}
	 
				





 
	
	
	




