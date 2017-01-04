#include "MPC5604B.h"
#include "IntcInterrupts.h"
#include "MPC5604B_LED.h"

//Board led definition
#define PIN_LED_1 0x01000000 // PIN 1 for state blinking
#define PIN_LED_2 0x02000000
#define PIN_LED_3 0x04000000
#define PIN_LED_4 0x08000000
#define BOUTON_1

//Configuration of PORT E as GPIO
void config_PORT_E(void) 
{
	/* Key inputs			*/
	SIU.PCR[64].R = 0x0100;
	SIU.PCR[65].R = 0x0100;
	/* LED Outputs			*/
	SIU.PCR[68].R = 0x0200;
	SIU.PCR[69].R = 0x0200;
	SIU.PCR[70].R = 0x0200;
	SIU.PCR[71].R = 0x0200;	
}


void Set_Led1(void){
	SIU.PGPDO[2].R |= PIN_LED_1;
}

void Reset_Led1(void){
	SIU.PGPDO[2].R &=~PIN_LED_1;
}

void Invert_Led1(void){
	SIU.PGPDO[2].R =(~PIN_LED_1&SIU.PGPDO[2].R)|(SIU.PGPDO[2].R^PIN_LED_1);
}

void Set_Led2(void){
	SIU.PGPDO[2].R |= PIN_LED_2;
}

void Reset_Led2(void){
	SIU.PGPDO[2].R &=~PIN_LED_2;
}

void Set_Led3(void){
	SIU.PGPDO[2].R |= PIN_LED_3;
}

void Reset_Led3(void){
	SIU.PGPDO[2].R &=~PIN_LED_3;
}

void Set_Led4(void){
	SIU.PGPDO[2].R |= PIN_LED_4;
}

void Reset_Led4(void){
	SIU.PGPDO[2].R &=~PIN_LED_4;
}

uint8_t Read_button_1(void){
return SIU.GPDI[64].B.PDI;	
}

uint8_t Read_button_2(void){
return SIU.GPDI[65].B.PDI;	
}

uint8_t Read_button_3(void){
return SIU.GPDI[66].B.PDI;	
}

uint8_t Read_button_4(void){
return SIU.GPDI[67].B.PDI;	
}



