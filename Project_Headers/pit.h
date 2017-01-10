/*
 * pit.h
 *
 *  Created on: Dec 11, 2016
 *      Author: Theo
 */

#ifndef PIT_H_
#define PIT_H_


#define F_PIT 64000000  // Hz 

void setupChannelPIT(unsigned int channel, unsigned int period_ms); // setup the corresponding channel to generate an interrupt every period
void startChannelPIT(unsigned int channel);
void stopChannelPIT(unsigned int channel);

#endif /* PIT_H_ */
