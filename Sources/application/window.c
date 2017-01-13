/*
 * window.c
 *
 *  Created on: Jan 11, 2017
 *      Author: Theo
 */

#include "window.h"

MC33887_pinout window_HB = {IN1_W, IN2_W, D2_W, EN_W, FS_W};



int init_window()
{
	window_state = STOPPED;
	window_position = UNKNOW;
	setupChannelPIT(PIT_MODE_W, PIT_MODE_W_TEMPO);
	setup_buttons();
	init_HBridge(&window_HB);
	cm_initialize();
}


void pit_wtch_tempo_isr()
{
	/* clear interrupt flag */
	PIT.CH[CM_PIT_WTCH_TEMPO].TFLG.B.TIF =1;
	
	stopChannelPIT(CM_PIT_WTCH_TEMPO);
	startChannelPIT(CM_PIT_CTU);
}


void buttons_isr()
{
	
	if((SIU.ISR.R & 0x1))  // if isr raised by PA_3
	{
		stop_HBridge(&window_HB); 
	}
	
	if((SIU.ISR.R & 0x2) && SIU.GPDI[BUTTON_UP].B.PDI) //if isr raised by PA_6 rising edge
	{
		if(window_state == STOPPED) 
		{
			start_HBridge(&window_HB,SENS1); 
			window_state = DOWN;
			startChannelPIT(CM_PIT_WTCH_TEMPO);
		}
		else if(window_state == DOWN) 
		{
			stop_HBridge(&window_HB);
			stop_PITs();
			window_state = STOPPED;
		}
		
	}
	
	if((SIU.ISR.R & 0x4) && SIU.GPDI[BUTTON_DOWN].B.PDI) //if isr raised by PA_7 rising edge
	{
		if(window_state == STOPPED) 
		{ 
			start_HBridge(&window_HB, SENS2); 
			window_state = UP;
			startChannelPIT(CM_PIT_WTCH_TEMPO);
		}
		else if(window_state == UP) 
		{
			stop_HBridge(&window_HB);
			stop_PITs();
			window_state = STOPPED;
		}
	}
	
	/* clear interrupt flag */
		// clear all isr
	SIU.ISR.R = 0xFFFF;
	
	/* toggle LED_1 */
	if(SIU.GPDO[PE_4].R == 1) SIU.GPDO[PE_4].R = 0;
	else SIU.GPDO[PE_4].R = 1;
	
}

int setup_buttons()
{
	int result=0;
	
	pinMode(BUTTON_UP, INPUT);
	pinMode(BUTTON_DOWN, INPUT);
	
	result = setup_EIRQ_pin(BUTTON_UP, BOTH);
	result = setup_EIRQ_pin(BUTTON_DOWN, BOTH);
	attachInterrupt_EIRQ0(buttons_isr, EIRQ0_PRIORITY);
	
	return result;
}

void stop_PITs()
{
	stopChannelPIT(CM_PIT_CTU);
	stopChannelPIT(CM_PIT_WTCH_TEMPO);
	stopChannelPIT(PIT_MODE_W);
}
