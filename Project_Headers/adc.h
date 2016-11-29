/*
 * adc.h
 *
 *  Created on: Nov 18, 2016
 *      Author: Theo
 */

#ifndef ADC_H_
#define ADC_H_

int setupAdc();
int setupPin(int pin);
int pinToAdcChannel(int pin, char * channel, char *type);
void enableADC();
void startConversion();
int analogRead(int pin);

#endif /* ADC_H_ */
