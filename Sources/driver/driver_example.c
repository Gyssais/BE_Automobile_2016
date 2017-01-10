/*
 * driver_example.c
 *
 *  Created on: Jan 7, 2017
 *      Author: Theo
 */

#include "driver_example.h"
#include "current_monitoring.h"
#include "gpio.h"



void eirq0_isr()
{
	
	if((SIU.ISR.R & 0x1))  // if isr raised by PA_3
	{
		stop_Hbridge(); 
	}
	
	if((SIU.ISR.R & 0x2)) //if isr raised by PA_6
	{
		if(window_state == STOPPED) 
		{
			start_Hbridge(DOWN_HB); 
			window_state = DOWN;
			startChannelPIT(CM_PIT_TEMPO);
		}
		else if(window_state == DOWN) {stop_Hbridge(); window_state = STOPPED;}
		
	}
	
	if((SIU.ISR.R & 0x4)) //if isr raised by PA_7
	{
		if(window_state == STOPPED) 
		{ 
		start_Hbridge(UP_HB); 
		window_state = UP;
		startChannelPIT(CM_PIT_TEMPO);
		}
		else if(window_state == UP) {stop_Hbridge(); window_state = STOPPED;}
	}
	
	/* clear interrupt flag */
		// clear all isr
	SIU.ISR.R = 0xFFFF;
	
	/* toggle LED_1 */
	if(SIU.GPDO[PE_4].R == 1) SIU.GPDO[PE_4].R = 0;
	else SIU.GPDO[PE_4].R = 1;
	
}

void gpio_isr_example()
{	int result =0;
	pinMode(PA_3, INPUT);
	init_LED();
	result = setup_EIRQ0_pin(PA_3, RISING);
	attachInterrupt_EIRQ0(eirq0_isr, 4);
}


void h_bridge_test()
{
	int result =0;
	
	init_LED();
	
	LED_off(4); // debug
	
	init_Hbridge();
	
	pinMode(PA_3, INPUT);
	pinMode(PA_6, INPUT);
	pinMode(PA_7, INPUT);
	
	result = setup_EIRQ0_pin(PA_3, RISING);
	result = setup_EIRQ0_pin(PA_6, RISING);
	result = setup_EIRQ0_pin(PA_7, RISING);
	
	
	
	attachInterrupt_EIRQ0(eirq0_isr, 4);
}



/* functions for a simple driver for MC33887 H-bridge. */


void init_Hbridge()
{
	
	
	SIU.PCR[IN1_HB].R = 0x0200;  // out
	SIU.PCR[IN2_HB].R = 0x0200;  // out
	SIU.PCR[EN_HB].R = 0x0200;	 // out
	SIU.PCR[D2_HB].R = 0x0200;   // out
	
	
	SIU.GPDO[IN1_HB].B.PDO = 0;
	SIU.GPDO[IN2_HB].B.PDO = 0;
	SIU.GPDO[EN_HB].B.PDO = 0;
	SIU.GPDO[D2_HB].B.PDO = 0;
}


void stop_Hbridge()
{
	SIU.GPDO[IN1_HB].B.PDO = 0;
	SIU.GPDO[IN2_HB].B.PDO = 0;
	SIU.GPDO[D2_HB].B.PDO = 0;
	
	stopChannelPIT(CM_PIT);
}

void start_Hbridge(int sens)
{
	SIU.GPDO[EN_HB].B.PDO = 1;
	SIU.GPDO[D2_HB].B.PDO = 1;	
	
	if(sens == UP_HB)
	{
		SIU.GPDO[IN1_HB].B.PDO = 0;
		SIU.GPDO[IN2_HB].B.PDO = 1;
	}
	else if(sens == DOWN_HB)
	{
		SIU.GPDO[IN1_HB].B.PDO = 1;
		SIU.GPDO[IN2_HB].B.PDO = 0;
	}
}





void adc_example()
{
	int result;
	
	result = setupADC();
	result =  setupPin_ADC(AD_PIN);
	enableADC();
	
	while(1)
	{
		result = analogRead(AD_PIN);
	}
}


void adc_wtch_isr()
{
	unsigned long flag = ADC.WTISR.R;
}

