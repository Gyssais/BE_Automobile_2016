/*
 * BCM_appli.c

 *
 *  Created on: Nov 13, 2016
 *      Author: Julien
 */
#include "SBC.h"
#include "Mode_manager.h"
#include "MPC5604B.h"
#include "Buttons_management.h"
#include "pin.h"
#include "BCM_appli.h"


/*
 * Fonction principale du BCM, doit tourner en permanance
 */
void appli_BCM()
{
	while (1)
	{
		door_management();
		window_management();
		send_informations();
		
		// wait (mettre le micro en sommeil)
	}
}



void door_management() {
     if (bouton4() == 1) {
    	 /*send lock_door DCM*/
    	 }
     if(read_speed() >= 10){
    	 /*send lock_door DCM*/
    	 }
}

void window_management(){
	uint8_t DCM_Stat=0;
	uint8_t t=0;
	uint8_t i=0;
	
    if (det_rain() == 1){
        if(read_speed() == 0){
        	/*send close_window DCM*/
        	
            if(DCM_Stat=1 && t<5){
            	/*send DCM*/
            }
            else i++;
            
            if (i<6){
            	/*send DCM*/
            }
            else /*send DCM*/ ;
        }
    }
}

void send_informations(){

	if(bat_min() == 1){/*send DCM*/ }

	if(det_rain() == 1) {/*send DCM*/ }

}
