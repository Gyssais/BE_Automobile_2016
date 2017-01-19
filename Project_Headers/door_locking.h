/*
 * door_locking.h
 *
 *  Created on: Jan 15, 2017
 *      Author: Theo
 */

#ifndef DOOR_LOCKING_H_
#define DOOR_LOCKING_H_

#include "define.h"
#include "MC33887.h"
#include "pit.h"
#include "gpio.h"
#include "pin.h"
#include "SBC.h"

#define LOCKING_TIME	70 // ms. motor on time for locking/unlocking


extern MC33887_pinout locking_HB;

int init_locking();
int setup_buttons_l();

void buttons_l_isr();
void pit_locking_isr();

void lock_door();
void unlock_door();

extern char button_lock_irq_mask;

#endif /* DOOR_LOCKING_H_ */
