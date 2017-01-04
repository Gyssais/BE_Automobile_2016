

 

#include "MPC5604B.h"
#include "IntcInterrupts.h"


uint8_t LED_state = 0; 


/*========================================================================*/
/*                          PROTOTYPES                                    */
/*========================================================================*/
void initModesAndClock(void);
void config_PORT_E(void);
void initPIT1(void);
void startPIT1(void);
void enableIrq(void);
void disableWatchdog(void);
void PIT1_Interrupt(void);


//initialisation de l'horloge système, configuration des modes de fonctionnement des périphériques, entrée dans le mode Run0
void initModesAndClock(void) {
  ME.MER.R = 0x0000001D;          /* Enable DRUN, RUN0, SAFE, RESET modes */
  
  /* PLL is turning off, the bus clock is derived from Fast oscillator clock
  ext quartz = 8 MHz, division by 8 to have a 1 MHz bus clock*/                                			
  CGM.FXOSC_CTL.R = 0x00800700;
  /* RUN0 cfg: voltage regu activé, flash en mode normal, 
  FIRCON=0,OSC0ON=1,PLL0ON=0,syclk=Div ext xtal clock */
  ME.RUN[0].R = 0x001F0023;       

  /* Peri. Cfg. 1 settings: only run in RUN0 mode
  Only RUNPC[1] mode configuration is defined. Only this configuration will be used.*/ 
  ME.RUNPC[1].R = 0x00000010; 	  
  										
  /*Selection of operation modes for active peripherals.*/ 
  //Voir Table 3.1 p 71/956 datasheet Bolero512K_RM_Rev7_07_2010.pdf pour obtenir les numéros associés à chaque périphérique.
  ME.PCTL[68].R = 0x01;  //SIUL use the configuration of RunPC[1]  
  ME.PCTL[92].R = 0x01;  //PIT1 use the configuration of RunPC[1]
  
                                  /* Mode Transition to enter RUN0 mode: */
  ME.MCTL.R = 0x40005AF0;         /* Enter RUN0 Mode & Key */
  ME.MCTL.R = 0x4000A50F;         /* Enter RUN0 Mode & Inverted Key */  
  //la transition de mode exige ces 2 instructions avec la clé la 1e fois, puis la clé inversée.
  									
  
  while (ME.GS.B.S_MTRANS) {}     /* Wait for mode transition to complete */    
 
  while(ME.GS.B.S_CURRENTMODE != 4) {} /* Verify RUN0 is the current mode */
                                  /* Note: This verification ensures a SAFE mode */
                                  /*       tranistion did not occur. SW could instead */
                                  /*       enable the safe mode tranision interupt */
}


//configuration du port E en sortie, activation de la fonction GPIO.
void config_PORT_E(void) 
{
	SIU.PCR[68].R = 0x0200;
	SIU.PCR[69].R = 0x0200;
	SIU.PCR[70].R = 0x0200;
	SIU.PCR[71].R = 0x0200;	
}

//initialisation du timer PIT1
//celui s'incrémente au rythme de l'horloge système (1MHz). Pour avoir un débordement toutes les 500 ms,
//il faut fournir 500000 comme valeur au compteur.
void initPIT1(void) {
  PIT.PITMCR.R = 0x00000001;       // Enable PIT and configure to stop in debug mode 
  PIT.PITMCR.B.MDIS = 1; 	//disable PIT module 
  PIT.CH[1].LDVAL.R = 500000;     /* Timeout= 500000 sysclks x 1sec/1M sysclks = 0.5 s */
  PIT.CH[1].TCTRL.B.TIE = 1; //Enable PIT1 Interrupt
}

void startPIT1(void) {
	PIT.PITMCR.B.MDIS = 0; //enable PIT module
	PIT.CH[1].TCTRL.B.TEN = 1; //Enable channel 1 of PIT1
}

//autorisation des interruptions masquables
void enableIrq(void) {
  INTC.CPR.B.PRI = 0;          /* Single Core: Lower INTC's current priority */
  asm(" wrteei 1");	    	   /* Enable external interrupts */
} 

//desactivation du watchdog du microcontrôleur
void disableWatchdog(void) {
  SWT.SR.R = 0x0000c520;     /* Write keys to clear soft lock bit */
  SWT.SR.R = 0x0000d928; 
  SWT.CR.R = 0x8000010A;     /* Clear watchdog enable (WEN) */
}  

/*
void Init_PIT(void)
{
	PIT.MCR.R = 0x00000001;	// Enable PIT and configure to stop in debug mode 
	PIT.MCR.B.MDIS = 1; 	//disable PIT module 
}


void Set_Counter_Value_PIT(uint8_t timer, uint32_t TimeOut) //count down and generate interrupt
{
	PIT.CH[timer].LDVAL.R = TimeOut; 	
}


void Start_PIT(uint8_t timer) 
{ 	
	PIT.CH[timer].TCTRL.B.TEN = 1;
	PIT.MCR.B.MDIS = 0; //enable PIT module 
}


void Disable_PIT(uint8_t timer) 
{ 	
	PIT.CH[timer].TCTRL.B.TEN = 0;
}


void Autorize_IT_PIT(uint8_t timer) 
{ 	
	PIT.CH[timer].TCTRL.B.TIE = 1;
}


void Clear_Flag_PIT(uint8_t timer)
{
	//raz du flag interruption PITx
	PIT.CH[timer].TFLG.B.TIF = 1;
}
*/
