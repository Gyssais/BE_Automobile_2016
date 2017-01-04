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

/* driver example */
void adc_example();
void ctu_trigger_example();
void adc_watchdog_example();
void adc_eoc_example();
void adc_eoc_isr();

#endif /* CURRENT_MONITORING_H_ */
