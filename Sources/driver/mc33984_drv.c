/**
	mc33984_drv.c
	
	SPI DRIVER FOR MPC5604B_MV27
	
	A driver for transferring 16-bit data 
	arrays with automated interrupts, 
	with a Object Oriented user interface.
	
	Serdar SAHIN - LAAS - 2013
*/


#include "mc33984_drv.h"

struct _SPI_DRV SPI[];

void MC33983_DRV_init(void)
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
	SIU.PCR[5].R = 0x0200;		//PA5:  RST
	//SIU.PCR[68].R = 0x0200;	//LED
	SIU.PCR[6].R = 0x0200;		//PA6: WAKEUP
//EIRQ
	SIU.PCR[7].R = 0x0100;		//PA7: External Interrupt FS, pulled up by the slave chip
//eMIOS
	SIU.PCR[10].R = 0x0400;		//PA10: eMIOS IN0
	SIU.PCR[11].R = 0x0400;
	
//Pin init:
	SIU.GPDO[111].B.PDO = 1;
	SIU.GPDO[5].B.PDO = 1; 		//RST
	SIU.GPDO[6].B.PDO = 1;
	
	SIU.IFEER.B.IFEE2 = 1;		// PA7 will trigger on falling edges
	SIU.IFER.B.IFE2 = 1;
	SIU.IFMC[2].B.MAXCNT = 3;	//Filter for removing glitches, waits for a steady 2-6µs hold of the FAULT
	SIU.IFCPR.B.IFCP = 0xF;
	SIU.IRER.B.EIRE2 = 1; //only EIRQ 2 can issue interrupts
	
	eMIOS0_init();				//eMIOS module init, frozen
	eMIOS0_ch8_MCB_init();		//counter init
	eMIOS0_ch10_OPWMB_init();	//PWM channels init
	eMIOS0_ch11_OPWMB_init();
	eMIOS0_ch2_OPWMB_init();
	eMIOS0_ch3_OPWMB_init();
	
	eMIOS0_ch10_OPWMB_en(0);	//PWM channels stopped
	eMIOS0_ch11_OPWMB_en(0);
	eMIOS0_ch2_OPWMB_en(0);
	eMIOS0_ch3_OPWMB_en(0);
	
	eMIOS0_start(1);			//eMIOS unfrozen
	eMIOS0_ch10_set(0);			//outputs set to LOW no PWM
	eMIOS0_ch11_set(0);
	eMIOS0_ch2_set(0);
	eMIOS0_ch3_set(0);

//Chip init:
	SIU.GPDO[111].B.PDO = 1;		//NORMAL mode
	
	TxBuf = (OUT_HS0<<7)|(REG_SOCHLR<<4)|(0xF);
	SPI[0].write(&TxBuf);  //over current low and high setup for HS0
	TxBuf = (OUT_HS1<<7)|(REG_SOCHLR<<4)|(0xF);
	SPI[0].write(&TxBuf);  //over current low and high setup for HS1
	
	TxBuf = (OUT_HS0<<7)|(REG_CDTOLR<<4)|(0x3);
	SPI[0].write(&TxBuf);  //over current low timing setup for HS0
	TxBuf = (OUT_HS1<<7)|(REG_CDTOLR<<4)|(0x3);
	SPI[0].write(&TxBuf);  //over current low timing setup for HS1
	
	TxBuf = (REG_OSDR<<4)|(0x7);
	SPI[0].write(&TxBuf);  //switching delays for HS0 and HS1 are set to 0ms.
	
	TxBuf = (OUT_HS0<<7)|(REG_DICR<<4)|(0x1);
	SPI[0].write(&TxBuf);  //output logic AND config for HS0
	TxBuf = (OUT_HS1<<7)|(REG_DICR<<4)|(0x1);
	SPI[0].write(&TxBuf);  //output logic AND config for HS1
	
	//disable direct in ?
}

void config_HS(uint8_t HS_no, uint8_t enabled)
{
	uint16_t TxBuf = 0x0000;
	
	TxBuf = (REG_OCR<<4);
	
	if(HS_no == 1)
	{
		MC33984_STATUS.HS1_en=(enabled) ? 1 : 0;
	}
	else if(HS_no == 0)
	{
		MC33984_STATUS.HS0_en=(enabled) ? 1 : 0;
	}
	else if(HS_no == 2)
	{
		MC33984_STATUS.HS0p_en=(enabled) ? 1 : 0;
	}
	else if(HS_no == 3)
	{
		MC33984_STATUS.HS0p_en=(enabled) ? 1 : 0;
	}
	

	if((MC33984_STATUS.HS0_en) || (MC33984_STATUS.HS0p_en))
	{
		TxBuf |= 0x1;
	}
	if((MC33984_STATUS.HS1_en) || (MC33984_STATUS.HS1p_en))
	{
		TxBuf |= 0x4;
	}
	
	if((HS_no == 0) || (HS_no == 1)){
		SPI[0].write(&TxBuf);  //output enables
	}
	else if((HS_no == 2) || (HS_no == 3)){
		SPI[0].write_p(&TxBuf);  //output enables
	}

}

