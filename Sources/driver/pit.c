/*
 * pit.c
 *
 *  Created on: Dec 11, 2016
 *      Author: Theo
 */

#include "driver/pit.h"




void setupChannelPIT(unsigned int channel, unsigned int period_ms)
{
	if(PIT.PITMCR.B.MDIS == 1) PIT.PITMCR.B.MDIS = 0; // enable PIT
	PIT.PITMCR.B.FRZ =0; // no freeze the timer when in debug mode

	
	PIT.CH[channel].LDVAL.R = period_ms *( F_PIT/1000); // set the reload value 
	PIT.CH[channel].TCTRL.B.TIE = 0; // disable interrupt from this channel. not needed to trigger the CTU.
			
}

void setupISRChannelPIT(unsigned int channel, INTCInterruptFn isr, int priority)
{
	INTC_InstallINTCInterruptHandler(isr, PIT_to_ISR_num(channel), priority);
	PIT.CH[channel].TCTRL.B.TIE = 1; // enable interrupt for this channel
}

void startChannelPIT(unsigned int channel)
{
	PIT.CH[channel].TCTRL.B.TEN = 1;
}

void stopChannelPIT(unsigned int channel)
{
	PIT.CH[channel].TCTRL.B.TEN = 0;
}


int PIT_to_ISR_num(int channel)
{
	switch (channel)
	{
	case 0:
			return PIT0_IRQ;
			break;
	case 1:
			return PIT1_IRQ;
			break;
	case 2:
			return PIT2_IRQ;
			break;
	case 3:
			return PIT3_IRQ; 
			break;
	case 4:
			return PIT4_IRQ;
			break;
	case 5:
			return PIT5_IRQ;
			break;
		
	}
	return -1;
}