void adc_eoc_isr()
{
	/* clear interrupt flags */
	
	ADC.ISR.B.EOCTU = 1;  // in this example, the interrupt source can only be the EOC from a CTU triggered conversion.
	ADC.CEOCFR[0].B.EOC_CH0 =1; // here, we use only the channel 0.
	
	/* toggle LED_1 */
	if(SIU.GPDO[PE_4].R == 1) SIU.GPDO[PE_4].R = 0;
	else SIU.GPDO[PE_4].R = 1;
	
}
// configure PIT, CTU and ADC to trigger a conversion on AD_PIN every 100ms
void ctu_trigger_example()
{
	int result;
	init_LED();
	
	result = setupADC();
	result = setupPin_ADC(AD_PIN);
	
	setupPin_ADC_Interrupt(PB_4, EOCTU_MASK);	// enable EOC_CTU interrupt from pin PB_4 
	attachInterrupt_ADC_EOC(adc_eoc_isr, 8);	// set the ISR to be called for adc EOC interrupt
	
	enableADC();
	
	setupChannel_CTU_trigger(0); 	// link the PIT to the ADC channel 0 (PB4) through the CTU and enable CTU
	
	setupChannelPIT(3, 500);  // use PIT_3, the only one to be linked to the CTU. period =100ms.
	
	startChannelPIT(3);	
}


void adc_eoc_example()
{
	int result;
	init_LED();
	
	result = setupADC();
	result = setupPin_ADC(AD_PIN);
	
	setupPin_ADC_Interrupt(PB_4, EOC_MASK);	// enable EOC_CTU interrupt from pin PB_4 
	attachInterrupt_ADC_EOC(adc_eoc_isr, 8);	// set the ISR to be called for adc EOC interrupt
	enableADC();
	
	startConversion();
	
	
}

void adc_watchdog_example()
{
	int result;
		
	result = setupADC();
	result =  setupPin_ADC(AD_PIN);
	
	result = setupAnalogWatchdog(AD_PIN, 700, 100, 0); // high_treshold =700, low = 100, first watchdog (0)
	attachInterrupt_ADC_WTCH(adc_wtch_isr, 7);
	startAnalogWatchdog(0);
	
	enableADC();
	
	
	while(1)
		{
			result = analogRead(AD_PIN);
		}
}


uint16_t data[130]= {
822,
933,
781,
648,
556,
331,
362,
296,
241,
230,
377,
408,
388,
390,
450,
385,
488,
509,
460,
449,
475,
514,
512,
527,
511,
533,
502,
491,
462,
490,
479,
526,
559,
558,
528,
616,
619,
571,
625,
661,
652,
579,
586,
632,
586,
573,
611,
543,
588,
527,
552,
562,
514,
562,
575,
512,
519,
561,
547,
527,
600,
566,
526,
565,
570,
499,
506,
546,
560,
507,
513,
567,
537,
514,
560,
549,
541,
598,
574,
529,
581,
592,
529,
596,
576,
577,
600,
556,
615,
555,
631,
556,
630,
553,
631,
598,
592,
638,
601,
615,
639,
581,
614,
645,
591,
601,
667,
632,
593,
621,
663,
627,
590,
649,
627,
567,
643,
608,
580,
632,
607,
608,
655,
629,
585,
617,
622,
589,
431,
429};

uint16_t moving_avr_buffer_test[MOVING_AVR_DEPTH] = {0};

uint16_t mving_avr_test(uint16_t new_data)
{
	static int32_t valMoy =0;
	static int32_t moving_avr_counter=0;
	
	valMoy = valMoy + new_data-(moving_avr_buffer_test[(moving_avr_counter+1)&(MOVING_AVR_DEPTH-1)]);
	moving_avr_counter = (moving_avr_counter+1)&(MOVING_AVR_DEPTH-1);
	moving_avr_buffer_test[moving_avr_counter]=new_data;
	
	return (valMoy >> AVR_SHIFT);
}

void test_filter()
{
	int i=0;
	
	for(i=0;i<130;i++)
	{
		data[i] = mving_avr_test(data[i]);
	}
}


