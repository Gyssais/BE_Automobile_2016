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
#include "define.h"

uint8_t Ack_Leve_Vitre=0;

/*
 * Fonction principale du BCM, doit tourner en permanence
 */
void appli_BCM()
{
	door_management();		//door locking
	window_management();	//the rise of the door window glass’s 
	send_informations();	//send information (rain,battery,speed)
}

/*
 * Interruption lors de la réception d'un message pour le BCM
 */
void Rx_management_bcm (uint8_t Data) {
	
	uint8_t TxData;
	
	switch(data) {
	
	case porte_G_verrouillee:
		TxData = porte_G_verrouillee;
		TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);
		break;
		
	case porte_G_deverrouillee:
		TxData = porte_G_deverrouillee;
		TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);
		TxData = antihijacking_desactive;
		TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);
		break;
		
	case probleme_porte_G:
		TxData = probleme_porte_G;
		TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);
		break;
		
	case pas_probleme_porte_G:
		TxData = pas_probleme_porte_G;
		TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);
		break;
		
	case vitre_G_fermee:
		TxData = vitre_G_fermee;
		TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);
		break;
		
	case vitre_G_ouverte:
		TxData = vitre_G_ouverte;
		TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);
		break;
		
	case probleme_vitre_G:
		TxData = probleme_vitre_G;
		TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);
		break;
		
	case pas_probleme_vitre_G:
		TxData = pas_probleme_vitre_G;
		TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);
		break;
		
	case pincement_vitre_G:
		TxData = pincement_vitre_G;
		TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);
		break;
		
	case pas_pincement_vitre_G:
		TxData = pas_pincement_vitre_G;
		TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);
		break;
		
	case vitres_en_fermeture:
		Ack_Leve_Vitre = 1;
		break;

	default:
		// action
		break;
	}
}

void door_management() {
	uint8_t TxData;
    if (bouton4() == 1) {
    	TxData = fermer_porte_G;
    	/*send lock_door to DCM via the CAN*/
    	TransmitMsg(&TxData, LENGTH_FRAME, ID_DCM);
   	 }
     if (read_speed() >= 10) {
    	TxData = fermer_porte_G;
    	/*send lock_door to  DCM via the CAN*/
    	TransmitMsg(&TxData, LENGTH_FRAME, ID_DCM);
    	TxData = antihijacking_active;
    	TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);
     }
}

void window_management(){
	
	uint8_t t=0;
	uint8_t i=0;
	
    if (det_rain() == 1){
    	// speed <= 3 considere comme a l'arret
        if(read_speed() <= 2){
        	/*send close_window to DCM via the CAN*/
        	TxData = fermer_fenetre_G;
        	TransmitMsg(&TxData, LENGTH_FRAME, ID_DCM);
        	
            if(Ack_Leve_Vitre=1 && t<5){ //TODO : utiliser un timer !
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
	uint8_t TxData;
	uint16_t speed;

	if(bat_min() == 1){/*send to  the instrument cluster “Low battery” via the CAN*/
		TxData = probleme_batterie;
		TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);
	}
	else {
		TxData = pas_probleme_batterie;
		TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);
	}

	if(det_rain() == 1) {/*send  to  the instrument cluster that’s raining via the  CAN */
		TxData = pluie;
		TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);
	}
	
	/*read speed’s values and send them to the instrument cluster via the CAN */
	TxData = (uint8_t) read_speed();
	TxData = TxData|0b10000000; // Bit de poids fort à 1 -> Trame de vitesse
	TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);
}

