/*
 * window.c
 *
 *  Created on: Jan 11, 2017
 *      Author: Theo
 */




#include "window.h"
#include "pwm.h"
#include "gpio.h"

#ifdef DCM

MC33887_pinout window_HB = {IN1_W, IN2_W, D2_W, EN_W, FS_W, 0};
 
char button_up_irq_mask; 
char button_down_irq_mask;

int init_window()
{
	button_up_irq_mask = (1<<pin_to_EIRQ(BUTTON_UP));
	button_down_irq_mask = (1<<pin_to_EIRQ(BUTTON_DOWN));
	
	window_state = STOPPED;
	window_position = UNKNOW;
	setupChannelPIT(PIT_MODE_W, PIT_MODE_W_TEMPO);


	setup_buttons_w();
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


void buttons_w_isr()
{
	
	/* if isr raised by BUTTON_UP rising edge */
	if((SIU.ISR.R & button_up_irq_mask) && SIU.GPDI[BUTTON_UP].B.PDI) 
	{
		if(window_state == STOPPED) window_up();
		else window_stop();
	}
	
	
	/* if isr raised by BUTTON_DOWN rising edge */
	if((SIU.ISR.R & button_down_irq_mask) && SIU.GPDI[BUTTON_DOWN].B.PDI) 
	{
		if(window_state == STOPPED)  window_down();
		else window_stop();

	}
	
	/* if isr raised by BUTTON_UP falling edge  */
	if((SIU.ISR.R & button_up_irq_mask) && (SIU.GPDI[BUTTON_UP].B.PDI== 0) ) 
		{
			if(window_state == UP)
			{	
				//  check if PIT_MOD_W > 100ms
				stopChannelPIT(PIT_MODE_W);
				/* if more than 100 ms has elapsed since the motor has started => manual mode => stop the motor. Else automatic mode */
				if(PIT.CH[PIT_MODE_W].TFLG.B.TIF == 1) window_stop();
			}
		}
	
	
	/* if isr raised by BUTTON_DOWN falling edge */
	if((SIU.ISR.R & button_down_irq_mask) && (SIU.GPDI[BUTTON_DOWN].B.PDI== 0)) 
		{
			if(window_state == DOWN)
			{
				//  check if PIT_MOD_W > 100ms
				stopChannelPIT(PIT_MODE_W);
				/* if more than 100 ms has elapsed since the motor has started => manual mode => stop the motor. Else automatic mode */
				if(PIT.CH[PIT_MODE_W].TFLG.B.TIF == 1) window_stop();
			}
		
		}
	
	/* clear interrupt flag */
		// clear all EIRQ0 isr
	//SIU.ISR.R = 0x00FF;  // now done in the global isr in the main
	
	/* for test & debug */
	/* toggle LED_1 */
	if(SIU.GPDO[PE_4].R == 1) SIU.GPDO[PE_4].R = 0;
	else SIU.GPDO[PE_4].R = 1;
	
}

int setup_buttons_w()
{
	int result=0;
	
	pinMode(BUTTON_UP, INPUT);
	pinMode(BUTTON_DOWN, INPUT);
	
	result = setup_EIRQ_pin(BUTTON_UP, BOTH);
	result = setup_EIRQ_pin(BUTTON_DOWN, BOTH);
	//attachInterrupt_EIRQ0(buttons_w_isr, EIRQ0_PRIORITY); now done in with  the global ISR  in the main
	
	return result;
}

void stop_PITs()
{
	stopChannelPIT(CM_PIT_CTU);
	stopChannelPIT(CM_PIT_WTCH_TEMPO);
	stopChannelPIT(PIT_MODE_W);
	PIT.CH[PIT_MODE_W].TFLG.B.TIF= 1 ; 
}



void window_up()
{
	uint8_t msg = vitres_en_fermeture;
#ifdef PWM
	digitalWrite(PA_1,1);
	digitalWrite(PA_0,0);   
	//initEMIOS_0ch21(900);
	start_PWM_0(21, 90); //Rapport cyclique de 90% à cause de PA_0 à 0
#endif
	
	//ADC.THRHLR[CM_WTCH].B.THRH = CM_WTCH_HIGH_T_UP; // test two threshold
	
	start_HBridge(&window_HB, SENS1); 
	window_state = UP;
	startChannelPIT(CM_PIT_WTCH_TEMPO);
	startChannelPIT(PIT_MODE_W);
	TransmitMsg(&msg, LENGTH_FRAME, ID_BCM);
}

void window_down()
{
#ifdef PWM
	digitalWrite(PA_1,1);
	digitalWrite(PA_0,1); 
	//initEMIOS_0ch21(300);
	start_PWM_0(21, 30); //Rapport cyclique de 70% à cause de PA_0 à 1
#endif
	
	//ADC.THRHLR[CM_WTCH].B.THRH = CM_WTCH_HIGH_T_DOWN; // test two threshold
	
	start_HBridge(&window_HB, SENS2); 
	window_state = DOWN;
	startChannelPIT(CM_PIT_WTCH_TEMPO);
	startChannelPIT(PIT_MODE_W);
	
}

void window_stop()
{
#ifdef PWM
	digitalWrite(PA_1,0);
	digitalWrite(PA_0,0); //Utile ?
#endif
	
	stop_HBridge(&window_HB);
	stop_PITs();
	window_state = STOPPED;
	
}

#endif

