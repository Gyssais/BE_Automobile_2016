/*
 * window.h
 *
 *  Created on: Jan 11, 2017
 *      Author: Theo
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include "define.h"
#include "current_monitoring.h"
#include "MC33887.h"
#include "gpio.h"
#include "pit.h"


extern MC33887_pinout window_HB;

// mode : automatic or manual
#define PIT_MODE_W_TEMPO	200 //ms
#define MODE_W_THRESHOLD	(0xFFFFFFFF - PIT_MODE_W_TEMPO*(F_PIT/1000))


void buttons_w_isr();
void pit_wtch_tempo_isr();

int setup_buttons_w();
void stop_PITs();

int init_window();

void window_up();
void window_down();
void window_stop();


extern MC33887_pinout window_HB;

#endif /* WINDOW_H_ */
