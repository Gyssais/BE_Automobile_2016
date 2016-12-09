/*
 * adc.h
 *
 *  Created on: Nov 18, 2016
 *      Author: Theo
 */

#ifndef ADC_H_
#define ADC_H_

// ADC error code
#define WRONG_PIN			-1
#define CHANNEL_DISABLED 	-2
#define UNVALID_DATA 		-3
#define WRONG_WATCHDOG		-4

#define ADC_MAX				1024
#define ADC_MIN				0


int setupAdc();
int setupPin(int pin);
int pinToAdcChannel(int pin, char * channel, char *type);
void enableADC();
void startConversion();
int analogRead(int pin);


int setupAnalogWatchdog(int pin, int high_threshold, int low_threshold, int watchdog);
void startAnalogWatchdog(int watchdog);
void stopAnalogWatchdog(int watchdog);


#endif /* ADC_H_ */
