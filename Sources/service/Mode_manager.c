/*
 * Mode_manager.c
 *
 *  Created on: Jan 18, 2015
 *      Author: Antoine
 */
/************* Include *****************/
#include "MPC5604B.h"
#include "IntcInterrupts.h"
#include "config.h"
#include "Mode_manager.h"


/************* Private function ********/


/************* Public function **********/
/*
 * Modes.c
 *
 *  Created on: Jan 19, 2015
 *      Author: Clément
 */

// avec modif de clément


void disableWatchdog(void) {
  SWT.SR.R = 0x0000c520;     /* Write keys to clear soft lock bit */
  SWT.SR.R = 0x0000d928; 
  SWT.CR.R = 0x8000010A;     /* Clear watchdog enable (WEN) */
}

//PIT 1 Timer initialisation: an interrupt every 100ms
void initPIT1(void){
  PIT.PITMCR.R = 0x00000001;       /* Enable PIT and configure to stop in debug mode */
  PIT.CH[1].LDVAL.R = 6400000;       /* Timeout= 800000 sysclks x 1sec/8M sysclks = 200 ms */
  PIT.CH[1].TCTRL.R = 0x000000003; /* Enable PIT1 interrupt & start PIT counting */
}


void initModesAndClock(void) {
	
	ME.MER.R = 0x0000047D;
	// ME = Mode Entry module
	// Enable STOP, DRUN, RUN0, RUN1, RUN2, SAFE, RESET modes
	// Mode enable register : tous les modes utilisés dans le pg
	// les réglages de ME_RUN_PC ne servent à rien si les modes ne sont pas enable dans MER
	
	CGM.FXOSC_CTL.R = 0x00800000;
	// CGM = Clock Generation Module 
	// external 8MHz , 0x80 dans EOCV, I_clk est envoyé lorsque 512*EOCV coup d'horloge ont eu lieu
	//pour s'assurer que la clock donnée par le quartz est stable
	
	CGM.FIRC_CTL.B.RCDIV = 31;
	//diviseur de Fast Internal Oscillator à 32 => 16MHz/32 = 500kHz
	//pas de EOCV (c'et un RC pas un quartz)
		
	// Config pour RUN0
	// RUN0 cfg:
	// FIRCON = 0 Fast internal RC oscillator -> not used
	// OSC0ON = 1 Activer Fast external oscillator 8MHz
	// PLL0ON = 1 Activer PLL	
	ME.RUN[0].R = 0x001F0064;
	
	// Config pour RUN1 désactivation PLL
	ME.RUN[1].R = 0x001F0023; //div ext xtal oscillator
	
	// Config pour RUN2 : passage sur le internal RC oscillator 16MHz/32
	//FMPLL = 0
	//FIRCON = 1
	//OSC0ON = 0
	ME.RUN[2].R = 0x001F0011;
	
	//test
	//ME.RUN[2].R = 0x001F0010;
	//ME.RUN[2].R = 0x001F0071; FIRCON = 1, OSCON = 1, FMPLL ON = 1
	
	//Config pour STOP0 : même config que RUN2
	ME.STOP0.R = 0x001F0011;
	
	// 64MHz FMPLL (cf formule boyer p3)
	// datasheet p105 + tableur
	CGM.FMPLL_CR.B.NDIV = 0x40; //champ NDIV à 64
	CGM.FMPLL_CR.B.IDF = 0b01; //champ IDF à 2
	CGM.FMPLL_CR.B.ODF = 0b01; //champ ODF à 4
	CGM.FMPLL_MR.B.FM_EN = 1; //Enable frequency modulation
	CGM.FMPLL_MR.B.MOD_PERIOD = 160; //pour une modulation à 100kHz
	CGM.FMPLL_MR.B.INC_STEP = 52; //cf  formule p107 -> modulation depth = 2%
	//si jamais pb, remplir INC_STEP qui correspond à l'indice de modulation
	
	// Peri. Cfg. 8 configurations au max :
	// p162 RM
	ME.RUNPC[0].R = 0x00000000; //les périph qui prennent cette config ne sont actifs dans aucun des modes
	ME.RUNPC[1].R = 0x00000010; //les périph qui prennent cette config seront actifs en RUN0 seulement	
	ME.RUNPC[2].R = 0x00000020; //les périph qui prennent cette config seront actifs en RUN1 seulement		
	ME.RUNPC[3].R = 0x00000030; //les périph qui prennent cette config seront actifs en RUN0 et RUN1
	ME.RUNPC[4].R = 0x00000040; //les périph qui prennent cette config seront actifs en RUN2 seulement
	ME.RUNPC[5].R = 0x00000070; //les périph qui prennent cette config seront actifs en RUN0, RUN1 et RUN2
	
	// Peri. Cfg. mode low power
	ME.LPPC[1].R = 0x000000400; //les périph qui prennent cette config seront actifs en STOP
	
	// Selection of operation modes for active peripherals.
	// voir p165 de la RM pour les registres PCTL des périphériques
	
	ME.PCTL[68].B.RUN_CFG = 5;
	ME.PCTL[68].B.LP_CFG =  1;
	
	// On utilise la config RUNPC[3] pour les SIUL
	// (pad config, port, GPIO, interrupts) (actifs en RUN0, RUN1 et RUN2)
	
	ME.PCTL[16].B.RUN_CFG = 3; //Flexcan0 config3 RUN
	ME.PCTL[17].B.RUN_CFG = 3; //Flexcan1
	ME.PCTL[18].B.RUN_CFG = 3; //Flexcan2
	ME.PCTL[10].B.RUN_CFG = 3; //Flexcan3
	ME.PCTL[20].B.RUN_CFG = 3; //Flexcan4
	ME.PCTL[21].B.RUN_CFG = 3; //Flexcan5
	
	ME.PCTL[4].B.RUN_CFG = 3; //DSPI_0 config RUN 3
	ME.PCTL[5].B.RUN_CFG = 3; //DSPI_1 config RUN 3	
	ME.PCTL[6].B.RUN_CFG = 3; //DSPI_2 config RUN 3		
    ME.PCTL[7].B.RUN_CFG = 3; //DSPI_3 config RUN 3 
   	
    ME.PCTL[32].B.RUN_CFG = 5; //ADC_0
    // ADC_0 actifs en RUN0 et en RUN1 et RUN2
    
#ifdef LCM
    ME.PCTL[92].B.RUN_CFG = 5 ;
    ME.PCTL[92].B.LP_CFG = 1;
#endif
#ifdef BCM
    ME.PCTL[92].B.RUN_CFG = 5; 
    ME.PCTL[92].B.LP_CFG = 1;
#endif
    //PIT Periodic Interrupt Timer
    // actifs en RUN0 et en RUN1 et RUN2 et STOP
    
    ME.PCTL[60].B.LP_CFG = 1;
    //CAN sampler actif en mode STOP
	
    // Mode Transition to enter RUN0 mode:
    // MCTL = Mode Control Register
    // Trigger des changements de mode : passage du mode DRUN au mode RUN0
	ME.MCTL.R = 0x40005AF0;  // Enter RUN0 Mode & Key,  16 LSB : Key, 16 MSB : Selection du mode (p158 RM)
	ME.MCTL.R = 0x4000A50F;  // Enter RUN0 Mode & Inverted Key (obligatoire d'écrire la clé et son inverse)  
									
	while (ME.GS.B.S_MTRANS) {}     	 // Wait for mode transition to complete   
	while(ME.GS.B.S_CURRENTMODE != 4) {} // Verify RUN0 is the current mode
	// Note: This verification ensures a SAFE mode 
	// transition did not occur. SW could instead 
	// enable the safe mode transition interupt
	
	enableIrq();//activate external interrupts
		
}


