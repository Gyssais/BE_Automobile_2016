/*
 * mode_inchaalah.c
 *
 *  Created on: Jan 13, 2017
 *      Author: EL MEHDI
 */


#include "MPC5604B.h"
#include "modeinchaalah.h"
void aller_au_mode_RUN0(void){
	 ME.MCTL.R = 0x40005AF0;   // d'entre le code du mode RUN0 " 4" et la cle et l'anti-cle
		ME.MCTL.R = 0x4000A50F;    									
		while (ME.GS.B.S_MTRANS) {}    // une verification de la bonne transition du mode ( currentmode!=4)   
	while(ME.GS.B.S_CURRENTMODE != 4) {} 
}

void initialisation_du_mode_et_du_clock(void){
	ME.MER.R=0x0000001D; // ME.MER.R = 0x0000248D; // j'active tt les mode RUN0 et STOP0
	CGM.FXOSC_CTL.R = 0x00800000;// je viens de l'ajouter
	CGM.FIRC_CTL.B.RCDIV = 31;// CGM.FIRC_CTL.B.RCDIV = 31;//je viens de l'ajouter
	CGM.FMPLL_CR.B.NDIV = 0x40; //champ NDIV à 64//je viens de l'ajouter
	CGM.FMPLL_CR.B.IDF = 0b01; //champ IDF à 2//je viens de l'ajouter
	CGM.FMPLL_CR.B.ODF = 0b01; //champ ODF à 4//je viens de l'ajouter
	CGM.FMPLL_MR.B.FM_EN = 1; //Enable frequency modulation//je viens de l'ajouter
	CGM.FMPLL_MR.B.MOD_PERIOD = 160; //pour une modulation à 100kHz//je viens de l'ajouter
	CGM.FMPLL_MR.B.INC_STEP = 52; //cf  formule p107 -> modulation depth = 2%// je viens de l'ajouter
	CGM.FMPLL_CR.R=0x02400100; //ME.DRUN.R = 0x001F0010; je configure l'horloge du système 
	ME.DRUN.R = 0x001F0010; // je configure DRUN en  16MHIRCON=1, syclk=16 MHz FIRC */
	ME.RUN[0].R=0X001F0010; //ME.RUN[0].R = 0x001F0010; je configure RUN1 16MHIRCON=1, syclk=16 MHz FIRC */
	// le mode STOP sera configuré après, dans la fonction aller_au_mode_stop_0
	ME.RUNPC[0].R=0x00000080; // Configuration des périphériques actif l'or de RUN0
	// LA CONFIGURATION DES PEREO+PHERIQUES SERA FAITE APRES DANS LA FONCTION aller_au_mode_stop_0
	//ME.LPPC[1].R=0x00000200;
	ME.PCTL[32].R=0x08;// ce périphérique est actif l'or de RUN0 et STOP0 : ADC
	ME.PCTL[60].R=0x08;// ce périphérique est actif l'or de RUN0 et STOP0 : CAN
	ME.PCTL[16].R=0x08;// ce périphérique est actif l'or de RUN0 et STOP0 : CAN_0
	ME.PCTL[92].R=0x08;// ce périphérique est actif l'or de RUN0 et STOP0 : PIT
	ME.PCTL[68].R=0x08;// ce périphérique est actif l'or de RUN0 et STOP0 : SIUL
	ME.PCTL[69].R=0x08;// ce périphérique est actif l'or de RUN0 et STOP0 : WKUP
	WKUP.NCR.B.NDSS=00;// configuration de WKUP 
	WKUP.NCR.B.NWRE=1;
	WKUP.NCR.B.NFEE=1;
	WKUP.NCR.B.NFE=1;
	WKUP.WRER.R=0x38C00000; // LE WKUP configuration, pour configurer les differents périphériques actif avec le 
							// wkup unit lors des différents mmode, configure précédement avec le registe PCTL
	ME.MCTL.R=0x40005AF0;// tansistion vers le mode RUN0, entre la cle et l'anti cle
	ME.MCTL.R=0x4000A50F;// entre l'enti cle
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
	asm("wrteei 1");
}
/*void arreterl_interuption(void){
	asm("wrteei 0"); // arreter l'interuption 
}*/
/*void alleraumodeRUN0(void){
	ME.MCTL.R=0x40005AF0;
	ME.MCTL.R=0x4000A50F;
	while(ME.GS.B.S_MTRANS){}
	while(ME.GS.B.S_CURRENTMODE!=4){}
}*/

void aller_au_mode_STOP(void){
	ME.STOP0.R=0x0015000F;// configuration des horloge : FIRCON=0 MVRON=0, flash LP, no sysclk*/
	ME.LPPC[1].R=0x00000400;// Configuration des périphériques actif l'or de STOP0
	ME.MCTL.R=0xA0005AF0;// entrer la cle et l'anti cle
	ME.MCTL.R=0xA000A50F;
	while(ME.GS.B.S_MTRANS){}
	while(ME.GS.B.S_CURRENTMODE!=0xA){}
	if(WKUP.NSR.R && 0x18D00000 )// WKUP.WISR.R && 0x18d00000)
	{
		aller_au_mode_RUN0();
	}

}
	
/*void alleraumodeSTOP(void){
	ME.MCTL.R=0xA0005AF0;
	ME.MCTL.R=0xA000A50F;
	while(ME.GS.B.S_MTRANS){}
	while(ME.GS.B.S_CURRENTMODE!=4){}
}*/
void initialisation_du_timer_PIT(uint32_t Timeout ){
	PIT.PITMCR.R=0x00000001;
	PIT.CH[0].LDVAL.R=Timeout;
}
void PIT_actif(void){
	PIT.CH[0].LDVAL.R=0160000;
	PIT.CH[0].TCTRL.R=3;
	//PIT.CH[0].TCTRL.B.
}

void PIT_desactive(void){
	PIT.CH[0].TCTRL.B.TEN=0;
}
void interruption(void){
	//s'assuer que l'iterface CAN est entréé en mode sleep et que le contôleur CAN est entré en mode disable
	//FAIRE LE TEST 
	//CLEAR THE FLAG
	//ACTIVER LE MODE STOP
	CAN_1.MCR.B.MDIS=1;
			while(CAN_1.MCR.B.LPMACK!=1)
			{}
			PIT.CH[0].TCTRL.R=3;
			aller_au_mode_STOP();
			while ( WKUP.WISR.R != 0x10000000 || WKUP.WISR.R != 0x08000000 || WKUP.WISR.R !=0x00800000 || WKUP.WISR.R != 0x00400000 || WKUP.WISR.R != 0x0020000)
			{}
			WKUP.WISR.R=0xFFFFFFFF;
			aller_au_mode_RUN0();
	
}

//void PADSinitialisation(void){
	//SIU.PCR[17].R=
	
//}
//void reveilledemodestp(void){
	// VERIFIER LES INTERUPTIONS SUR LES PINS , POUR SIMULER LES HW EXTERNE, ET LE CAN
	//ENTRER EN MODE RUN0
	//WAKEUP A ULILISE OBOLIGATOIREMENT
	//if (WKUP.WISR.R && 0x18d00000){
		
	//}
	
//	alleraumodeRUN0();
//}
