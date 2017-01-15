/*
 * Mode_manager.h
 *
 *  Created on: Jan 18, 2015
 *      Author: Antoine
 */

#ifndef MODE_MANAGER_H_
#define MODE_MANAGER_H_
	//Initialisation Modes et Clock selon les specs de Maitre Dubois
//Apr�s l'�xecution, on se trouve dans le mode RUN0
//Les interruptions externes sont autoris�es
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
//voir caract�ristiques d�taill�es sur l'excel RecapRequirements

void GoModeRUN0();
//FMMOD activ�e par d�faut

void GoModeRUN1();


void GoModeRUN2();


void GoModeSTOP();


#endif /* MODE_MANAGER_H_ */