//Enable masked interrupts
void enableIrq(void) {
	INTC.CPR.B.PRI = 0;          /* Single Core: Lower INTC's current priority */
	asm(" wrteei 1");	    	   /* Enable external interrupts */
}


//Disable masked interrupts
void disableIrq(void) {
	asm(" wrteei 0");	    	   /* Disable external interrupts */
} 

//NORMAL_MODE_INIT
void GoModeRUN0(){
	ME.MCTL.R = 0x40005AF0; //RUN0 + Key      
	ME.MCTL.R = 0x4000A50F; //RUN0 + Inverted Key       
	  											
	while (ME.GS.B.S_MTRANS) {}     	 // Wait for mode transition to complete 
	while(ME.GS.B.S_CURRENTMODE != 4) {} // Verify RUN0 is the current mode
}

//FAILSOFT_MODE_INIT
void GoModeRUN1(){
	ME.MCTL.R = 0x50005AF0; //RUN1 + Key
	ME.MCTL.R = 0x5000A50F; //RUN1 + Inverted Key
	  			  											
	while (ME.GS.B.S_MTRANS) {}          // Wait for mode transition to complete  
	while(ME.GS.B.S_CURRENTMODE != 5) {} // Verify RUN1 is the current mode
}

//LOW_POWER_MODE_INIT
void GoModeRUN2(){
	ME.MCTL.R = 0x60005AF0; //RUN2 + Key
	ME.MCTL.R = 0x6000A50F; //RUN2 + Inverted Key
		  			  											
	while (ME.GS.B.S_MTRANS) {}          // Wait for mode transition to complete  
	while(ME.GS.B.S_CURRENTMODE != 6) {} // Verify RUN2 is the current mode
	
	//initPIT1();
	//INTC_InstallINTCInterruptHandler(PIT1_Interrupt,60,2);	/* Set PIT1 interrupt handler*/
	
}

void GoModeSTOP(){
	ME.MCTL.R = 0xA0005AF0; //STOP + Key
	ME.MCTL.R = 0xA000A50F; //STOP + Inverted Key
			  			  											
	while (ME.GS.B.S_MTRANS) {}            // Wait for mode transition to complete  
	while(ME.GS.B.S_CURRENTMODE != 0xA) {} // Verify RUN2 is the current mode
}

