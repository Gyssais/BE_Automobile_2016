/*
 * current_monitoring.c
 *
 *  Created on: Dec 21, 2016
 *      Author: Theo
 */

#include "current_monitoring.h"

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


// configure PIT, CTU and ADC to trigger a conversion on AD_PIN every 100ms
void ctu_trigger_example()
{
	int result;
		
	result = setupADC();
	result = setupPin_ADC(AD_PIN);
	
	setupChannel_CTU_trigger(0); 	// link the PIT to the ADC channel 0 (PB4) through the CTU
	setupChannelPIT(3, 100);  // use PIT_3, the only one to be linked to the CTU. period =100ms.
	startChannelPIT(3);
	
	enableADC();
	
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
