/*
 * window.c
 *
 *  Created on: Jan 11, 2017
 *      Author: Theo
 */

#include "window.h"

MC33887_pinout window_HB = {IN1_W, IN2_W, D2_W, EN_W, FS_W};



void pit_wtch_tempo_isr()
{
	/* clear interrupt flag */
	PIT.CH[CM_PIT_WTCH_TEMPO].TFLG.B.TIF =1;
	
	stopChannelPIT(CM_PIT_WTCH_TEMPO);
	startChannelPIT(CM_PIT_CTU);
}
