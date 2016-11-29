/*
 * SBC.h
 *
 *  Created on: Jan 18, 2015
 *      Author: Antoine
 */

#ifndef SBC_H_
#define SBC_H_

uint8_t get_flag_frame_received(void);
void reset_flag_frame_received(void);
uint16_t Read_voltage_value(void);
void TransmitMsg(uint8_t * TxData, uint8_t length, uint16_t MsgID);
void ReceiveMsg(void);
void Data_treatment_BCM(uint8_t length , uint8_t * data);
void Data_treatment_LCM( uint8_t length , uint8_t * data);
void Init_SBC_DBG(void);

void initDSPI_1(void);
void initCAN1 (void);


#endif /* SBC_H_ */
