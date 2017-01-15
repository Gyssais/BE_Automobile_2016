/*
 * SBC.h
 *
 *  Created on: Jan 18, 2015
 *      Author: Antoine
 */

#ifndef SBC_H_
#define SBC_H_

#include "MPC5604B.h"


/*
 * Initialisation du CAN 1
 * Buffer 0 pour �mission, buffer 1 pour r�ception
 * Interruption configur�e sur r�ception
 */
void initCAN1 (void);

/*
 * Fonction d'interruption activ�e � chaque r�ception de message, � �crire dans le main.
 */
void Interrupt_Rx_CAN1 ();

/*
 * Extraction des donn�es d'un message. A utiliser dans Interrupt_Rx_CAN1().
 * Renvoie la donn�e transmise. Voir define.h pour la structure des trames.
 */
uint8_t ReceiveMsg(void);

/*
 * Emission d'un message sur le CAN.
 * 		- TxData : pointeur sur la donn�e � transmettre
 * 		- length : des messages d'un octet devraient suffir -> utiliser LENGTH_FRAM
 * 		- MsgID : adresse de destination du message (ID_IC, ID_BCM ou ID_DCM)
 */
void TransmitMsg(uint8_t * TxData, uint8_t length, uint16_t MsgID);



// Pour SPI
uint8_t get_flag_frame_received(void);
void reset_flag_frame_received(void);
uint16_t Read_voltage_value(void);
void Init_SBC_DBG(void);

//TODO � supprimer
void Data_treatment_BCM(uint8_t length , uint8_t * data);
void Data_treatment_LCM( uint8_t length , uint8_t * data);


#endif /* SBC_H_ */
