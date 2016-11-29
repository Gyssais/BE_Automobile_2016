/*
 * SBC.c
 *
 *  Created on: Jan 18, 2015
 *      Author: Antoine
 */

#include "MPC5604B.h"
#include "IntcInterrupts.h"
#include "config.h"
#include "Driver_phare.h" 
#include "SBC.h"

#define SIZE_BUFFER_CAN 40

//global variable

uint8_t get_flag_frame_received(void){
	return 0;
}
void reset_flag_frame_received(void){
	;
}

uint16_t Read_voltage_value(void){
	return 0;
}

void initDSPI_1(void)
{
	DSPI_1.MCR.R = 0x80010001;		/* Set Master Mode, CS select at LOW, HALT=1			*/   
	DSPI_1.CTAR[0].R = 0x78024424;	/* Set timing: Tcsc=Tasc=4µs, Tdt= 1µs, BR=100kbits/s 	*/
  
 	DSPI_1.MCR.B.HALT = 0x0;	    /* Exit HALT mode: go from STOPPED to RUNNING state		*/
 	
 	SIU.PCR[113].R = 0x0A04;        /* MPC56xxB: Config pad as DSPI_0 SOUT output - PH1		*/
 	SIU.PCR[112].R = 0x0103;        /* MPC56xxB: Config pad as DSPI_0 SIN input - PH0 		*/
 		SIU.PSMI[8].R = 2;          /* MPC56xxB: Select PCR 112 for DSPI_1 SIN input 		*/
 	SIU.PCR[114].R = 0x0A04;        /* MPC56xxB: Config pad as DSPI_0 SCK output - PH2 		*/
 	SIU.PCR[115].R = 0x0A04;        /* MPC56xxB: Config pad as DSPI_0 PCS0 output - PH3 	*/
}

//FlexCAN1 initialisation
	void initCAN1 (void) {
	uint8_t   i;
	/* Put in Freeze Mode, local priority disable, enable only 8 message buffers, common ID filter */
	CAN_1.MCR.R = 0x50000007;  
	/* Configure for 8MHz OSC, 100KHz bit time */     
	CAN_1.CR.R = 0x04DB2003;        
	/* Disactivate all 64 message buffers */
	for (i=0; i<64; i++) {
		CAN_1.BUF[i].CS.B.CODE = 0;   
	} 
	/* MB 0 will be the TX buffer, so initialised with TX INACTIVE		*/
	CAN_1.BUF[0].CS.B.CODE = 8;     /* Message Buffer 0 set to TX INACTIVE */
	/* MB 1 will be RX buffer		*/
	CAN_1.BUF[1].CS.B.IDE = 0; 		/* MB 1 will look for a standard ID (11 bits) */
#ifdef BCM
	CAN_1.BUF[1].ID.B.STD_ID = ID_CAN_BCM ; /* MB 1 will look for ID = 555 */
#endif
#ifdef LCM
	CAN_1.BUF[1].ID.B.STD_ID = ID_CAN_LCM ; /* MB 1 will look for ID = 555 */
#endif
	CAN_1.BUF[1].CS.B.CODE = 4; 	 /* MB 1 set to RX EMPTY*/  

	/*Common ID filtering: accept all bits if standard ID is used for matching*/
	CAN_1.RXGMASK.R = 0x07FF0000; 

	/* Pin configuration		*/
	SIU.PCR[42].R = 0x0624;         /* MPC56xxB: Config port C10 as CAN1TX, open drain */
	SIU.PCR[43].R = 0x0100;         /* MPC56xxB: Configure port C11 as CAN1RX */
	SIU.PSMI[0].R = 0x01;           /* MPC56xxB: Select PCR 43 for CAN1RX Input */

	/* Leave Freeze mode			*/
	CAN_1.MCR.R = 0x00000007;       /* Negate FlexCAN1 halt state for the 8 first message buffers */
}
	
void ReadDataDSPI_1(void) {
	while (DSPI_1.SR.B.RFDF != 1){} /* Wait for Receive FIFO Drain Flag = 1 				*/
  	DSPI_1.POPR.R; 					/* Read data received by slave SPI 						*/
  	DSPI_1.SR.R = 0x80020000;       /* Clear TCF, RDRF flags by writing 1 to them 			*/
}
void Init_SBC_DBG(void) 			/* Send SPI commands for activating CAN Transciever		*/
{
	
	vuint32_t i;
	
	initDSPI_1();
	initCAN1();
	
  	DSPI_1.PUSHR.R = 0x0001DF80; 
    ReadDataDSPI_1();            	/* A dummy read after each command						*/
  	for (i=0; i<200; i++) {}		/* Wait a while for operations to be completed			*/
  	
  	DSPI_1.PUSHR.R = 0x00015A00; 
    ReadDataDSPI_1();				            
  	for (i=0; i<200; i++) {}

  	DSPI_1.PUSHR.R = 0x00015E90; 
    ReadDataDSPI_1();            
  	for (i=0; i<200; i++) {}
  	
  	DSPI_1.PUSHR.R = 0x000160C0; 
    ReadDataDSPI_1();            
  	for (i=0; i<200; i++) {}	
  	
  	DSPI_1.PUSHR.R = 0x00021800; 
    ReadDataDSPI_1();            
  	for (i=0; i<200; i++) {}	
}






