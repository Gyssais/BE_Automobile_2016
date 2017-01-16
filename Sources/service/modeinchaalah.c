/*
 * mode_inchaalah.c
 *
 *  Created on: Jan 13, 2017
 *      Author: EL MEHDI
 */


#include "MPC5604B.h"
#include "modeinchaalah.h"
#include "pit.h"


void aller_au_mode_RUN0(void){
	 ME.MCTL.R = 0x40005AF0;   // d'entre en MODE RUN0, et la cle
		ME.MCTL.R = 0x4000A50F;   // entree en MODE RUN0 , anti cle						
		while (ME.GS.B.S_MTRANS) {}    // une verification de la bonne transition du mode ( currentmode!=4)   
	while(ME.GS.B.S_CURRENTMODE != 4) {} 
}

void initialisation_du_mode_et_du_clock(void){
	ME.MER.R=0x0000043D;// on active le mode RUN0, RUN1, le DRUN, STOP0, SAFE, RESET
	CGM.FXOSC_CTL.R = 0x00800000;// CGM = Clock Generation Module 
	// external 8MHz , 0x80 dans EOCV, I_clk est envoyé lorsque 
	//les 512*EOCV coup d'horloge finissent afin de s'assurer que la clock donnée par le quartz est stable
	CGM.FIRC_CTL.B.RCDIV = 31;//diviseur de Fast Internal Oscillator à 32 => 16MHz/32 = 500kHz
	CGM.FMPLL_CR.B.NDIV = 0x40; //champ NDIV à 64
	CGM.FMPLL_CR.B.IDF = 0b01; //champ IDF à 2
	CGM.FMPLL_CR.B.ODF = 0b01; //champ ODF à 4
	CGM.FMPLL_MR.B.FM_EN = 1; //Enable frequency modulation
	CGM.FMPLL_MR.B.MOD_PERIOD = 160; //pour une modulation à 100kHz
	CGM.FMPLL_MR.B.INC_STEP = 52; //cf  formule p107 -> modulation depth = 2%
	//CGM.FMPLL_CR.R=0x02400100; //ME.DRUN.R = 0x001F0010; je configure l'horloge du système 
	CGM.SC_DC[1].R = 0x80; //InitPeriClkGen
	ME.DRUN.R = 0x001F0010; // je configure DRUN en  16MHIRCON=1, syclk=16 MHz FIRC */
	ME.RUN[0].R=0X001F0010; //ME.RUN[0].R = 0x001F0010; je configure RUN1 16MHIRCON=1, syclk=16 MHz FIRC */
	// le mode STOP sera configuré après, dans la fonction aller_au_mode_stop_0
	// la configuration des peréphériques SERA FAITE APRES DANS LA FONCTION aller_au_mode_stop_0
	ME.RUNPC[0].B.RUN0=1;// PERIPHERIQUE ACTIF EN MODE RUN0
	ME.RUNPC[0].B.DRUN=1;// PERIPHERIQUE ACTIF EN MODE DRUN
	//ME.PCTL[32].R=0x08;// ce périphérique est actif l'or de RUN0 et STOP0 : ADC
	ME.PCTL[32].B.LP_CFG=000;// ADC actif POUR RUNPC0
	ME.PCTL[32].B.RUN_CFG=000;// ADC actif POUR LOW POWER MODE
	//ME.PCTL[60].R=0x08;// ce périphérique est actif l'or de RUN0 et STOP0 : CAN
	ME.PCTL[60].B.LP_CFG=000;// CAN SAMPLER actif POUR RUNPC0
	ME.PCTL[60].B.RUN_CFG=000;// CAN SAMPLER actif POUR LOW POWER MODE
	//ME.PCTL[16].R=0x08;// ce périphérique est actif l'or de RUN0 et STOP0 : CAN_0
	ME.PCTL[16].B.LP_CFG=000;// CAN_0 actif POUR RUNPC0
	ME.PCTL[16].B.RUN_CFG=000;// CAN_0 actif POUR LOW POWER MODE
	//ME.PCTL[92].R=0x08;// ce périphérique est actif l'or de RUN0 et STOP0 : PIT
	ME.PCTL[92].B.LP_CFG=000;// PIT actif POUR RUNPC0
	ME.PCTL[92].B.RUN_CFG=000;// PIT actif POUR LOW POWER MODE
	//ME.PCTL[68].R=0x08;// ce périphérique est actif l'or de RUN0 et STOP0 : SIUL
	ME.PCTL[68].B.LP_CFG=000;// SIUL actif POUR RUNPC0
	ME.PCTL[68].B.RUN_CFG=000;// SIUL actif POUR LOW POWER MODE
	//ME.PCTL[69].R=0x08;// ce périphérique est actif l'or de RUN0 et STOP0 : WKUP
	ME.PCTL[69].B.LP_CFG=000;// WKUP actif POUR RUNPC0
	ME.PCTL[69].B.RUN_CFG=000;// WKUP actif POUR LOW POWER MODE
	//ME.PCTL[92].B.LP_CFG=
	//ME.PCTL[92].B.RUN_CFG=
	WKUP.NCR.B.NDSS=00;// configuration de WKUP 
	WKUP.NCR.B.NWRE=1;
	WKUP.NCR.B.NFEE=1;
	WKUP.NCR.B.NFE=1;
	WKUP.WRER.B.WRE=0x10710;// LE WKUP configuration, pour configurer les differents périphériques actif avec le 
							// wkup unit lors des différents mode, configuré précédement avec le registe PCT
	WKUP.WIFER.B.IFE=0x10710;
	WKUP.WIREER.B.IREE=0x10710;
	WKUP.WIPUER.R = 0x000FFFFF; /* MPC56xxB: Ena. WKUP pins pullups to stop leakage*/
	ME.MCTL.R=0x40005AF0;// tansistion vers le mode RUN0  CODE+CLE
	ME.MCTL.R=0x4000A50F;// TRANSISTION MODE RUN0 CODE+ANTICLE
	while(ME.GS.B.S_MTRANS){}
	while(ME.GS.B.S_CURRENTMODE!=4){}
}
void enlever_watchdog(void){
	SWT.SR.R=0x0000C520;
	SWT.SR.R=0x0000D928;
	SWT.CR.R=0x8000010A;
}

