/*
 * Driver_phare.h
 *
 *  Created on: Jan 15, 2015
 *      Author: Marine
 */

#ifndef DRIVER_PHARE_H_
#define DRIVER_PHARE_H_

#include "spi_drv.h"
#include "Driver_phare.h"
//#include <string.h>


//SOCH (D3)
#define overCurrentH100A 0x0
#define overCurrentH75A 0x8

//SOCL (D2..D0)
#define overCurrentL20A 0x2
#define overCurrentL22_5A 0x1
#define overCurrentL17_5A 0x3
#define overCurrentL15A 0x4

//CDTOLR (D3..D0)
#define overCurrentLTime155ms 0x0
#define overCurrentLTime10ms 0x01
#define overCurrentLTime1_2ms 0x2
#define overCurrentLTime150us 0x3
#define openLoadEnable 0x0
#define openLoadDisable 0x8
#define overCurrentLDetectFailureEnable 0x0
#define overCurrentLDetectFailureDisable 0x4

//UOVR
#define overVoltageEnable 0x0
#define overVoltageDisable 0x01
#define underVoltageEnable 0x0
#define underVoltageDisable 0x02

/**********************Commandes des phares******************/
void Position_ON();
void Position_OFF();

void Croisement_ON();
void Croisement_OFF();

void Direction_droit_ON();
void Direction_droit_OFF();

void Direction_gauche_ON();
void Direction_gauche_OFF();


void Route_ON();
void Route_OFF();

void Clignotant_droit_ON();
void Clignotant_droit_OFF();

void Clignotant_gauche_ON();
void Clignotant_gauche_OFF();



void MC33983_DRV_init_0(void);
/*---------------------------------------------------
					PWM FUNCTIONS
	Used for direct control inputs of the high 
	side switches.
----------------------------------------------------*/

//Initialization of eMIOS
void eMIOS0_init(void);
//A function for starting its clock (used by device init)
void eMIOS0_start(uint8_t start);

//Init of the counter bus.
void eMIOS0_ch8_MCB_init(void);

//Channel 10 control functions as PWM or as a steady output
void eMIOS0_ch10_OPWMB_init(void);
void eMIOS0_ch10_OPWMB_setDC(uint8_t duty_cycle);
void eMIOS0_ch10_OPWMB_en(uint8_t enable);
void eMIOS0_ch10_set(uint8_t output);

//Channel 11 control functions as PWM or as a steady output
void eMIOS0_ch11_OPWMB_init(void);
void eMIOS0_ch11_OPWMB_setDC(uint8_t duty_cycle);
void eMIOS0_ch11_OPWMB_en(uint8_t enable);
void eMIOS0_ch11_set(uint8_t output);

#endif /* DRIVER_PHARE_H_ */