void write_HS(uint8_t HS_no, uint8_t set)
{
	if(HS_no==1)
	{
		eMIOS0_ch11_OPWMB_en(0);
		eMIOS0_ch11_set(set!=0);
	}
	else if (HS_no==0)
	{
		eMIOS0_ch10_OPWMB_en(0);
		eMIOS0_ch10_set(set!=0);
	}
	else if (HS_no==2)
	{
		eMIOS0_ch2_OPWMB_en(0);
		eMIOS0_ch2_set(set!=0);
	}
	else if (HS_no==3)
	{
		eMIOS0_ch3_OPWMB_en(0);
		eMIOS0_ch3_set(set!=0);
	}
}


void conf_PWM_HS(uint8_t HS_no, uint8_t en)
{
	if(HS_no == 1)
	{
		eMIOS0_ch11_OPWMB_en(en!=0);
	}
	else if(HS_no == 0)
	{
		eMIOS0_ch10_OPWMB_en(en!=0);
	}
	else if(HS_no == 2)
	{
		eMIOS0_ch2_OPWMB_en(en!=0);
	}
	else if(HS_no == 3)
	{
		eMIOS0_ch3_OPWMB_en(en!=0);
	}	
}

void dutycycle_PWM_HS(uint8_t HS_no, uint8_t duty_cycle)
{
	if(HS_no == 1)
	{
		eMIOS0_ch11_OPWMB_setDC(duty_cycle);
	}
	else if(HS_no == 0)
	{
		eMIOS0_ch10_OPWMB_setDC(duty_cycle);
	}
	else if(HS_no == 2)
	{
		eMIOS0_ch2_OPWMB_setDC(duty_cycle);
	}
	else if(HS_no == 3)
	{
		eMIOS0_ch3_OPWMB_setDC(duty_cycle);
	}	
}

void read_fault(uint8_t HS_no)
{
	uint16_t TxBuf = 0x0000;
	uint16_t RxBuf;
	TxBuf = (OUT_HS1<<7)|(REG_STATR<<4);
	
	if((HS_no == 1) || (HS_no == 3)){
		TxBuf = (OUT_HS1<<7)|(REG_STATR<<4);
	}
	else if((HS_no == 0) || (HS_no == 2))
	{
		TxBuf = (OUT_HS0<<7)|(REG_STATR<<4);
	}
	
	if ((HS_no == 0) || (HS_no == 1)) { //CS0
		SPI[0].write(&TxBuf);  //output enables
		SPI[0].read(&RxBuf);
	}
	else if((HS_no == 2) || (HS_no == 3)){
		SPI[0].write_p(&TxBuf);  //output enables
		SPI[0].read_p(&RxBuf);	
	}
	
	if(HS_no == 1)
	{
		MC33984_STATUS.HS_1_FAULTR = (uint8_t)RxBuf;
	}
	else if(HS_no == 0)
	{
		MC33984_STATUS.HS_0_FAULTR = (uint8_t)RxBuf;
	}
	else if(HS_no == 2)
	{
		MC33984_STATUS.HS_0p_FAULTR = (uint8_t)RxBuf;
	}
	else if(HS_no == 3)
	{
		MC33984_STATUS.HS_1p_FAULTR = (uint8_t)RxBuf;
	}	
}

/*---------------------------------------------------
					PWM FUNCTIONS
	Fixed 300Hz frequency, adjustable duty cycle
	initialized at 50%. 
	
----------------------------------------------------*/

void eMIOS0_init(void)
{
	EMIOS_0.MCR.B.FRZ = 1; 		//Freeze channels for debugging
	EMIOS_0.MCR.B.GPRE = 63; 	//Divides Peripheral Clock (64MHz) by63+1 ==> 1MHz clock
	EMIOS_0.MCR.B.GPREN = 0;	//Disable the clock	
	EMIOS_0.MCR.B.GTBE = 0; 	//Disable : Global Time Base Enable (8-bit module counter)
}

void eMIOS0_start(uint8_t start)
{
	EMIOS_0.MCR.B.GPREN = (start!=0) ? 1 : 0;	//Enable the clock	
	EMIOS_0.MCR.B.GTBE 	= (start!=0) ? 1 : 0; 	// Global Time Base Enable (8-bit module counter)
}


