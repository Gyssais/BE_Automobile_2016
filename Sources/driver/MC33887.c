/*
 * MC33887.c
 *
 *  Created on: Jan 11, 2017
 *      Author: Theo
 */


#include "driver/MC33887.h"


void init_HBridge(const MC33887_pinout * pinout)
{

	SIU.PCR[pinout->IN1].R = 0x0200;  // out
	SIU.PCR[pinout->IN2].R = 0x0200;  // out
	SIU.PCR[pinout->EN].R = 0x0200;	 // out
	SIU.PCR[pinout->D2].R = 0x0200;   // out
	SIU.PCR[pinout->FS].R = 0x0100;   // in

	
	SIU.GPDO[pinout->IN1].B.PDO = 0;
	SIU.GPDO[pinout->IN2].B.PDO = 0;
	SIU.GPDO[pinout->EN].B.PDO = 0;
	SIU.GPDO[pinout->D2].B.PDO = 0;
}


void stop_HBridge(const MC33887_pinout * pinout)
{
	SIU.GPDO[pinout->IN1].B.PDO = 0;
	SIU.GPDO[pinout->IN2].B.PDO = 0;
	SIU.GPDO[pinout->D2].B.PDO = 0;
	
}

void start_HBridge(const MC33887_pinout * pinout, int sens)
{
	SIU.GPDO[pinout->EN].B.PDO = 1;
	SIU.GPDO[pinout->D2].B.PDO = 1;	
	
	if(sens == SENS1)
	{
		SIU.GPDO[pinout->IN1].B.PDO = 0;
		SIU.GPDO[pinout->IN2].B.PDO = 1;
	}
	else if(sens == SENS2)
	{
		SIU.GPDO[pinout->IN1].B.PDO = 1;
		SIU.GPDO[pinout->IN2].B.PDO = 0;
	}
}