void faire_fonctionner_l_interuption(void){
	INTC.CPR.B.PRI=0; // lancée l'interuption 
	asm("wrteei 1");// enable les intéruption externes
}


	
void aller_au_mode_STOP0(void){
	ME.MCTL.R=0xA0005AF0;
	ME.MCTL.R=0xA000A50F;
	while(ME.GS.B.S_MTRANS){}
	while(ME.GS.B.S_CURRENTMODE!=4){}
}
void initialisation_du_timer_PIT(uint32_t Timeout ){
	PIT.PITMCR.R=0x00000001;
	PIT.CH[0].LDVAL.R=Timeout;
}


void PIT_desactive(void){
	PIT.CH[0].TCTRL.B.TEN=0;
}
void interruption(void){
	
	CAN_1.MCR.B.MDIS=1; // mettre l'iterface CAN en mode disable
	while(CAN_1.MCR.B.LPMACK!=1)
		{}// s'assurer que le CAN est en mode disable 
	PIT_desactive();// je desactive le timer PIT0
	aller_au_mode_STOP0();// faire la transistion vers le mode stop0
	while ( WKUP.WISR.R != 0x00000010 || WKUP.WISR.R != 0x00000100 || WKUP.WISR.R !=0x00000100 || WKUP.WISR.R != 0x00080000 || WKUP.WISR.R != 0x0020000)
		{}
	WKUP.WISR.R=0xFFFFFFFF;// REMETTRE LES FLAGS EN ETATS NORMALE
	aller_au_mode_RUN0();// ALLER EN MODE RUN0
	startChannelPIT(0);
}