//*************************************PWM CH2*********************************************//
void eMIOS0_ch2_OPWMB_init(void)
{
	EMIOS_0.CH[2].CCR.B.MODE = 0x0;	//Ensure the GPIO mode for internal reset
	EMIOS_0.CH[2].CCR.B.UCPEN = 0; 	//Disable prescaler
	EMIOS_0.CH[2].CCR.B.FREN = 1;		//Freeze for debugging
	EMIOS_0.CH[2].CCR.B.BSL = 0x1;
	EMIOS_0.CH[2].CCR.B.EDPOL = 1;
	EMIOS_0.CH[2].CCR.B.FEN = 1;
	EMIOS_0.CH[2].CCR.B.DMA = 1;
	EMIOS_0.CH[2].CADR.B.CADR = 1; 	//Leading edge
	EMIOS_0.CH[2].CBDR.B.CBDR = 1650; 	//Trailing edge
	EMIOS_0.CH[2].CCR.B.MODE = 0x60;	//OPWMB
	EMIOS_0.CH[2].CCR.B.UCPEN = 1; 	//Enable prescaler	
}

void eMIOS0_ch2_OPWMB_setDC(uint8_t duty_cycle)
{
	if(duty_cycle < 1) duty_cycle=1;
	if(duty_cycle > 100) duty_cycle=100;
	
	EMIOS_0.CH[2].CBDR.B.CBDR = duty_cycle*3333/100; //set dutycycle
}

void eMIOS0_ch2_OPWMB_en(uint8_t enable)
{
	if(enable)
	{
		EMIOS_0.CH[2].CCR.B.MODE = 0x60;
		EMIOS_0.CH[2].CCR.B.EDPOL = 1;
	}
	else
	{
		EMIOS_0.CH[2].CCR.B.MODE = 0x01;
	}
}

void eMIOS0_ch2_set(uint8_t output)
{
	EMIOS_0.CH[2].CCR.B.MODE = 0x1;
	EMIOS_0.CH[2].CCR.B.EDPOL = (output!=0) ? 1 : 0;
}


//*************************************PWM CH3*********************************************//
void eMIOS0_ch3_OPWMB_init(void)
{
	EMIOS_0.CH[3].CCR.B.MODE = 0x0;	//Ensure the GPIO mode for internal reset
	EMIOS_0.CH[3].CCR.B.UCPEN = 0; 	//Disable prescaler
	EMIOS_0.CH[3].CCR.B.FREN = 1;		//Freeze for debugging
	EMIOS_0.CH[3].CCR.B.BSL = 0x1;
	EMIOS_0.CH[3].CCR.B.EDPOL = 1;
	EMIOS_0.CH[3].CCR.B.FEN = 1;
	EMIOS_0.CH[3].CCR.B.DMA = 1;
	EMIOS_0.CH[3].CADR.B.CADR = 1; 	//Leading edge
	EMIOS_0.CH[3].CBDR.B.CBDR = 1650; 	//Trailing edge
	EMIOS_0.CH[3].CCR.B.MODE = 0x60;	//OPWMB
	EMIOS_0.CH[3].CCR.B.UCPEN = 1; 	//Enable prescaler	
}

void eMIOS0_ch3_OPWMB_setDC(uint8_t duty_cycle)
{
	if(duty_cycle < 1) duty_cycle=1;
	if(duty_cycle > 100) duty_cycle=100;
	
	EMIOS_0.CH[3].CBDR.B.CBDR = duty_cycle*3333/100; //set dutycycle
}

void eMIOS0_ch3_OPWMB_en(uint8_t enable)
{
	if(enable)
	{
		EMIOS_0.CH[3].CCR.B.MODE = 0x60;
		EMIOS_0.CH[3].CCR.B.EDPOL = 1;
	}
	else
	{
		EMIOS_0.CH[3].CCR.B.MODE = 0x01;
	}
}

void eMIOS0_ch3_set(uint8_t output)
{
	EMIOS_0.CH[3].CCR.B.MODE = 0x1;
	EMIOS_0.CH[3].CCR.B.EDPOL = (output!=0) ? 1 : 0;
}

