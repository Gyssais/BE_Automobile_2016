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
 * 		- 2 premiers bits :
 * 							00 -> Donnée binaire (pluie, porte verouillée, etc.)
 * 							11 -> Donnée numérique (vitesse uniquement)
 * 		- 6 derniers bits : information
 */
#define LENGTH_FRAME 1

// Codes des infos à transmettre par le CAN
// Attention ! : la fonction TransmitMsg(...) prend en paramètre un pointeur sur la donnée à transmettre,
// il faut donc définir une variable ayant la valeur définie par le #define (voir BCM_appli.c).
// TODO Si quelqu'un a une meilleure, je prends

// Statut portes : de DCM vers IC
#define porte_G_verrouillee  		1
#define porte_G_deverouille 		2
#define probleme_porte_G 			3
#define pas_probleme_porte_G 		4

#define porte_D_verrouillee 		5
#define porte_D_deverouille 		6
#define probleme_porte_D 			7
#define pas_probleme_porte_D 		8

// De BCM vers IC
#define antihijacking_active 		9
#define antihijacking_desactive 	10

// Statut fenêtres : de DCM vers IC
#define vitres_en_fermeture 		11

#define vitre_G_fermee 				12
#define vitre_G_ouverte 			13
#define probleme_vitre_G 			14
#define pas_probleme_vitre_G 		15
#define pincement_vitre_G 			16
#define pas_pincement_vitre_G 		17

#define vitre_D_fermee 				18
#define vitre_D_ouverte 			19
#define probleme_vitre_D 			20
#define pas_probleme_vitre_D 		21
#define pincement_vitre_D 			22
#define pas_pincement_vitre_D 		23

// De DCM vers IC
#define probleme_batterie 			24
#define pas_probleme_batterie 		25

// De BCM vers DCM
#define fermer_fenetre_G			26
#define fermer_fenetre_D			27
#define fermer_porte_G				28
#define fermer_porte_D				29

#define pluie						30


/*
 * Test functions
 */
#define AD_PIN PB_4

void adc_example();
void ctu_trigger_example();
void adc_watchdog_example();

#endif /* DEFINE_H_ */
