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
#include "gpio.h"

/* driver example */
void adc_example();
void ctu_trigger_example();
void adc_watchdog_example();
void adc_eoc_example();
void adc_eoc_isr();
void test_filter();
void gpio_isr_example();
void h_bridge_test();

/* functions for a simple driver for MC33887 H-bridge. */

#define IN1_HB		PC_4
#define IN2_HB		PC_5
#define EN_HB		PC_6
#define D2_HB		PC_7

#define DOWN_HB		0
#define UP_HB		1

void init_Hbridge();
void stop_Hbridge();
void start_Hbridge(int sens);

#endif /* DRIVER_EXAMPLE_H_ */
