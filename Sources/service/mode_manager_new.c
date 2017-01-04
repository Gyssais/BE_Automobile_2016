/*
 * mode_manager.c
 *
 *  Created on: Jan 4, 2017
 *      Author: EL MEHDI
 */


#include "MPC5604B.h"

#include "mode_manager_new.h"



void initialisation_du_mode_et_du_clock(void){
	ME.MER.R=0x0000001D;
	CGM.FMPLL_CR.R=0x02400100;
	ME.RUN[0].R=0X001F0074;
	ME.RUNPC[0].R=0x00000010;
	ME.PCTL[32].R=0x08;
	ME.PCTL[60].R=0x08;
	ME.PCTL[16].R=0x08;
	ME.PCTL[92].R=0x08;
	ME.PCTL[68].R=0x08;
	ME.PCTL[69].R=0x08;
	WKUP.NCR.B.NDSS=00;
	WKUP.NCR.B.NWRE=1;
	WKUP.NCR.B.NFEE=1;
	WKUP.NCR.B.NFE=1;
	WKUP.WRER.R=0x38C00000;
	ME.MCTL.R=0x40005AF0;
	ME.MCTL.R=0x4000A50F;
	while(ME.GS.B.S_MTRANS){}
	while(ME.GS.B.S_CURRENTMODE!=4){}
}
void aller_au_mode_RUN0(void){
	 ME.MCTL.R = 0x40005AF0;  
		ME.MCTL.R = 0x4000A50F;    
										
		while (ME.GS.B.S_MTRANS) {}       
	while(ME.GS.B.S_CURRENTMODE != 4) {} 
}
void enlever_watchdog(void){
	SWT.SR.R=0x0000C520;
	SWT.SR.R=0x0000D928;
	SWT.CR.R=0x8000010A;
}

void faire_fonctionner_l_interuption(void){
	INTC.CPR.B.PRI=0;
	asm("wrteei 1");
}
void arreter_l_interuption(void){
	asm("wrteei 0");
}
/*void alleraumodeRUN0(void){
	ME.MCTL.R=0x40005AF0;
	ME.MCTL.R=0x4000A50F;
	while(ME.GS.B.S_MTRANS){}
	while(ME.GS.B.S_CURRENTMODE!=4){}
}*/

void aller_au_mode_STOP(void){
	ME.STOP0.R=0X001F0074;
	ME.LPPC[1].R=0x00000400;
	ME.MCTL.R=0xA0005AF0;
	ME.MCTL.R=0xA000A50F;
	while(ME.GS.B.S_MTRANS){}
	while(ME.GS.B.S_CURRENTMODE!=4){}
	if(WKUP.NSR.R && 0x18D00000){
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
	PIT.CH[0].TCTRL.B.TIE=1;
}
void interruption(void){
	//s'assuer que l'iterface CAN est entréé en mode sleep et que le contôleur CAN est entré en mode disable
	//FAIRE LE TEST 
	//CLEAR THE FLAG
	//ACTIVER LE MODE STOP
	CAN_1.MCR.B.MDIS=1;
			if(CAN_1.MCR.B.LPMACK==1)
			{
				PIT.CH[0].TCTRL.B.TIE=1;
				aller_au_mode_STOP();
			}
	
	
}
void PAD_S_initialisation(void){
//	SIU.PCR[17].R=
	
//}
	// VERIFIER LES INTERUPTIONS SUR LES PINS , POUR SIMULER LES HW EXTERNE, ET LE CAN
	//ENTRER EN MODE RUN0
	//WAKEUP A ULILISE OBOLIGATOIREMENT
	
	aller_au_mode_RUN0();
}


