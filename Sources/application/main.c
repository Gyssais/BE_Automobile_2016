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
}


 int main(void) {
	 
	uint8_t TxData;
	uint8_t length;
	uint8_t Data;
	uint8_t i;
	 
	init();
	
	TxData = 0xAA;
	length = 1;
	//TransmitMsg(&TxData, length, 555);
	
	Data = ReceiveMsg();
	
	i++;
	
	while(1) {}
	
}
				





 
	
	
	




