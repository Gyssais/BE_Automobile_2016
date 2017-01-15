/*
 * BCM_appli.c

 *
 *  Created on: Nov 13, 2016
 *      Author: Julien
 */
#include "driver/SBC.h"
#include "service/Mode_manager.h"
#include "driver/MPC5604B.h"
#include "service/Buttons_management.h"
#include "pin.h"
#include "application/BCM_appli.h"
#include "define.h"


uint8_t pb_b=probleme_batterie;
uint8_t p_pb_b=pas_probleme_batterie;
uint8_t f_f_G=fermer_fenetre_G;
uint8_t f_f_D=fermer_fenetre_D;
uint8_t f_p_G=fermer_porte_G;
uint8_t f_p_D=fermer_porte_D;
uint8_t p=pluie;


/*
 * Fonction principale du BCM, doit tourner en permanance
 */
void appli_BCM()
{

	door_management();//door locking
	window_management();//the rise of the door window glass’s 
	send_informations();// send information (rain,battery,speed)
	
}



void door_management() {
     if (bouton4() == 1) {
    	 /*send lock_door to DCM via the CAN*/
    	 TransmitMsg(&f_p_G, LENGTH_FRAME, ID_DCM);
    	 TransmitMsg(&f_p_D, LENGTH_FRAME, ID_DCM);
    	 
    	 }
     if(read_speed() >= 10){
    	 /*send lock_door to  DCM via the CAN*/
    	 TransmitMsg(&f_p_G, LENGTH_FRAME, ID_DCM);
    	 TransmitMsg(&f_p_D, LENGTH_FRAME, ID_DCM);
    	 }
}

void window_management(){
	uint8_t DCM_Stat=0;
	uint8_t t=0;
	uint8_t i=0;
	
    if (det_rain() == 1){
        if(read_speed() == 0){
        	/*send close_window to DCM via the CAN*/
        	TransmitMsg(&f_f_G, LENGTH_FRAME, ID_DCM);
        	TransmitMsg(&f_f_D, LENGTH_FRAME, ID_DCM);
        	
            if(DCM_Stat=1 && t<5){ //TODO : utiliser un timer !
            	/*it’s what we want */
            	//TODO : à finir (à quoi sert DCM_Stat ?)
            }
            else i++;
            
            if (i<6){ //TODO : utiliser un timer !
            	/*resend to DCM again via the CAN*/
            }
            else /*BCM give up*/  ;
        }
    }
}

void send_informations(){

	if(bat_min() == 1){/*send to  the instrument cluster “Low battery” via the CAN*/
		TransmitMsg(&pb_b, LENGTH_FRAME, ID_IC);
	}
	else TransmitMsg(&p_pb_b, LENGTH_FRAME, ID_IC);

	if(det_rain() == 1) {/*send  to  the instrument cluster that’s raining via the  CAN */
		TransmitMsg(&p, LENGTH_FRAME, ID_IC);
		// TODO : Il ne faut pas fermer les fenêtres aussi ?
	}
	// TODO : read_speed() juste comme ça ne sert à rien, à compéter
	read_speed(); /*read speed’s values and send them to the instrument cluster via the CAN */
	
}
