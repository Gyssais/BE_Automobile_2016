/*
 * Mode_manager.h
 *
 *  Created on: Jan 18, 2015
 *      Author: Antoine
 */

#ifndef MODE_MANAGER_H_
#define MODE_MANAGER_H_
	//Initialisation Modes et Clock selon les specs de Maitre Dubois
//Après l'éxecution, on se trouve dans le mode RUN0
//Les interruptions externes sont autorisées
void initModesAndClock(void);

//Les 4 boutons en mode poussoir
//Les 4 LEDs en digital OUT etteintes
void config_PORT_E(void) ;

//Autorise les interruptions externes
void enableIrq(void);

//interdit les interruptions externes
void disableIrq(void);

void initPIT1(void);
void PIT1_Interrupt(void);
void disableWatchdog(void);

//passage dans les differents mode
//voir caractéristiques détaillées sur l'excel RecapRequirements

void GoModeRUN0();
//FMMOD activée par défaut

void GoModeRUN1();


void GoModeRUN2();


void GoModeSTOP();


#endif /* MODE_MANAGER_H_ */