//*************************************INTERNAL COUNTER CH8*********************************************//
void eMIOS0_ch8_MCB_init(void)
{
	EMIOS_0.CH[8].CCR.B.MODE = 0x0;	//Ensure the GPIO mode for internal reset
	EMIOS_0.CH[8].CCR.B.UCPEN = 0; 	//Disable prescaler
	EMIOS_0.CH[8].CCR.B.FREN = 1;		//Freeze for debugging
	EMIOS_0.CH[8].CCR.B.BSL = 0x3;
	EMIOS_0.CH[8].CADR.B.CADR = 3333; 	//Counter from 1 to 3333: roll over 300.03Hz.
	EMIOS_0.CH[8].CCNTR.B.CCNTR = 1; 	//Initial value of the internal counter.
	EMIOS_0.CH[8].CCR.B.MODE = 0x50;	//MCB, Up mode, clocked by the prescaled clock
	EMIOS_0.CH[8].CCR.B.UCPRE = 0; 	//Divide eMIOS presclaed clock by 1 : 1MHZ clock
	EMIOS_0.CH[8].CCR.B.UCPEN = 1; 	//Disable prescaler	
}


//*************************************PWM CH10*********************************************//
void eMIOS0_ch10_OPWMB_init(void)
{
	EMIOS_0.CH[10].CCR.B.MODE = 0x0;	//Ensure the GPIO mode for internal reset
	EMIOS_0.CH[10].CCR.B.UCPEN = 0; 	//Disable prescaler
	EMIOS_0.CH[10].CCR.B.FREN = 1;		//Freeze for debugging
	EMIOS_0.CH[10].CCR.B.BSL = 0x1;
	EMIOS_0.CH[10].CCR.B.EDPOL = 1;
	EMIOS_0.CH[10].CCR.B.FEN = 1;
	EMIOS_0.CH[10].CCR.B.DMA = 1;
	EMIOS_0.CH[10].CADR.B.CADR = 1; 	//Leading edge
	EMIOS_0.CH[10].CBDR.B.CBDR = 1650; 	//Trailing edge
	EMIOS_0.CH[10].CCR.B.MODE = 0x60;	//OPWMB
	EMIOS_0.CH[10].CCR.B.UCPEN = 1; 	//Enable prescaler	
}

void eMIOS0_ch10_OPWMB_setDC(uint8_t duty_cycle)
{
	if(duty_cycle < 1) duty_cycle=1;
	if(duty_cycle > 100) duty_cycle=100;
	
	EMIOS_0.CH[10].CBDR.B.CBDR = duty_cycle*3333/100; //set dutycycle
}

void eMIOS0_ch10_OPWMB_en(uint8_t enable)
{
	if(enable)
	{
		EMIOS_0.CH[10].CCR.B.MODE = 0x60;
		EMIOS_0.CH[10].CCR.B.EDPOL = 1;
	}
	else
	{
		EMIOS_0.CH[10].CCR.B.MODE = 0x01;
	}
}

void eMIOS0_ch10_set(uint8_t output)
{
	EMIOS_0.CH[10].CCR.B.MODE = 0x1;
	EMIOS_0.CH[10].CCR.B.EDPOL = (output!=0) ? 1 : 0;
}

//*************************************PWM CH11*********************************************//
void eMIOS0_ch11_OPWMB_init(void)
{
	EMIOS_0.CH[11].CCR.B.MODE = 0x0;	//Ensure the GPIO mode for internal reset
	EMIOS_0.CH[11].CCR.B.UCPEN = 0; 	//Disable prescaler
	EMIOS_0.CH[11].CCR.B.FREN = 1;		//Freeze for debugging
	EMIOS_0.CH[11].CCR.B.BSL = 0x1;
	EMIOS_0.CH[11].CCR.B.EDPOL = 1;
	EMIOS_0.CH[11].CCR.B.FEN = 1;
	EMIOS_0.CH[11].CCR.B.DMA = 1;
	EMIOS_0.CH[11].CADR.B.CADR = 1; 	//Leading edge
	EMIOS_0.CH[11].CBDR.B.CBDR = 1650; 	//Trailing edge
	EMIOS_0.CH[11].CCR.B.MODE = 0x60;	//OPWMB
	EMIOS_0.CH[11].CCR.B.UCPEN = 1; 	//Enable prescaler	
}

void eMIOS0_ch11_OPWMB_setDC(uint8_t duty_cycle)
{
	if(duty_cycle < 1) duty_cycle=1;
	if(duty_cycle > 100) duty_cycle=100;
	
	EMIOS_0.CH[11].CBDR.B.CBDR = duty_cycle*33; //set dutycycle
}

void eMIOS0_ch11_OPWMB_en(uint8_t enable)
{
	if(enable)
	{
		EMIOS_0.CH[11].CCR.B.MODE = 0x60;
		EMIOS_0.CH[11].CCR.B.EDPOL = 1;

	}
	else
	{
		EMIOS_0.CH[11].CCR.B.MODE = 0x1;
	}
}

void eMIOS0_ch11_set(uint8_t output)
{
	EMIOS_0.CH[11].CCR.B.MODE = 0x1;
	EMIOS_0.CH[11].CCR.B.EDPOL = (output!=0) ? 1 : 0;
}

