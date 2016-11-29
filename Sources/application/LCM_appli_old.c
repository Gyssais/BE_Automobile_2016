#include "MPC5604B.h"
#include "IntcInterrupts.h"
#include "config.h"
#include "LCM_appli.h"
#include "Driver_phare.h"
#include "SBC.h"
#include "Mode_manager.h"

/*************** Static variable **************/
uint8_t mode_LCM=NORMAL_MODE;
uint8_t light_state_LCM=0;

// ************* Public Function

void Normal_mode_LCM(void){
	static uint8_t sleep_cpt;
	//if lights are on measure the current
	if(light_state_LCM!=0){
		//TO DO : add the lights
	}
	else{
		// if we didn't receive CAN message
		if((get_flag_frame_received()==0)){
				sleep_cpt++;
				if(sleep_cpt>LISTEN_ONLY_TIME){
					//sleep mode
					if(sleep_cpt>SLEEP_TIME){
						//mode_LCM=LOW_POWER_MODE;
						//GoModeRUN2();
						
					}
					//listen only mode
					else{
						
					}
				}
			}
		// if we receive a can message
		else{
			reset_flag_frame_received();
			sleep_cpt=0;
		}
	}
}


void Failsoft_mode_LCM(void){
	uint16_t voltage_value;
	
	voltage_value=Read_voltage_value();
	
	if(voltage_value>MIN_VOLTAGE_NORMAL_MODE){
		//read and send current value
		
	}
	else{
		// go to normal mode
		mode_LCM=NORMAL_MODE;
		GoModeRUN0();
	}
}

void scheduler_LCM(void){
	switch (mode_LCM){
		
		case NORMAL_MODE :
			Normal_mode_LCM();
			break;
		case FAILSOFT_MODE :
			Failsoft_mode_LCM();
			break;
		default: 
			Normal_mode_LCM();
					
		}
}
