/*
 * define.h
 *
 *  Created on: Nov 24, 2016
 *      Author: Julien
 */

#ifndef DEFINE_H_
#define DEFINE_H_

// Choix module
#define BCM //Recepteur tests CAN
//#define DCM //Emetteur tests CAN
//#define IC


/*
 * Application global declarations. Must at the end replace config.h. Functions of main.c must be declared here.
 */


/*
 * CAN ID : adresses des modules
 */
#define ID_BCM 111
#define ID_DCM 222
#define ID_IC 333



/*
 * Test functions
 */
#define AD_PIN PB_4

void adc_example();
void ctu_trigger_example();
void adc_watchdog_example();

#endif /* DEFINE_H_ */
