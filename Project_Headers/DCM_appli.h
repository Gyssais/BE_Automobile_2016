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
void appli_dcm(); // fonction g�n�rale qui ex�cute toutes les fonctions pr�vues pour le DCM (avec envoi/r�ception des messages CAN notammant)
// end TODO

#endif /* DCM_APPLI_H_ */
