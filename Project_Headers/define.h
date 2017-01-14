/*
 * define.h
 *
 *  Created on: Nov 24, 2016
 *      Author: Julien
 */

#ifndef DEFINE_H_
#define DEFINE_H_

#include "MPC5604B.h"

// Choix module
#define BCM //Recepteur tests CAN
//#define DCM //Emetteur tests CAN
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
 * 		- 2 premiers bits :
 * 							00 -> Donnée binaire (pluie, porte verouillée, etc.)
 * 							11 -> Donnée numérique (vitesse uniquement)
 * 		- 6 derniers bits : information
 */



/*
 * Test functions
 */
#define AD_PIN PB_4

void adc_example();
void ctu_trigger_example();
void adc_watchdog_example();

#endif /* DEFINE_H_ */
