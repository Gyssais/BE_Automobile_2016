/*
 * BCM_appli.h
 *
 *  Created on: Jan 19, 2015
 *      Author: Antoine
 */

#ifndef BCM_APPLI_H_
#define BCM_APPLI_H_
#include "define.h"

#ifdef BCM

/*
 * Fonction pricipale du BCM, doit tourner en permanance
 */
void appli_BCM();
void init_appli_BCM();

//void buttons_w_isr();

void Rx_management_bcm(uint8_t Data);
void send_rain_message();
void door_management();// locking door management 
void window_management();//the rise of the door window glass�s 
void send_informations();// send information (rain,battery,speed)

#endif /* BCM */
#endif /* BCM_APPLI_H_ */
