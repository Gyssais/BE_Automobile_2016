/*
 * gpio.h
 *
 *  Created on: Jan 10, 2017
 *      Author: Theo
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "pin.h"
#include "MPC5604B.h"
#include "interrupt_number.h"
#include "IntcInterrupts.h"

enum eirq_mode {RISING, FALLING, BOTH};

#define INPUT			0
#define OUTPUT			1

#define WRONG_PIN		-1
#define WRONG_MODE		-2
#define RISING			1
#define FALLING			2
#define BOTH			3


void pinMode(int pin, int mode);
void digitalWrite(int pin, int value);
int digitalRead(int pin); 

int setup_EIRQ0_pin(int pin, int mode);
int setup_EIRQ1_pin(int pin, int mode);

void attachInterrupt_EIRQ0(INTCInterruptFn isr, unsigned char priority); // same ISR for the 8 source of EIRQ0. check in the ISR who raise the interrupt
void attachInterrupt_EIRQ1(INTCInterruptFn isr, unsigned char priority); // same ISR for the 8 source of EIRQ1. check in the ISR who raise the interrupt

#endif /* GPIO_H_ */
