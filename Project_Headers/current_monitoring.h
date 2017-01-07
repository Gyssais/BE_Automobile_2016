/*
 * current_monitoring.h
 *
 *  Created on: Dec 21, 2016
 *      Author: Theo
 */

#ifndef CURRENT_MONITORING_H_
#define CURRENT_MONITORING_H_


#include "pit.h"
#include "pin.h"
#include "adc.h"
#include "define.h"
#include "MPC5604B.h"
#include "IntcInterrupts.h"
#include "Buttons_management.h"


#define CURRENT_SAMPLING_RATE	10 		//ms
#define BUFFER_TIME_LENGTH		3000 	//ms
#define BUFFER_SIZE				(uint32_t)(BUFFER_TIME_LENGTH/BUFFER_TIME_LENGTH)


#define CM_PIN 					PB_4
#define WTCH_ISR_PRIORITY		10
#define EOCTU_ISR_PRIORITY		5
#define CM_WTCH_HIGH_T			900
#define CM_WTCH_LOW_T			-1	// the low threshold trigger is not used, set a value < to ADC_MIN to disable
#define CM_WTCH					0
#define CM_PIT					3


extern uint16_t current_buffer[BUFFER_SIZE];

/* functions prototype */

void cm_adc_watchdog_isr();
void cm_adc_eoctu_isr();

int cm_initialize(); // initialize and start the monitoring of the motor's current.

#endif /* CURRENT_MONITORING_H_ */
