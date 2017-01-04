/*
 * Comodo.c
 *
 *  Created on: Jan 18, 2015
 *      Author: Antoine
 */

#include "MPC5604B.h"
#include "IntcInterrupts.h"
#include "config.h"
#include "Comodo.h"

void Init_button(void) // pour modeliser une interruption exterieures
{
	
	SIU.PCR[3].R = 0x0100; //config en entrée PA[3] //clignotant gauche
	SIU.PCR[6].R = 0x0100; //config en entrée PA[6] //navigation light
	SIU.PCR[7].R = 0x0100; //config en entrée PA[7]	//BIT_DIMMED_HEADLIGHT
	SIU.PCR[8].R = 0x0100; //config en entrée PA[8] BIT_HEADLIGHT
	SIU.PCR[14].R = 0x0100; //config en entrée PA[14] // clignotant droit 
	
	initADC();
}


uint8_t Read_comodo_button(void){
	uint8_t comodo_state=0;
	
	//BIT_FLASHING_LIGHT_LEFT
		if( SIU.GPDI[3].B.PDI==0){
			comodo_state|=BIT_FLASHING_LIGHT_LEFT; 
		}
		else{
			comodo_state&=~BIT_FLASHING_LIGHT_LEFT; 
		}
	
	//BIT_FLASHING_LIGHT_RIGHT
		if( SIU.GPDI[14].B.PDI==0){
			comodo_state|=BIT_FLASHING_LIGHT_RIGHT; 
		}
		else{
			comodo_state&=~BIT_FLASHING_LIGHT_RIGHT; 
		}
		
	//BIT_NAVIGATION_LIGHTS
		if( SIU.GPDI[6].B.PDI==0){
			comodo_state|=BIT_NAVIGATION_LIGHTS; 
		}
		else{
			comodo_state&=~BIT_NAVIGATION_LIGHTS; 
		}
			
	//BIT_DIMMED_HEADLIGHT
		if( SIU.GPDI[7].B.PDI==0){
			comodo_state|=BIT_DIMMED_HEADLIGHT; 
		}
		else{
			comodo_state&=~BIT_DIMMED_HEADLIGHT; 
		}
		
	//BIT_DIMMED_HEADLIGHT
		if( SIU.GPDI[8].B.PDI==0){
			comodo_state|=BIT_HEADLIGHT; 
		}
		else{
			comodo_state&=~BIT_HEADLIGHT; 
		}	
		
	return comodo_state;
}

void Turn_on_ADC(void){
	;
}

void Turn_off_ADC(void){
	;
}



void config_pad_B5(void)
{
	SIU.PCR[21].R = 0x2000; // Config APC => analog pad input
}

void config_pad_B4(void) 
{
	SIU.PCR[20].R = 0x2000; // Config APC => analog pad input
}



//pas de presampling, pas d'analog watchdog, pas d'interruption sur fin de conversion
//durée totale conversion = 15 périodes d'horloge ADC

void initADC(void) 
{
	
	config_pad_B4();
	config_pad_B5();
	//configuration de la conversion :
	ADC.MCR.B.WLSIDE = 0; //alignement à droite	
	ADC.MCR.B.MODE = 0;  //en mode one-shot pour avoir une seule mesure.
	ADC.MCR.B.ADCLKSEL = 1; //ADC clock freq = peripheral clock freq = 8MHz
	ADC.MCR.B.CTUEN = 0; //CTU triggered conversion disabled.
	ADC.MSR.B.NSTART = 1;
	ADC.MCR.B.JTRGEN = 1; 
	
	//choix des entrées analogiques à convertir de manière séquentielle 
	ADC.NCMR[0].B.CH0 = 1;    //ANP[0] : PB4
	ADC.NCMR[0].B.CH1 = 1;    //ANP[1] : PB5
	
	//config conversion timing : 15 période d'horloge ADC pour la durée totale de la conversion, soit 1.875 µs
	//configuration uniquement pour les entrées analog de précision
	// INPSAMPLE = 5, INPCMP = 1, INPLATCH = 0
	ADC.CTR[0].R = 0x00008205;
	
	//config du délai de sortie du mode power down: 8 périodes d'horloge ADC
	ADC.PDEDR.R = 0x00000008;
	
	start_ADC();
}


void LaunchConv(void) 
{
	ADC.MCR.B.NSTART = 1; //lancement de la conversion
}


uint8_t Read_luminosity_sensor(void){	
	char result = 0;
	uint32_t luminosity = 0;
	ADC.NCMR[0].B.CH1 = 1;    //ANP[1]
	ADC.NCMR[0].B.CH0 = 0;
	ADC.NCMR[1].B.CH7 = 0;
	
	LaunchConv(); //lancement de la séquence de conversion analogique numérique
	while (ADC.CDR[1].B.VALID != 1) {}; /* Wait for cross trigger to complete */
	luminosity = ADC.CDR[1].B.CDATA; 	     /* Read ANP1 conversion result data */ 

	if (luminosity < THRESHOLD_LUMINOSITY){
		result = 0xFF;
	}
	return result;
}

uint8_t Read_Steering_Position(void){
		static uint8_t result = 0;
		uint32_t position = 0;
		ADC.NCMR[0].B.CH1 = 0;   
		ADC.NCMR[0].B.CH0 = 1;
		ADC.NCMR[1].B.CH7 = 0;
		
		
		LaunchConv(); //lancement de la séquence de conversion analogique numérique
		while (ADC.CDR[0].B.VALID != 1) {}; /* Wait for cross trigger to complete */
		position = ADC.CDR[0].B.CDATA; 	     /* Read ANP0 conversion result data */ 

		if (position > THRESHOLD_RIGHT_HIGHT && result==00){
			result = DIRECTION_RIGHT;
		} 
		else if(position < THRESHOLD_LEFT_LOW && result==00){
			result = DIRECTION_LEFT;
		} else if(position>THRESHOLD_LEFT_HIGHT && result==DIRECTION_LEFT){
			result = 0x00;
		} else if(position<THRESHOLD_RIGHT_LOW && result==DIRECTION_RIGHT){
			result = 0x00;
		}
		return result;
}

void start_ADC(void){
	int i;
	
	//sortie du mode power down de l'ADC : nécessaire pour lancer toute conversion.
	ADC.MCR.B.PWDN = 0; //requête de sortie du mode power down
	while(ADC.MSR.R == 0x00000001) {} //Attente de la sortie du mode power down
	
	// Les deux boucles for ci-apres ont ete mises car parfois les premières mesures ne sont pas bonnes
	// Il y a parfois besoin de 5/6 conversions avant que la valeur sortant du capteur de luminosite soit stabilisee
	
	ADC.NCMR[0].B.CH1 = 1;   
	ADC.NCMR[0].B.CH0 = 0;
	for(i=0; i<10; i++){
		LaunchConv(); 
		while (ADC.CDR[1].B.VALID != 1) {};	
	}
	
	ADC.NCMR[0].B.CH1 = 0;   
	ADC.NCMR[0].B.CH0 = 1;
	for(i=0; i<10; i++){
			LaunchConv(); 
			while (ADC.CDR[0].B.VALID != 1) {};	
		}
}                                
                                 
               
