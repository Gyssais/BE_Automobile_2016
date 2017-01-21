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

#ifdef BCM

uint8_t Ack_Leve_Vitre=0;
uint8_t nb_sending_try=0;
uint8_t etat_porte=0;
uint8_t autoriser_antihijacking=1; //autorise par defaut
uint8_t speed=0;
uint8_t antihijacking_actif=0;
uint8_t old_antihijacking_actif=0;

void button_bcm()
{
	uint8_t TxData;
	autoriser_antihijacking=0; //desactivation antihijacking -> mode manuel prioritaire
	antihijacking_actif=0;
	if (etat_porte==0)
	{
		TxData = fermer_porte_G;
		/*send lock_door to DCM via the CAN*/
		TransmitMsg(&TxData, LENGTH_FRAME, ID_DCM);
		etat_porte=1;
	}
	else
	{
		TxData = ouvrir_porte_G;
		/*send unlock_door to DCM via the CAN*/
		TransmitMsg(&TxData, LENGTH_FRAME, ID_DCM);
		etat_porte=0;
	}
}

/*
 * Fonction principale du BCM, doit tourner en permanence
 */
void appli_BCM()
{
	//door_management();		//door locking
	window_management();	//the rise of the door window glass's 
	//send_informations();	//send information (rain,battery,speed) -> gere par interruption sur timer

}

void init_appli_BCM()
{
	init_speed_button();
	// Timer pour send_informations
	setupChannelPIT(PIT_LOCK,10); // 10 ms
	setupISRChannelPIT(PIT_LOCK, send_informations, 20);
	startChannelPIT(PIT_LOCK);
	// Timer pour antihijacking
	setupChannelPIT(PIT_HIJACK,1000); // 1 s
	setupISRChannelPIT(PIT_HIJACK, door_management, 50);
	startChannelPIT(PIT_HIJACK);
	
}


/*
 * Interruption lors de la réception d'un message pour le BCM
 */
void Rx_management_bcm (uint8_t Data) {
	
	uint8_t TxData;
	
	switch(Data) {
	
	case porte_G_verrouillee:
		TxData = porte_G_verrouillee;
		TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);
		break;
		
	case porte_G_deverouille:
		TxData = porte_G_deverouille;
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
    if ( (autoriser_antihijacking==1) && (etat_porte==0) && (speed >= 10))
    {
    	TxData = fermer_porte_G;
    	/*send lock_door to  DCM via the CAN*/
    	TransmitMsg(&TxData, LENGTH_FRAME, ID_DCM);
    	//TxData = antihijacking_active;
    	//TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);
    	antihijacking_actif=1;
    	etat_porte = 1;
    }
    else if (speed < 10)
    {
    	autoriser_antihijacking=1;
    	antihijacking_actif=0;
    }
    PIT.CH[PIT_HIJACK].TFLG.B.TIF =1;
}

void send_rain_message() // Executée sur interruption timer
{
	uint8_t TxData = fermer_fenetre_G;
	TransmitMsg(&TxData, LENGTH_FRAME, ID_DCM);
	// Pas de porte droite dans demo
	nb_sending_try++;
	PIT.CH[PIT_RAIN].TFLG.B.TIF =1;
}


void window_management() {
	
	setupChannelPIT(4,5000); // 5 secondes
	setupISRChannelPIT(4, send_rain_message,10);
	
    if (det_rain() == 1) {
        if(speed < 2) {
        	//send close_window to DCM via the CAN
        	send_rain_message();
        	startChannelPIT(PIT_RAIN);
        	while(Ack_Leve_Vitre == 0 && nb_sending_try<6)
        	{}	// Attente reception ACK
        	stopChannelPIT(PIT_RAIN);
        	nb_sending_try = 0;
        }
    }
}


void send_informations(){
	uint8_t TxData;
	//uint16_t speed;

	if(bat_min() == 1){/*send to  the instrument cluster â€œLow batteryâ€� via the CAN*/
		TxData = probleme_batterie;
		TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);
	}
	else {
		TxData = pas_probleme_batterie;
		TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);
	}

	if(det_rain() == 1) {/*send  to  the instrument cluster that's raining via the  CAN */
		TxData = pluie;
		TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);
	}
	
	if (old_antihijacking_actif != antihijacking_actif)
	{
		if (antihijacking_actif == 1) {
			TxData = antihijacking_active;
			TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);	
		}
		else {
			TxData = antihijacking_desactive;
			TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);	
		}
		
		old_antihijacking_actif = antihijacking_actif;
	}
	
	/*read speed values and send them to the instrument cluster via the CAN */
	speed = (uint8_t) read_speed();
	TxData = speed|0b10000000; // Bit de poids fort Ã  1 -> Trame de vitesse
	TransmitMsg(&TxData, LENGTH_FRAME, ID_IC);
	
	PIT.CH[PIT_LOCK].TFLG.B.TIF =1;
}

#endif
