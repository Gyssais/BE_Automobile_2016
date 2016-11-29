/**
 * \file mc33984_drv.h
 * \brief MC33984 DRIVER FOR MPC5604B_MV27 and SPI_DRV
 * \author Serdar SAHIN
 * \version 1.0
 * \date august 2013
 	
 *	A driver for using SPI Driver in an
 *  adapted way for commanding MC339834
 *  smart chip.
	
	Serdar SAHIN - LAAS - 2013
*/


/*! \mainpage MC33984 Driver for MPC5604B_MV27
*
* \section intro_sec About
*
*	This driver provides a simple user interface for using
*	the MC33984 smart MOS chip (high side). It uses SPI_DRV for using
*	DSPI module and it also uses eMIOS for generating PWM signals.      
*	   
*	This is meant to be an example for exploiting SPI_DRV for driving SPI interfaced
*	smart chips. DON'T FORGET TO SET THE SPI FRAMESIZE TO 8 IN SPI_DRV.H!!!!!!!
*                                                               
*	In this example, it is assumed that an output HS is commanded trough an input
*	pin while the IN_SPI bit of OCR register is set. (Therefore the output is a logic
*	AND of INx_SPI bit of OCR register and INx pin. 
*
*	MC33984 evaluation board is used. The watchdog is grounded. Connections
*
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
*
*	A7	EIRQ		FS				FSB		Not connected to an output pin on the kit, might need
*											a jumper wire.
*/

#ifndef _MC33984_DRV_H
#define _MC33984_DRV_H


#include "MPC5604B.h"
#include "spi_drv.h"
//#include <string.h>

/**
 * \def MC33984_ERROR();
 *
 * An inifite loop that is entered on a detectable error on the driver,
 * usually related to missing or inappropriate user parameters.
*/
#define MC33984_ERROR(); 	  while(1){}



/**
 * \defgroup group1	High Side Switches
 * \{
 * 
 * \brief MC33984 Output MOSFETs
 *
 * This bit is used as the MSB of SPI
 * write messages.
 *
*/
#define OUT_HS0 	0x0
#define OUT_HS1 	0x1
/**
 * \}
*/


/**
 * \defgroup group2	Internal Registers
 * \{
 * 
 * \brief MC33984 Registers
 *
 * These registers are accessed trought SPI.
 * Some of them are double, one for HS0, other
 * HS1. The MSB sets this. (During call) 
 *
*/
#define REG_STATR 	0x0
#define REG_OCR 	0x1
#define REG_SOCHLR 	0x2
#define REG_CDTOLR 	0x3
#define REG_DICR 	0x4
#define REG_OSDR 	0x5
#define REG_WDR 	0xD
#define REG_NAR 	0x6
#define REG_UOVR 	0xE
/**
 * \}
*/



/**
 * 	\defgroup group3 SPI Attributes
 *	\brief Delay configuration for the driver.
 * 	\{
 *
 *	MC33984 requires:
 *		CS to SCK :  >167ns
 *		SCK to CS :  > 167ns => 11 clocl cycles
 *		DT		  :  >  300ns => 19clock cycles
 *	
 *	And with system clock being 64MHz, and SPI baudrate 1MHz, a period is 64 cycles
 *	so half a clock is 32 cycles giving 500ns.
 *	Let's set 32 clock cycles for guarantee. So a prescaler of 1 (0) and a scaler of 32(4).
 *
 *	SPI framsize is set to 8
 *
*/
#define SPI_DELAY_MC33984 0x00040404
/**
 * \}
*/


/*
 * This functions toggles the reset input to go to the
 * NORMAL state. Watchdog is supposed to be disabled with FSI
 * being grounded.
 * 
 * Over current Low detection level is: 7.5A (111)
 * 	-Over current low detection blanking time is 150µs (11)
 * Over current High detection level is: 75A (1)
 * 	-Over-current high detection blanking time is 20µs (cannot be changed)
 *
 * Switching delays are 0ms for both HS0 and HS1
 *
 *
*/
void MC33983_DRV_init(void);


/*
 * This function is used to enable or disable one of the 
 * high side switches.
 *
 * HS_no : 		OUT_HS0 or 	OUT_HS1
 * enabled: 	0(nop)	or 	1(yes)
 * slew_rate: 	0(low)  or  1(high)
 *
*/
void config_HS(uint8_t HS_no, uint8_t enabled);


/*
 * This function is used for setting a switch to
 * a steady logical state, it automatically disabled the
 * PWM and sets the output to the specified value. 
 *
 * HS_no : 		OUT_HS0 or 	OUT_HS1
 * set	 :		1(HIGH) or  0(LOW)
 *
 *
*/
void write_HS(uint8_t HS_no, uint8_t set);

/*
 * This function enables or disables the PWM usage on a
 * high side switch. PWM's frequency is 300Hz.
 *
 * HS_no : 		OUT_HS0 or 	OUT_HS1
 * set	 :		1(enabdled) or  0(disabled)
 *
*/
void conf_PWM_HS(uint8_t HS_no, uint8_t en);

/*
 * This function sets the duty cycle of the PWM.
 *
 * HS_no : 		OUT_HS0 or 	OUT_HS1
 * duty_cycle:	1 to 100, set to 100 if bigger than 100 etc
 *
*/
void dutycycle_PWM_HS(uint8_t HS_no, uint8_t duty_cycle);


/*
 * This structure retains some info about the device.
 * It stores the switches configration state (enabled/not enabled).
 * And its fault registers.
 *
 *
 *
*/
struct _MC33984_STATUS
{
	uint8_t HS_0_FAULTR;			//the FAULT register from the documentation
	uint8_t HS_1_FAULTR;			//updated after a fault read or an external interrupt due to a fault
	uint8_t HS_0p_FAULTR;
	uint8_t HS_1p_FAULTR;
	uint8_t HS0_en;					//updated after a config_HS command.
	uint8_t HS1_en;
	uint8_t HS0p_en;
	uint8_t HS1p_en;
};
extern struct _MC33984_STATUS MC33984_STATUS;


/*
 * This function updates the stored fault register of a
 * selected high side, in the _MC33984_STATUS structure.
 *
*/
void read_fault(uint8_t HS_no);



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

//Channel 2 control functions as PWM or as a steady output
void eMIOS0_ch2_OPWMB_init(void);
void eMIOS0_ch2_OPWMB_setDC(uint8_t duty_cycle);
void eMIOS0_ch2_OPWMB_en(uint8_t enable);
void eMIOS0_ch2_set(uint8_t output);

//Channel 3 control functions as PWM or as a steady output
void eMIOS0_ch3_OPWMB_init(void);
void eMIOS0_ch3_OPWMB_setDC(uint8_t duty_cycle);
void eMIOS0_ch3_OPWMB_en(uint8_t enable);
void eMIOS0_ch3_set(uint8_t output);

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









#endif
