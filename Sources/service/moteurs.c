/*
 * moteurs.c
 *
 *  Created on: Jan 15, 2017
 *      Author: Julien, Taha
 */

#include "MPC5604B.h" 
#include "pwm.h" 
#include "moteurs.h"
#include "pin.h" 

/*
 * Initialisation PWM pour moteurs
 */
void init_PWM_moteurs()
{
	 initEMIOS_0(); /* Initialize eMIOS 0 to provide 1 MHz clock to channels */
	 initEMIOS_0ch23(); /* Initialize eMIOS 0 channel 23 as modulus counter*/
	 initEMIOS_0ch21(500); /* Initialize eMIOS 0 channel 21 as OPWM, using ch 23 as time base */
	 initEMIOS_0ch22(); /* Initialize eMIOS 0 channel 22 as OPWM, using ch 23 as time base */
}

void init_pins_moteurs()
{
	 Configure_to_DO(PA_0); //PA_0 is IN2 for the 1st H-bridg
	 Configure_to_DO(PA_1); //PA_1 is the ENABLE for the 1st H-bridg
	 Configure_to_DO(PA_2); //PA_2 is IN2 for the 2nd H-bridge
	 Configure_to_DO(PA_3); //PA_3 is the ENABLE for the 2nd H-bridge
}
