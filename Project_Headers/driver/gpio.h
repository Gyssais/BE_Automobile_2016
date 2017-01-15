/*
 * gpio.h
 *
 *  Created on: Jan 10, 2017
 *      Author: Theo
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "pin.h"
#include "driver/MPC5604B.h"
#include "interrupt_number.h"
#include "driver/IntcInterrupts.h"


#define INPUT			0
#define OUTPUT			1

#define WRONG_PIN		-1
#define WRONG_MODE		-2
#define WRONG_CONF		-3
#define RISING			1
#define FALLING			2
#define BOTH			3


void pinMode(int pin, int mode);
int digitalWrite(int pin, int value);
int digitalRead(int pin); 

int setup_EIRQ_pin(int pin, int mode);

// old EIRQ functions
/*
int setup_EIRQ0_pin(int pin, int mode);
int setup_EIRQ1_pin(int pin, int mode);
*/

void attachInterrupt_EIRQ0(INTCInterruptFn isr, unsigned char priority); // same ISR for the 8 source of EIRQ0. check in the ISR who raise the interrupt
void attachInterrupt_EIRQ1(INTCInterruptFn isr, unsigned char priority); // same ISR for the 8 source of EIRQ1. check in the ISR who raise the interrupt

// search in the look up table the corresponding irq number.
char pin_to_EIRQ(int pin);


// look up table for pin/eirq corresponding
extern char eirq_to_pin[16];




#endif /* GPIO_H_ */
