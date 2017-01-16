/*
 * current_monitoring.c
 *
 *  Created on: Dec 21, 2016
 *      Author: Theo
 */

#include "current_monitoring.h"

#ifdef DCM
int16_t current_buffer[BUFFER_SIZE] = {0}; // circular buffer
int16_t moving_avr_buffer[MOVING_AVR_DEPTH] = {0};

int16_t moving_avr_counter;
int cm_adc_channel;
uint32_t cm_buffer_counter;



int cm_initialize()
{
	int result;
	cm_buffer_counter = 0;
	
	cm_adc_channel = pinToADCChannel(CM_PIN);
	if(cm_adc_channel <0) return cm_adc_channel;
	
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
	setupChannelPIT(CM_PIT_CTU, CURRENT_SAMPLING_RATE);  // use PIT_3, the only one to be linked to the CTU
	
	setupChannelPIT(CM_PIT_WTCH_TEMPO, CM_WTCH_TEMPO);
	setupISRChannelPIT(CM_PIT_WTCH_TEMPO, pit_wtch_tempo_isr, PIT_WTCH_TEMPO_PRIORITY);
	 // the PIT will be started by the hbridge_tempo_isr() in order to not detect the motor start pick current
	
	
	enableADC();	
	return 0;	
}


void cm_adc_eoctu_isr()
{
	/* clear interrupt flags */
		
	ADC.ISR.B.EOCTU = 1;  // Clear ADC global flag. In this application, the interrupt source can only be the EOC from a CTU triggered conversion.
	ADC.CEOCFR[0].R  = (1<<cm_adc_channel); // Clear the channel flag
	
	/* get the new adc value and append it in the circular buffer */
	if(ADC.CDR[cm_adc_channel].B.VALID ==1) // if data is valid
		{
			current_buffer[cm_buffer_counter] = ADC.CDR[cm_adc_channel].B.CDATA;
			
			cm_buffer_counter++;
			if(cm_buffer_counter >= BUFFER_SIZE) cm_buffer_counter =0; // loop circular buffer
		}

}


/* this isr will be called each time the motor current exceed a configured value */
void cm_adc_watchdog_isr()
{
	int i = 0;
	int closed = 0; 
	
	/* clear interrupt flags */			
	ADC.WTISR.R = (1 << (CM_WTCH+4));
	
	
	/* turn off the motor */
	stop_HBridge(&window_HB);
	
	/* stop all PIT timer */
	stop_PITs();
	
	
	//// analyse the current_buffer data to determine if it's a pinch or a normal closure ////
	
	/* moving average to smooth the data */
	cm_buffer_counter++; // start the filter after the current peak  //TODO doesn't fix the last value getting low
	while(i<BUFFER_SIZE)
	{
		if(cm_buffer_counter >=BUFFER_SIZE) cm_buffer_counter =0;
		
		current_buffer[cm_buffer_counter] = mving_avr(current_buffer[cm_buffer_counter]);	
		cm_buffer_counter++;
		i++;
	}
	
	i =0;
	
	/* derivative */  // TODO to debug
	while(i< BUFFER_SIZE)
	{
		if(cm_buffer_counter >=BUFFER_SIZE) cm_buffer_counter =0;
		
		if(cm_buffer_counter == 0) current_buffer[0] -= current_buffer[BUFFER_SIZE-1];
		else current_buffer[cm_buffer_counter] -= current_buffer[cm_buffer_counter-1];
		
		if((int16_t)current_buffer[cm_buffer_counter] >= CLOSE_THRH) closed =1; // if the the current variation goes that far, we consider the windows is closed
			
		cm_buffer_counter++;
		i++;	               
	}
	
	
	
	if(closed)
	{
		// case windows has closed normally
		LED_on(4);
	}
	else 
	{
		// case pinch has occured
		LED_off(4);
	}
	
	if(window_state == UP) window_position = CLOSED;
	if(window_state == DOWN) window_position =OPEN;
	
	window_state = STOPPED;
}


int16_t mving_avr(int16_t new_data)
{
	static int32_t valMoy =0;
	
	valMoy = valMoy + new_data-(moving_avr_buffer[(moving_avr_counter+1)&(MOVING_AVR_DEPTH-1)]);
	moving_avr_counter = (moving_avr_counter+1)&(MOVING_AVR_DEPTH-1);
	moving_avr_buffer[moving_avr_counter]=new_data;
	
	return (valMoy >> AVR_SHIFT);
}

#endif
