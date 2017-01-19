/*
 * pit.h
 *
 *  Created on: Dec 11, 2016
 *      Author: Theo
 */

#ifndef PIT_H_
#define PIT_H_

#include "interrupt_number.h"
#include "IntcInterrupts.h"
#include "MPC5604B.h"

#define F_PIT 64000000  // Hz 

void setupChannelPIT(unsigned int channel, unsigned int period_ms); // setup the corresponding channel to generate an interrupt every period
void startChannelPIT(unsigned int channel);
void stopChannelPIT(unsigned int channel);
void setupISRChannelPIT(unsigned int channel, INTCInterruptFn isr, int priority);
void clearInterruptFlagPIT(unsigned int channel);
int PIT_to_ISR_num(int channel);
#endif /* PIT_H_ */
