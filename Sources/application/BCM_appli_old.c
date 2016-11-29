/*
 * BCM_appli.c
 *
 *  Created on: Jan 19, 2015
 *      Author: Antoine
 */

#include "MPC5604B.h"
#include "IntcInterrupts.h"
#include "config.h"
#include "BCM_appli.h"
#include "LCM_appli.h"
#include "Comodo.h"
#include "SBC.h"
#include "Mode_manager.h"

/*************** Static variable **************/
uint8_t mode_BCM=NORMAL_MODE;
uint8_t light_state_BCM=0;

//******************************** BCM *************************************//

//function to update lights value in function of the button
void Update_light(void){
	uint16_t Steering_wheel;
	uint16_t Luminosity;
	uint8_t Comodo_button;
	uint8_t Last_light_state;
	uint8_t current_light;
	uint8_t tab_can[2];
	
	
	Last_light_state=light_state_BCM;
	current_light=Last_light_state;
	
	Steering_wheel=Read_Steering_Position();
	Luminosity=Read_luminosity_sensor();
	Comodo_button=Read_comodo_button();
	
	//NAVIGATION_LIGHT
	if((Comodo_button&BIT_BUTTON_NAVIGATION)==BIT_BUTTON_NAVIGATION){
		current_light|=BIT_NAVIGATION_LIGHTS;
	}
	else{
		current_light&=~BIT_NAVIGATION_LIGHTS;
	}
	
	//DIMMED HEAD LIGHT
	if(((Comodo_button&BIT_BUTTON_DIMMED_HEADLIGHT)==BIT_BUTTON_DIMMED_HEADLIGHT)||(Luminosity<MIN_LUMINOSITY)){
			current_light|=BIT_DIMMED_HEADLIGHT;
	}
	else{
			current_light&=~BIT_DIMMED_HEADLIGHT;
	}
	
	//HEAD LIGHT
	if((Comodo_button&BIT_BUTTON_HEADLIGHT)==BIT_BUTTON_HEADLIGHT){
		current_light|=BIT_HEADLIGHT;
	}
	else{
			
		current_light&=~BIT_HEADLIGHT;
	}
		
	//Flashing LIGHT right
	if((Comodo_button&BIT_BUTTON_FLASHING_LIGHT_RIGHT)==BIT_BUTTON_FLASHING_LIGHT_RIGHT){
			current_light|=BIT_FLASHING_LIGHT_RIGHT;
	}
	else{
			current_light&=~BIT_FLASHING_LIGHT_RIGHT;
	}
	//Flashing LIGHT left
	if((Comodo_button&BIT_BUTTON_FLASHING_LIGHT_LEFT)==BIT_BUTTON_FLASHING_LIGHT_LEFT){
			current_light|=BIT_BUTTON_FLASHING_LIGHT_LEFT;
	}
	else{
			current_light&=~BIT_BUTTON_FLASHING_LIGHT_LEFT;
	}
	
	//Direction LIGHT left
	if(Steering_wheel==DIRECTION_RIGHT){
			current_light|=BIT_DIRECTION_LIGHT_RIGHT;
	}
	else{
			current_light&=~BIT_DIRECTION_LIGHT_RIGHT;
	}
	//Direction LIGHT right
	if(Steering_wheel==DIRECTION_LEFT){
			current_light|=BIT_DIRECTION_LIGHT_LEFT;
	}
	else{
			current_light&=~BIT_DIRECTION_LIGHT_LEFT;
	}
	
	light_state_BCM=current_light;
			
	//if state changed send data
	if(current_light!=Last_light_state){
		tab_can[0]=LIGHT_MESSAGE;
		tab_can[1]=current_light;
		TransmitMsg(tab_can,2,ID_CAN_LCM);
	}

}


void Normal_mode_BCM(void){
	static sleep_cpt; //counter to know if we go to listen only and sleep mode
	
	Update_light();
	
	//counter to set sleep mode and 
	if((light_state_BCM==0x00)&&(get_flag_frame_received()==0)){
		sleep_cpt++;
		if(sleep_cpt>LISTEN_ONLY_TIME){
			//sleep mode
			if(sleep_cpt>SLEEP_TIME){
				//GoModeRUN2();
				//mode_BCM=LOW_POWER_MODE;
				
			}
			//listen only mode
			else{
				
			}
		}
	}
	else{
		reset_flag_frame_received();
		sleep_cpt=0;
	}

	
	
}
	

void Low_power_mode_BCM(void){
	uint16_t Luminosity;
	uint8_t Comodo_button;
	
	Turn_on_ADC();
	Luminosity=Read_luminosity_sensor();
	Turn_off_ADC();
	
	Comodo_button=Read_comodo_button();
	
	if((Luminosity>>MIN_LUMINOSITY)||Comodo_button){
		// go to normal mode
		GoModeRUN0();
		mode_BCM=NORMAL_MODE;
	}
	
}

void Failsoft_mode_BCM(void){
	uint16_t voltage_value;
	voltage_value=Read_voltage_value();
	
	if(voltage_value>MIN_VOLTAGE_NORMAL_MODE){
		//read and send light value
		Update_light();
	}
	else{
		// go to normal mode
		GoModeRUN0();
		mode_BCM=NORMAL_MODE;
	}
	
}


void scheduler_BCM(void){
	switch (mode_BCM){
	
	case NORMAL_MODE :
		Normal_mode_BCM();
		break;
	case LOW_POWER_MODE :
		Low_power_mode_BCM();
		break;
	case FAILSOFT_MODE :
		Failsoft_mode_BCM();
		break;
	default: 
		Normal_mode_BCM();
				
	}
}
