/*
 * pwm.h
 *
 *  Created on: Jan 14, 2017
 *      Author: Julien, Taha
 */

#ifndef PWM_H_
#define PWM_H_

void initEMIOS_0(void);
void initEMIOS_0ch23(void);
void initEMIOS_0ch21(int x);  //x varie de 0 à 999 telque le rapport cyclique serait x/1000
void initEMIOS_0ch22(void);

//TODO
void set_duty_cycle(uint8_t channel, uint8_t duty_cycle);

/*
 * Configure pin en digital output. TODO à passer dans gpio.c
 */
void Configure_to_DO(uint32_t num_pin);

/*
 * Ecrit état du pin : 0 ou 1
 * si i>0 -> pin à 1
 * TODO passer dans gpio.c -> DigitalWrite(...)
 */
void Write_to_output(uint32_t num_pin, uint8_t i);

/*
 * Lit valeur pin.
 * TODO à passer dans gpio.c -> DigitalRead()
 */
uint8_t Read_input(uint32_t num_pin);

#endif /* PWM_H_ */
