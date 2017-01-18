/*
 * define.h
 *
 *  Created on: Nov 24, 2016
 *      Author: Julien
 */

#ifndef DEFINE_H_
#define DEFINE_H_

#include "MPC5604B.h"


/************************ COMMON PART ***********************************/
// Choix module
//#define BCM //Recepteur tests CAN
#define DCM //Emetteur tests CAN
//#define IC

/*
 * Application global declarations. Must at the end replace config.h. Functions of main.c must be declared here.
 */
void init();


// Pour debug
extern uint8_t LED_status;

/*
 * CAN ID : adresses des modules
 */
#define ID_BCM 111
#define ID_DCM 222
#define ID_IC 333

/*
 * Structure des trame CAN :
 * ID : destinataire
 * Data (1 octet) :
 * 		- premier bits :
 * 							0 -> DonnÃ©e binaire (pluie, porte verouillÃ©e, etc.)
 * 							1 -> DonnÃ©e numÃ©rique (vitesse uniquement)
 * 		- 7 derniers bits : information
 */
#define LENGTH_FRAME 1

// Codes des infos à transmettre par le CAN - TOUT PASSE PAR LE BCM
// Attention ! : la fonction TransmitMsg(...) prend en paramètre un pointeur sur la donnée Ã  transmettre,
// il faut donc définir une variable ayant la valeur définie par le #define (voir BCM_appli.c).

/* EXEMPLE:
 * 		void ma_fonction () {
 * 			uint8_t TxData;
 * 			...
 * 			TxData = Ma_Data;
 * 			TransmitMsg (&TxData, LENGTH_FRAME, ID_Destinataire);
 * 		}
 */

// Statut portes : de DCM vers BCM
#define porte_G_verrouillee  		1
#define porte_G_deverouille 		2
#define probleme_porte_G 			3
#define pas_probleme_porte_G 		4

#define porte_D_verrouillee 		5  // DCM :done
#define porte_D_deverouille 		6  // DCM :done
#define probleme_porte_D 			7
#define pas_probleme_porte_D 		8

// De BCM vers IC
#define antihijacking_active 		9
#define antihijacking_desactive 	10

// Statut fenêtres : de DCM vers BCM
#define vitres_en_fermeture 		11

#define vitre_G_fermee 				12	// DCM :done
#define vitre_G_ouverte 			13	// DCM :done
#define probleme_vitre_G 			14
#define pas_probleme_vitre_G 		15
#define pincement_vitre_G 			16	// DCM :done
#define pas_pincement_vitre_G 		17

#define vitre_D_fermee 				18
#define vitre_D_ouverte 			19
#define probleme_vitre_D 			20
#define pas_probleme_vitre_D 		21
#define pincement_vitre_D 			22
#define pas_pincement_vitre_D 		23

// De BCM vers IC
#define probleme_batterie 			24
#define pas_probleme_batterie 		25

// De BCM vers DCM
#define fermer_fenetre_G			26	// DCM :done
#define fermer_fenetre_D			27
#define fermer_porte_G				28	// DCM :done
#define fermer_porte_D				29
#define ouvrir_fenetre_G			31
#define pluie						30

#define CAN_BUF_ISR_PRIORITY 		12 // priority for the CAN interruption. Same for all platform

#define SBC_INT						PE_10 // Jump this pin to PA_1 because SBC is mapped to it on the board. Also put PA_1 as input


 /* EIRQ isr priority */

#define BUTTON_PRIORITY				3
#define FAULT_PRIORITY				14 // FS from H-bridge and SBC




//#ifdef DCM /********************* DCM PART *****************************/

/* PIT utilization */
#define PIT_ADC			3  // can't be changed !
#define PIT_WTCH_TEMPO	1 // PIT used for tempo between motor start and start current monitoring
#define PIT_MODE_W		2 // PIT used to select automatic or manual mode. 
#define PIT_LOCKING		4

/* DCM pin mapping */

// window H-bridge 
#define IN1_W		PC_4
#define IN2_W		PC_5
#define EN_W		PC_6
#define D2_W		PC_7
#define FS_W		PC_8
#define FB_W		PB_4 // analog


// door locking H-bridge
#define IN1_L		PG_9
#define IN2_L		PG_10
#define EN_L		PG_11
#define D2_L		PG_12
#define FS_L		PG_13

// window buttons
#define BUTTON_UP	PA_6		
#define BUTTON_DOWN PA_7

// locking button
#define LOCK_BUTTON	PA_8

/* DCM ISR priority */
#define WTCH_ISR_PRIORITY		10
#define EOCTU_ISR_PRIORITY		5
#define PIT_WTCH_TEMPO_PRIORITY 6
#define PIT_LOCKING_PRIOTITY	7


/* windows state definition */
#define STOPPED		0
#define UP			1
#define DOWN		2
/* windows position definition */
#define CLOSED		3
#define OPEN		4
#define UNKNOW		5

/* Door state definition */
#define LOCKED		porte_G_verrouillee
#define UNLOCKED	porte_G_deverouille

extern uint8_t window_position;
extern uint8_t window_state;
extern uint8_t door_state;

//#endif /***************** END DCM PART ******************/



#endif /* DEFINE_H_ */
