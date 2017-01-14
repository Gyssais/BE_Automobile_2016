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


/*************** Private function prototype **********/

/*************** Public function            **********/



void init()
{
	disableWatchdog();
	initModesAndClock();
	initialise_SPI_DRIVER();
	SPI[1].init(SPI_BAUD_62500, SPI_DELAY_DEFAULT);
	Init_SBC_DBG();
	initCAN1();
	
	init_LED();
	LED_off(1);
	LED_off(2);
	LED_off(3);
	LED_off(4);
	init_buttons();
}


void Interrupt_Rx_CAN1 () {

	uint8_t Data = ReceiveMsg();
	if (Data==fermer_porte_G) {
		if (LED_status==0) {
			LED_on(1);
			LED_status = 1;
		}
		else {
			LED_off(1);
			LED_status = 0;
		}
	}
}


 int main(void) {
	 
	uint8_t TxData;
	uint8_t length;
	//uint8_t Data;
	LED_status=0;
	 
	init();
	
	TxData = 0xAA;
	length = 1;

	while (1)
	{
		//door_management();//door locking
		//window_management();//the rise of the door window glass’s 
		//send_informations();// send information (rain,battery,speed)
	#ifdef test_emission
		if (bouton4()==1) {
			TransmitMsg(&TxData, length, ID_BCM); //transmet message à BCM
		}
	#endif

	}
	
}
				





 
	
	
	




