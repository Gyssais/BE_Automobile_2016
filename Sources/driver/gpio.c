/*
 * gpio.c
 *
 *  Created on: Jan 10, 2017
 *      Author: Theo
 */

#include "gpio.h"

char eirq_to_pin[16] = {PA_3, PA_6, PA_7, PA_8, PA_14, PC_2, PC_3, PC_5, PC_14, PE_4, PE_10, PE_12, PE_14, PF_15, PG_1, PG_8};

char pin_to_EIRQ(int pin)
{
	int i;
	for(i=0;i<16;i++)
	{
		if(eirq_to_pin[i] == pin) return i;
	}
	
	return WRONG_PIN;
}


void pinMode(int pin, int mode)
{
	if(mode == OUTPUT) SIU.PCR[pin].R = 0x0200;
	else if (mode == INPUT) SIU.PCR[pin].R = 0x0100;
}

int digitalWrite(int pin, int value)
{
	if(SIU.PCR[pin].R == 0x0200) SIU.GPDO[pin].B.PDO = value;
	else return WRONG_CONF;
	
	return 0;
}

int digitalRead(int pin)
{
	if(SIU.PCR[pin].R == 0x0100) return SIU.GPDI[pin].B.PDI;
	else return WRONG_CONF;
}



int setup_EIRQ_pin(int pin, int mode)
{
	char irq = pin_to_EIRQ(pin);
	if(irq == WRONG_PIN) return WRONG_PIN;
	
	if(mode == RISING) SIU.IREER.R |= (1<<irq);
	else if(mode == FALLING) SIU.IFEER.R |= (1<<irq);
	else if(mode == BOTH) {SIU.IREER.R |=(1<<irq); SIU.IFEER.R |=(1<<irq);}
	else return WRONG_MODE;
	
	SIU.IRER.R |=(1<<irq);
	
	return 0;
}


