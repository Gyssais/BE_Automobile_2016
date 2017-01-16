/*
 * door_locking.c
 *
 *  Created on: Jan 15, 2017
 *      Author: Theo
 */

#include "door_locking.h"


MC33887_pinout locking_HB = {IN1_L, IN2_L, D2_L, EN_L, FS_L};


int setup_buttons_l()
{
	int result=0;
		
	pinMode(LOCK_BUTTON, INPUT);
	
	result = setup_EIRQ_pin(LOCK_BUTTON, RISING);
	attachInterrupt_EIRQ1(buttons_l_isr, EIRQ1_PRIORITY);
		
	return result;
}

int init_locking()
{

	stopChannelPIT(0);
	setupChannelPIT(PIT_LOCKING, LOCKING_TIME);
	setupISRChannelPIT(PIT_LOCKING, pit_locking_isr, PIT_LOCKING_PRIOTITY);
	setup_buttons_l();
	init_HBridge(&locking_HB);
	startChannelPIT(0);
}

void buttons_l_isr()
{
	/* clear interrupt flag */
	// clear all EIRQ1 isr
	SIU.ISR.R = 0xFF00;
	
	
	if(door_state == UNLOCKED) lock_door();
	else if (door_state == LOCKED) unlock_door();	
}

void pit_locking_isr()
{
	/* clear interrupt flag */
	PIT.CH[PIT_LOCKING].TFLG.B.TIF = 1;
	stopChannelPIT(PIT_LOCKING);
	stop_HBridge(&locking_HB);
}

void lock_door()
{
	startChannelPIT(PIT_LOCKING);
	start_HBridge(&locking_HB, SENS1);
	door_state = LOCKED;
}

void unlock_door()
{
	startChannelPIT(PIT_LOCKING);
	start_HBridge(&locking_HB, SENS2);
	door_state = UNLOCKED;
}
