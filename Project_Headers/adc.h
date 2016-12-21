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

#define PRECISION_CHANNEL	0
#define STANDARD_CHANNEL	1
	
/* EOC interrupt mask */
/* define the events that can trigger the AD_EOC interrupt signal (cf interrupt_number.h, reference manual p602) */
#define EOCTU_MASK	0x10
#define JEOC_MASK	0x08
#define JECH_MASK	0x04
#define EOC_MASK	0x02
#define ECH_MASK	0x01

int setupADC(); /* init the adc, disable all channel */
int setupPin_ADC(unsigned int pin); /* enable the analog channel corresponding to the given channel */
int setupPin_ADC_Interrupt(unsigned int pin, unsigned int interrupt_flag); // set the global flags and enable the interrupt from the pin
int pinToADCChannel(unsigned int pin, char * channel, char *type);
void enableADC();
void startConversion();
int analogRead(unsigned int pin); /* start a conversion and return the result. the pin must be configured before with setupPin_ADC()*/
void attachInterrupt_ADC_EOC(INTCInterruptFn isr, unsigned char priority);


int setupAnalogWatchdog(int pin, unsigned int high_threshold, unsigned int low_threshold, int watchdog);
void attachInterrupt_ADC_WTCH(INTCInterruptFn isr, unsigned char priority);
void startAnalogWatchdog(int watchdog);
void stopAnalogWatchdog(int watchdog);


/* setup the adc channel to be used by the PIT3 channel through CTU. */
int ADCChannelToCTUChannel(unsigned int adc_channel);
void setupChannel_CTU_trigger(unsigned int adc_channel);

#endif /* ADC_H_ */
