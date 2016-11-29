/*
 * Comodo.h
 *
 *  Created on: Jan 18, 2015
 *      Author: Antoine
 */

#ifndef COMODO_H_
#define COMODO_H_

void Init_button(void);
uint8_t Read_Steering_Position(void);
uint8_t Read_luminosity_sensor(void);
uint8_t Read_comodo_button(void);
void Turn_on_ADC(void);
void Turn_off_ADC(void);
void start_ADC(void);
void initADC(void);

#endif /* COMODO_H_ */
