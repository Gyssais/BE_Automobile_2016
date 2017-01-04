/*
 * BCM_appli.h
 *
 *  Created on: Jan 19, 2015
 *      Author: Antoine
 */

#ifndef BCM_APPLI_H_
#define BCM_APPLI_H_

/* OLD */
void Update_light(void);
void Normal_mode_BCM(void);
void Low_power_mode_BCM(void);
void Failsoft_mode_BCM(void);
void scheduler_BCM(void);
/* OLD */

/*
 * Fonction pricipale du BCM, doit tourner en permanance
 */
void appli_BCM();


void door_management();
void window_management();
void send_informations();

#endif /* BCM_APPLI_H_ */
