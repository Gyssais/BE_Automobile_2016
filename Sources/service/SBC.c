/*
 * SBC.c
 *
 *  Created on: Jan 18, 2015
 *      Author: Antoine
 *      
 *  Modified by: Gido van Wijk
 *  Date: December 2016
 */

/*
 * Utilisation dans le main.c
 * 		initialise_SPI_DRIVER();
		SPI[1].init(SPI_BAUD_62500, SPI_DELAY_DEFAULT);
		Init_SBC_DBG();
		
 */

#include "MPC5604B.h"
#include "IntcInterrupts.h"
#include "config.h"
#include "SBC.h"
#include "spi_drv.h"
#include "can_drv.h"
#include "define.h"
#include "interrupt_number.h"
#include "Buttons_management.h"

#define SIZE_BUFFER_CAN 40

struct _SPI_DRV SPI[];

uint8_t get_flag_frame_received(void){
	return 0;
}
void reset_flag_frame_received(void){
	;
}

uint16_t Read_voltage_value(void){
	return 0;
}



/* Init_SBC_DBG
 * 
 * Func: This function initialises the CAN in device debug mode
 * !! POWER UP WITH DBG PIN AT 8-10V !!
 * Disconnect J4 and connect J5
 */
void Init_SBC_DBG(void) 			
{
		uint32_t i;
		uint16_t TData;
		uint16_t RData;
		uint16_t ID_prod;
				
		// Demande ID produit
		// initCAN1();
		
		TData = 0x2580;
		SPI[1].write(&TData);
		SPI[1].read(&RData);
		ID_prod = RData;
				
		TData = 0xDF80; // Read Vreg register H
		SPI[1].write(&TData);
		SPI[1].read(&RData);
		
		TData = 0x5A00; // Enter in Normal Mode
		SPI[1].write(&TData);
				
		TData = 0x5E90; // Voltage regulator config: 5V_CAN and Vaux
		SPI[1].write(&TData);
				
		TData = 0x60C0; // Config CAN: Set CAN in TxRx Mode, fast slew rate
		SPI[1].write(&TData);
				
		TData = 0x1800;
		SPI[1].write(&TData);
}




	



