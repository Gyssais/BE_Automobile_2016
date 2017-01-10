/*
 * driver_example.h
 *
 *  Created on: Jan 7, 2017
 *      Author: Theo
 */

#ifndef DRIVER_EXAMPLE_H_
#define DRIVER_EXAMPLE_H_


#include "pit.h"
#include "pin.h"
#include "adc.h"
#include "define.h"
#include "MPC5604B.h"
#include "IntcInterrupts.h"
#include "Buttons_management.h"


/* driver example */
void adc_example();
void ctu_trigger_example();
void adc_watchdog_example();
void adc_eoc_example();
void adc_eoc_isr();


#endif /* DRIVER_EXAMPLE_H_ */
