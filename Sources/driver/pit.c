/*
 * pit.c
 *
 *  Created on: Dec 11, 2016
 *      Author: Theo
 */

#include "pit.h"
#include "MPC5604B.h"


void setupChannelPIT(unsigned int channel, unsigned int period)
{
	if(PIT.PITMCR.B.MDIS == 1) PIT.PITMCR.B.MDIS = 0; // enable PIT
	
	PIT.CH[channel].LDVAL.R = (period * F_PIT); // set the reload value
	PIT.CH[channel].TCTRL.B.TIE = 1; // enable interrupt from this channel
			
}

void startChannelPIT(unsigned int channel)
{
	PIT.CH[channel].TCTRL.B.TEN = 1;
}

void stopChannelPIT(unsigned int channel)
{
	PIT.CH[channel].TCTRL.B.TEN = 0;
}