// old setup_EIRQ function
/*
int setup_EIRQ0_pin(int pin, int mode)
{
	switch(pin)
	{
	case PA_3:
		if(mode == RISING) SIU.IREER.R |=0x1;
		else if (mode == FALLING)  SIU.IFEER.R |=0x1;
		else if(mode == BOTH) {SIU.IREER.R |=0x1; SIU.IFEER.R |=0x1;}
		else return WRONG_MODE;
		SIU.IRER.R |=0x1;
		break;
	case PA_6:
		if(mode == RISING) SIU.IREER.R |=0x2;
		else if (mode == FALLING)  SIU.IFEER.R |=0x2;
		else if(mode == BOTH) {SIU.IREER.R |=0x2; SIU.IFEER.R |=0x2;}
		else return WRONG_MODE;
		SIU.IRER.R |=0x2;
		break;
	case PA_7:
		if(mode == RISING) SIU.IREER.R |=0x4;
		else if (mode == FALLING)  SIU.IFEER.R |=0x4;
		else if(mode == BOTH) {SIU.IREER.R |=0x4; SIU.IFEER.R |=0x4;}
		else return WRONG_MODE;
		SIU.IRER.R |=0x4;
		break;
	case PA_8:
		if(mode == RISING) SIU.IREER.R |=0x8;
		else if (mode == FALLING)  SIU.IFEER.R |=0x8;
		else if(mode == BOTH) {SIU.IREER.R |=0x8; SIU.IFEER.R |=0x8;}
		else return WRONG_MODE;
		SIU.IRER.R |=0x8;
		break;
	case PA_14:
		if(mode == RISING) SIU.IREER.R |=0x10;
		else if (mode == FALLING)  SIU.IFEER.R |=0x10;
		else if(mode == BOTH) {SIU.IREER.R |=0x10; SIU.IFEER.R |=0x10;}
		else return WRONG_MODE;
		SIU.IRER.R |=0x10;
		break;
	case PC_2:
		if(mode == RISING) SIU.IREER.R |=0x20;
		else if (mode == FALLING)  SIU.IFEER.R |=0x20;
		else if(mode == BOTH) {SIU.IREER.R |=0x20; SIU.IFEER.R |=0x20;}
		else return WRONG_MODE;
		SIU.IRER.R |=0x20;
		break;
	case PC_3:
		if(mode == RISING) SIU.IREER.R |=0x40;
		else if (mode == FALLING)  SIU.IFEER.R |=0x40;
		else if(mode == BOTH) {SIU.IREER.R |=0x40; SIU.IFEER.R |=0x40;}
		else return WRONG_MODE;
		SIU.IRER.R |=0x40;
		break;
	case PC_5:
		if(mode == RISING) SIU.IREER.R |=0x80;
		else if (mode == FALLING)  SIU.IFEER.R |=0x80;
		else if(mode == BOTH) {SIU.IREER.R |=0x80; SIU.IFEER.R |=0x80;}
		else return WRONG_MODE;
		SIU.IRER.R |=0x80;
		break;
	default:
		return WRONG_PIN;
	}
	return 0;
}


int setup_EIRQ1_pin(int pin, int mode)
{
	switch(pin)
		{
		case PC_14:
			if(mode == RISING) SIU.IREER.R |=0x100;
			else if (mode == FALLING)  SIU.IFEER.R |=0x100;
			else if(mode == BOTH) {SIU.IREER.R |=0x100; SIU.IFEER.R |=0x100;}
			else return WRONG_MODE;
			SIU.IRER.R |=0x1;
			break;
		case PE_4:
			if(mode == RISING) SIU.IREER.R |=0x200;
			else if (mode == FALLING)  SIU.IFEER.R |=0x200;
			else if(mode == BOTH) {SIU.IREER.R |=0x200; SIU.IFEER.R |=0x200;}
			else return WRONG_MODE;
			SIU.IRER.R |=0x2;
			break;
		case PE_10:
			if(mode == RISING) SIU.IREER.R |=0x400;
			else if (mode == FALLING)  SIU.IFEER.R |=0x400;
			else if(mode == BOTH) {SIU.IREER.R |=0x400; SIU.IFEER.R |=0x400;}
			else return WRONG_MODE;
			SIU.IRER.R |=0x4;
			break;
		case PE_12:
			if(mode == RISING) SIU.IREER.R |=0x800;
			else if (mode == FALLING)  SIU.IFEER.R |=0x800;
			else if(mode == BOTH) {SIU.IREER.R |=0x800; SIU.IFEER.R |=0x800;}
			else return WRONG_MODE;
			SIU.IRER.R |=0x8;
			break;
		case PE_14:
			if(mode == RISING) SIU.IREER.R |=0x1000;
			else if (mode == FALLING)  SIU.IFEER.R |=0x1000;
			else if(mode == BOTH) {SIU.IREER.R |=0x1000; SIU.IFEER.R |=0x1000;}
			else return WRONG_MODE;
			SIU.IRER.R |=0x10;
			break;
		case PF_15:
			if(mode == RISING) SIU.IREER.R |=0x2000;
			else if (mode == FALLING)  SIU.IFEER.R |=0x2000;
			else if(mode == BOTH) {SIU.IREER.R |=0x2000; SIU.IFEER.R |=0x2000;}
			else return WRONG_MODE;
			SIU.IRER.R |=0x20;
			break;
		case PG_1:
			if(mode == RISING) SIU.IREER.R |=0x4000;
			else if (mode == FALLING)  SIU.IFEER.R |=0x4000;
			else if(mode == BOTH) {SIU.IREER.R |=0x4000; SIU.IFEER.R |=0x4000;}
			else return WRONG_MODE;
			SIU.IRER.R |=0x40;
			break;
		case PG_8:
			if(mode == RISING) SIU.IREER.R |=0x8000;
			else if (mode == FALLING)  SIU.IFEER.R |=0x8000;
			else if(mode == BOTH) {SIU.IREER.R |=0x8000; SIU.IFEER.R |=0x8000;}
			else return WRONG_MODE;
			SIU.IRER.R |=0x80;
			break;
		default:
			return WRONG_PIN;
		}
		return 0;
}
*/

void attachInterrupt_EIRQ0(INTCInterruptFn isr, unsigned char priority)
{
	INTC_InstallINTCInterruptHandler(isr, IRQ0, priority);
}


void attachInterrupt_EIRQ1(INTCInterruptFn isr, unsigned char priority)
{
	INTC_InstallINTCInterruptHandler(isr, IRQ1, priority);
}
