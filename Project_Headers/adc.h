/*
 * adc.h
 *
 *  Created on: Nov 18, 2016
 *      Author: Theo
 */

#ifndef ADC_H_
#define ADC_H_

#include "IntcInterrupts.h"

// ADC error code
#define WRONG_PIN			-1
#define CHANNEL_DISABLED 	-2
#define UNVALID_DATA 		-3
#define WRONG_WATCHDOG		-4

#define ADC_MAX				1024
#define ADC_MIN				0


int setupAdc(); /* init the adc, disable all channel */
int setupPin(int pin); /* enable the analog channel corresponding to the give channel */
int pinToAdcChannel(int pin, char * channel, char *type);
void enableADC();
void startConversion();
int analogRead(int pin); /* start a conversion and return the result. the pin must be configured before with setupPin()*/


int setupAnalogWatchdog(int pin, unsigned int high_threshold, unsigned int low_threshold, int watchdog);
int setupAnalogWatchdogISR(INTCInterruptFn isr);
void startAnalogWatchdog(int watchdog);
void stopAnalogWatchdog(int watchdog);


/* setup the adc channel to be used by the PIT3 channel through CTU. */
int adcChannelToCTUChannel(unsigned int adc_channel);
void setup_CTU_PIT(unsigned int adc_channel);

#endif /* ADC_H_ */
