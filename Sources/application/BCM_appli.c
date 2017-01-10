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
		door_management();//door locking
		window_management();//the rise of the door window glass’s 
		send_informations();// send information (rain,battery,speed)
		
		// wait (mettre le micro en sommeil)
	}
}



void door_management() {
     if (bouton4() == 1) {
    	 /*send lock_door to  DCM via the CAN*/
    	 }
     if(read_speed() >= 10){
    	 /*send lock_door to  DCM via the CAN*/
    	 }
}

void window_management(){
	uint8_t DCM_Stat=0;
	uint8_t t=0;
	uint8_t i=0;
	
    if (det_rain() == 1){
        if(read_speed() == 0){
        	/*send close_window to DCM via the CAN*/
        	
            if(DCM_Stat=1 && t<5){
            	/*it’s  what we want */
            }
            else i++;
            
            if (i<6){
            	/*resend to DCM again via the CAN*/
            }
            else /*BCM give up*/  ;
        }
    }
}

void send_informations(){

	if(bat_min() == 1){/*send to  the instrument cluster “Low battery” via the CAN*/ }

	if(det_rain() == 1) {/*send  to  the instrument cluster  that’s rainning via the  CAN */  }
	read_speed(); /*read speed’s values and send them to the instrument cluster via the CAN */
	
}

