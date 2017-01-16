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
#include "MC33887.h"
#include "window.h"
#include "SBC.h"

#define CURRENT_SAMPLING_RATE	5 		//ms
#define BUFFER_TIME_LENGTH		3000 	//ms
#define BUFFER_SIZE				(uint32_t)(BUFFER_TIME_LENGTH/CURRENT_SAMPLING_RATE)
#define MOVING_AVR_DEPTH		8 // must be a power of two.
#define AVR_SHIFT				3 // = log2(MOVING_AVR_DEPTH)

#define CM_PIN 					FB_W
#define CM_WTCH_HIGH_T			780
#define CM_WTCH_LOW_T			-1	// the low threshold trigger is not used, set a value < to ADC_MIN to disable
#define CM_WTCH					0
#define CM_PIT_CTU				PIT_ADC
#define CM_PIT_WTCH_TEMPO		PIT_WTCH_TEMPO
#define CM_WTCH_TEMPO			30 // delay in ms before launching current  acquisition.
#define CLOSE_THRH				38

/* functions prototype */

void cm_adc_watchdog_isr();
void cm_adc_eoctu_isr();
int cm_initialize(); // initialize and start the monitoring of the motor's current.
int16_t mving_avr(int16_t data);

void adc_example();
void adc_wtch_isr();
void adc_eoc_isr();
void ctu_trigger_example();

#endif /* CURRENT_MONITORING_H_ */
