/*
 * can_drv.h
 *
 *  Created on: Jan 18, 2017
 *      Author: Gido
 */

/*
 * This driver is used for transmission via the CAN bus.
 * It uses the FlexCAN1 unit from the MPC5604B.
 * initCAN1 : initialises the FlexCAN1
 * TransmitMsg : transmits messages on TX pin
 * ReceiveMsg : receives messages on RX pin
 */

#ifndef CAN_DRV_H_
#define CAN_DRV_H_

/*
 * initCAN1
This function initialises the FlexCAN1 module. 
All 64 message buffers (MB) are set to inactive, 
except the MB0 for transmission and MB1 for reception. 
The MB1 is also set to filter only messages with the correct ID, 
and triggers an interrupt when a correct message is received.
*/
void initCAN1 (void);

/*
 * TransmitMsg
This function is used for data transmission 
via the CAN bus, using three parameters: 
Data to be sent (max. 8 bytes), 
its length (in bytes) 
and the message standard ID.
 */
void TransmitMsg(uint8_t * TxData, uint8_t length, uint16_t MsgID);

/*
 * ReceiveMsg
This function is used for data reception on MB 1 
with data ID=ID_BCM, ID_DCM or ID_IC., 
and returns the data received.
 */
uint8_t ReceiveMsg(void);

#endif /* CAN_DRV_H_ */
