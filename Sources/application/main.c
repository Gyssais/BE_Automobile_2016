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
#include "Buttons_management.h"
#include "current_monitoring.h"
#include "driver_example.h"

/*************** Private function prototype **********/

/*************** Public function            **********/


 int main(void) {

	disableWatchdog();
	initModesAndClock();


	//adc_eoc_example();
	
	//ctu_trigger_example();
	
	//result = BUFFER_SIZE;
	//result = cm_initialize();
	
	//gpio_isr_example();
	h_bridge_test();
	while(1)
	{
		
	}
	

}
	 
				





 
	
	
	