void TransmitMsg(uint8_t * TxData, uint8_t length, uint16_t MsgID) {
	uint8_t	i;
	/* Assumption:  Message buffer CODE is INACTIVE --> done in initCAN1 */                                 
	CAN_1.BUF[0].CS.B.IDE = 0;           /* Use standard ID length */
	CAN_1.BUF[0].ID.B.STD_ID = MsgID;      /* Transmit ID */
	CAN_1.BUF[0].CS.B.RTR = 0;           /* Data frame, not remote Tx request frame */
	CAN_1.BUF[0].CS.B.LENGTH = length;
	for (i=0;i<length;i++){
		CAN_1.BUF[0].DATA.B[i]=TxData[i]; /* Data to be transmitted */
	}
	for(i=length;i<8;i++){
		CAN_1.BUF[0].DATA.B[i] = 0;	
	}
	CAN_1.BUF[0].CS.B.SRR = 1;           /* Tx frame (not required for standard frame)*/
	CAN_1.BUF[0].CS.B.CODE =0xC;         /* Activate msg. buf. to transmit data frame */ 
}

//Receive a message on MB 1 with data ID 555
//Print 4 LSB bits of the first byte on PE4-PE7.
void ReceiveMsg(void) {
	vuint8_t j;
	vuint32_t dummy;
	vuint32_t result32;
	uint8_t RxCODE;
	uint8_t RxID;
	uint8_t RxLENGTH;
	uint8_t RxDATA[SIZE_BUFFER_CAN];
	uint8_t RxTIMESTAMP;

	
	//IFRL = IFLAG1 in Bolero datasheet.
	while (CAN_1.IFRL.B.BUF01I == 0) {}		/* Wait for CAN 1 MB 1 flag */
	RxCODE   = CAN_1.BUF[1].CS.B.CODE;		/* Read CODE, ID, LENGTH, DATA, TIMESTAMP */
	RxID     = CAN_1.BUF[1].ID.B.STD_ID;
	RxLENGTH = CAN_1.BUF[1].CS.B.LENGTH;
	for (j=0; j<RxLENGTH; j++) { 
		RxDATA[j] = CAN_1.BUF[1].DATA.B[j];
	}
	RxTIMESTAMP = CAN_1.BUF[1].CS.B.TIMESTAMP; 
	dummy = CAN_1.TIMER.R;                	/* Read TIMER to unlock message buffers */    
	CAN_1.IFRL.R = 0x00000002;           	/* Clear CAN 1 MB 0 flag */
#ifdef BCM
	Data_treatment_BCM(RxLENGTH,RxDATA);
#endif
#ifdef LCM
	Data_treatment_LCM(RxLENGTH,RxDATA);
#endif
	
	}
	
	
void Data_treatment_BCM( uint8_t length , uint8_t * data){
	if (length==2){
		switch(data[0]){
		case LIGHT_MESSAGE :
			//TO DO
			break;
		case CURRENT_MESSAGE :
			// TO DO
			break;
		case SWITCH_MESSAGE :
			//TO DO
			break;
			
		}
	}
}

void Data_treatment_LCM( uint8_t length , uint8_t * data){
	if (length==2){
		if(data[0]==LIGHT_MESSAGE){
			//BIT_NAVIGATION_LIGHTS
			if(data[1]&BIT_NAVIGATION_LIGHTS){
				Position_ON();
			}
			else{
				Position_OFF();
			}
			//BIT_DIMMED_HEADLIGHT
			if(data[1]&BIT_DIMMED_HEADLIGHT) //croisement
			{
				Croisement_ON();
			}
			else{
				Croisement_OFF();
			}
			//BIT_HEADLIGHT
			if(data[1]&BIT_HEADLIGHT) //route
			{
				Route_ON();
			}
			else{
				Route_OFF();
			}
			//BIT_FLASHING_LIGHT_RIGHT
			if(data[1]&BIT_FLASHING_LIGHT_RIGHT) //clignotant
			{
				Clignotant_droit_ON();
			}
			else{
				Clignotant_droit_OFF();
			}
			//BIT_FLASHING_LIGHT_LEFT
			if(data[1]&BIT_FLASHING_LIGHT_LEFT)
			{
				Clignotant_gauche_ON();
			}
			else{
				Clignotant_gauche_OFF();
			}
			//BIT_DIRECTION_LIGHT_RIGHT
			if(data[1]&BIT_DIRECTION_LIGHT_RIGHT) //direction
			{
				Direction_droit_ON();
			}
			else{
				Direction_droit_OFF();
			}
			//BIT_DIRECTION_LIGHT_LEFT
			if(data[1]&BIT_DIRECTION_LIGHT_LEFT)
			{
				Direction_gauche_ON();
			}
			else{
				Direction_gauche_OFF();
			}
		
		
		}
		// A completer
		
	}
}
