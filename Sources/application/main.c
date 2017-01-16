/************ Include *********************/
#include "MPC5604B.h"
#include "IntcInterrupts.h"
#include "config.h" //TODO à supprimer (remplacé par define.h)
#include "SBC.h"
#include "Mode_manager.h"
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
#include "window.h"



/*************** Private function prototype **********/

/*************** Public function            **********/


//TODO : mettre tous les pins mapping dans define.h
//TODO : faire un fichier windows.c et windows.h
//TODO : changer le threshold du watchdog selon le sens du moteur


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
	init_window();
	
}

/*
 * Fonction de gestion de l'interruption sur reception d'un message par le CAN
 * Utiliser la fonction ReceiveMsg() pour récupérer le message
 */
void Interrupt_Rx_CAN1 () {

#ifdef TEST_RECEPTION
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
#endif
#ifdef BCM
	//TODO: Rx_management_bcm();
#endif
#ifdef DCM
	//TODO: Rx_mangement_dcm();
#endif
}


 int main(void) {
	
#ifdef TEST_EMISSION
	uint8_t TxData;
	uint8_t length;
	TxData = 0xAA;
	length = 1;
	LED_status=0;
#endif
	
	init();
	
	

	while (1)
	{
#ifdef TEST_RECEPTION
		if (bouton4()==1) {
			TransmitMsg(&TxData, length, ID_BCM); //transmet message à BCM
		}
#endif
#ifdef BCM
		appli_BCM();
#endif
#ifdef DCM
		//TODO: appli_dcm();
#endif
	}
 }
	

				





 
	
	
	




