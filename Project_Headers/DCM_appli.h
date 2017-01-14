/*
 * DCM_appli.h
 *
 *  Created on: Nov 13, 2016
 *      Author: Julien
 */

#ifndef DCM_APPLI_H_
#define DCM_APPLI_H_

//TODO
void open_window();
void close_window();
void lock_door();
void unlock_door();
void appli_dcm(); // fonction générale qui exécute toutes les fonctions prévues pour le DCM (avec envoi/réception des messages CAN notammant)
// end TODO

#endif /* DCM_APPLI_H_ */
