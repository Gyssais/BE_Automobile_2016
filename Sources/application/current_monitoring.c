/*
 * current_monitoring.c
 *
 *  Created on: Dec 21, 2016
 *      Author: Theo
 */

#include "current_monitoring.h"

int cm_initialize()
{
	int result;
	
	/* pin initialization */
	result = setupADC();
	result = setupPin_ADC(CM_PIN);
	result = setupPin_ADC_Interrupt(CM_PIN, EOCTU_MASK);	// enable EOC_CTU interrupt from pin PB_4 
	if(result !=0) return result;
	
	/* EOC global interrupt enabled */
	attachInterrupt_ADC_EOC(cm_adc_eoctu_isr, EOCTU_ISR_PRIORITY);	// set the ISR to be called for adc EOCTU interrupt
	
	/* watchdog*/
	result = setupAnalogWatchdog(CM_PIN, CM_WTCH_HIGH_T, CM_WTCH_LOW_T, CM_WTCH); 
	if(result !=0) return result;
	
	attachInterrupt_ADC_WTCH(cm_adc_watchdog_isr, WTCH_ISR_PRIORITY);
	startAnalogWatchdog(CM_WTCH);
	
	/* PIT & CTU*/
	result = pinToADCChannel(CM_PIN);
	if(result < 0) return result;
	
	setupChannel_CTU_trigger(result); 	// link the PIT to the ADC channel corresponding to CM_PIN through the CTU and enable CTU
	setupChannelPIT(CM_PIT, CURRENT_SAMPLING_RATE);  // use PIT_3, the only one to be linked to the CTU
	startChannelPIT(CM_PIT);
	
	
	enableADC();	
	return 0;	
}
