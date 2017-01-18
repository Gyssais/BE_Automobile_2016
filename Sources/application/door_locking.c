/*
 * door_locking.c
 *
 *  Created on: Jan 15, 2017
 *      Author: Theo
 */

#include "door_locking.h"


MC33887_pinout locking_HB = {IN1_L, IN2_L, D2_L, EN_L, FS_L};

char button_lock_irq_mask; 


int setup_buttons_l()
{
	int result=0;
		
	pinMode(LOCK_BUTTON, INPUT);
	
	result = setup_EIRQ_pin(LOCK_BUTTON, RISING);
	//attachInterrupt_EIRQ1(buttons_l_isr, EIRQ1_PRIORITY); done in the main
		
	return result;
}

int init_locking()
{
	button_lock_irq_mask = (1<<pin_to_EIRQ(LOCK_BUTTON));
	setupChannelPIT(PIT_LOCKING, LOCKING_TIME);
	setupISRChannelPIT(PIT_LOCKING, pit_locking_isr, PIT_LOCKING_PRIOTITY);
	setup_buttons_l();
	init_HBridge(&locking_HB);
}

void buttons_l_isr()
{
	
	if(SIU.ISR.R & button_lock_irq_mask) 
	{
		if(door_state == UNLOCKED) lock_door();
		else if (door_state == LOCKED) unlock_door();
	}
	// flag cleared with the global interrupt in the main
	
}

void pit_locking_isr()
{	
	uint8_t msg = door_state;	
	/* clear interrupt flag */
	PIT.CH[PIT_LOCKING].TFLG.B.TIF = 1;
	stopChannelPIT(PIT_LOCKING);
	stop_HBridge(&locking_HB);
	TransmitMsg(&msg, LENGTH_FRAME, ID_BCM);
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
