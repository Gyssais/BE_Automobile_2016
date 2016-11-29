/*
 * Driver_phare.c
 *
 *  Created on: Jan 15, 2015
 *      Author: Marine
 */

#include "Driver_phare.h"
#include "mc33984_drv.h"

struct _MC33984_STATUS MC33984_STATUS;

void MC33983_DRV_init_0(void)
{
	uint16_t TxBuf = 0x0000;

//Flag setup
	MC33984_STATUS.HS_0_FAULTR=0;
	MC33984_STATUS.HS_1_FAULTR=0;
	MC33984_STATUS.HS0_en=0;
	MC33984_STATUS.HS1_en=0;

//Pin setup
/*
 *	MCU				-			KIT33984
 *	A15	DSPI		CS				2				
 *	A10	eMIOS		IN0				3
 *	A14	DSPI		SCLK			4
 *	A11	eMIOS		IN1				5
 *	A13	DSPI		Slave IN		6
 *	A5	SIU			RST				7
 *	A6	SIU			WAKE			8
 *	A12	DSPI		Slave OUT		12				
 *					GND				18-25
 *	A7	EIRQ		FS				FSB
*/

//SPI
	initialise_SPI_DRIVER(); 
	SPI[0].init(SPI_BAUD_1000000, SPI_DELAY_MC33984); /* PA12-PA15	*/
//SIU
	SIU.PCR[111].R = 0x0200;	//PA5:  RST
	SIU.PCR[6].R = 0x0200;		//PA6: WAKEUP
	SIU.PCR[5].R = 0x0200;		//PA6: WAKEUP
//EIRQ
	SIU.PCR[7].R = 0x0100;		//PA7: External Interrupt FS, pulled up by the slave chip
//eMIOS
	SIU.PCR[10].R = 0x0400;		//PA10: eMIOS IN0
	SIU.PCR[11].R = 0x0400;
	
//Pin init:
	SIU.GPDO[111].B.PDO = 1;
	SIU.GPDO[6].B.PDO = 1;
	SIU.GPDO[5].B.PDO = 1;
	
	SIU.IFEER.B.IFEE2 = 1;// PA7 will trigger on falling edges
	SIU.IFER.B.IFE2 = 1;
	SIU.IFMC[2].B.MAXCNT = 3;	//Filter for removing glitches, waits for a steady 2-6µs hold of the FAULT
	SIU.IFCPR.B.IFCP = 0xF;
	SIU.IRER.B.EIRE2 = 1; //only EIRQ 2 can issue interrupts
	
	eMIOS0_init();				//eMIOS module init, frozen
	eMIOS0_ch8_MCB_init();		//counter init
	eMIOS0_ch10_OPWMB_init();	//PWM channels init
	eMIOS0_ch11_OPWMB_init();
	eMIOS0_ch10_OPWMB_en(0);	//PWM channels stopped
	eMIOS0_ch11_OPWMB_en(0);
	eMIOS0_start(1);			//eMIOS unfrozen
	eMIOS0_ch10_set(0);			//outputs set to LOW no PWM
	eMIOS0_ch11_set(0);

//Chip init:
	SIU.GPDO[111].B.PDO = 1;		//NORMAL mode
	
	TxBuf = (OUT_HS0<<7)|(REG_SOCHLR<<4)|(overCurrentH75A)|(overCurrentL20A);
	SPI[0].write(&TxBuf);  //over current low and high setup for HS0 overcurrent low 20A (0x2) overcurrent high 75A (0x8)
	TxBuf = (OUT_HS1<<7)|(REG_SOCHLR<<4)|(overCurrentH75A)|(overCurrentL20A);
	SPI[0].write(&TxBuf);  //over current low and high setup for HS1 overcurrent low 20A (0x2) overcurrent high 75A (0x8)
	
	TxBuf = (OUT_HS0<<7)|(REG_CDTOLR<<4)|(overCurrentLTime10ms)|(openLoadEnable)|(overCurrentLDetectFailureEnable);
	SPI[0].write(&TxBuf);  //over current low timing setup for HS0
	TxBuf = (OUT_HS1<<7)|(REG_CDTOLR<<4)|(overCurrentLTime10ms)|(openLoadEnable)|(overCurrentLDetectFailureEnable);
	SPI[0].write(&TxBuf);  //over current low timing setup for HS1
	//for overcurrent high device latch up within 20µs
	
	TxBuf = (REG_OSDR<<4)|(0x7);
	SPI[0].write(&TxBuf);  //switching delays for HS0 and HS1 are set to 0ms.
	
	TxBuf = (OUT_HS0<<7)|(REG_DICR<<4)|(0x1);
	SPI[0].write(&TxBuf);  //output logic AND config for HS0
	TxBuf = (OUT_HS1<<7)|(REG_DICR<<4)|(0x1);
	SPI[0].write(&TxBuf);  //output logic AND config for HS1
	
	//TxBuf = (OUT_HS0<<7)|(REG_UOVR<<4)|(overVoltageEnable);
	SPI[0].write(&TxBuf);  //output logic AND config for HS0
	TxBuf = (OUT_HS1<<7)|(REG_DICR<<4)|(underVoltageEnable);
	SPI[0].write(&TxBuf);  //output logic AND config for HS1
	
	TxBuf = (OUT_HS0<<7)|(REG_OCR<<4)|(0xA);
	SPI[0].write(&TxBuf);  //csns for HS0
	TxBuf = (OUT_HS1<<7)|(REG_OCR<<4)|(0xA);
	SPI[0].write(&TxBuf);  //csns for HS1

	
}

void Route_ON(void)
{
	write_HS(0,1);
	config_HS(0, 1);
	conf_PWM_HS(0,1);
	dutycycle_PWM_HS(0,50);
}

 

void Route_OFF(void)
{
	write_HS(0,0);
	config_HS(0, 0);
}

 

void Croisement_ON(void)
{	SIU.PCR[69].R = 0x0200;
	write_HS(1,1);
	config_HS(1, 1);
	conf_PWM_HS(1,1);
	dutycycle_PWM_HS(1,50);
}


void Croisement_OFF(void)
{	write_HS(1,0);
	config_HS(1, 0);
}

void Position_ON(){
	;
}
void Position_OFF(){
;
}


void Direction_droit_ON(){
	;
}
void Direction_droit_OFF(){
	;
}
void Direction_gauche_ON(){
	;
}
void Direction_gauche_OFF(){
	;
}


void Clignotant_droit_ON(){
	;
}
void Clignotant_droit_OFF(){
	;
}

void Clignotant_gauche_ON(){
	;
}
void Clignotant_gauche_OFF(){
	;
}
