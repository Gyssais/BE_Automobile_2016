/*
 * locker.c
 *
 *  Created on: Jan 16, 2017
 *      Author: Julien
 */

#include "locker.h"
#include "gpio.h"
#include "pwm.h"

void init_locker()
{
	pinMode(PA_2, OUTPUT); // IN2 for H-bridge of lock
	pinMode(PA_3, OUTPUT); // ENABLE for H-bridge of lock
	digitalWrite(PA_3, 0); //Disable H-baridge
	init_PWM_0();
	start_PWM_0(22, 50); // Rapport cyclique de 50%
}

void lock_door()
{
	digitalWrite(PA_3,1); 
	digitalWrite(PA_2,0); // PA_7 dans fichier Taha -> erreur ?
}

void unlock_door()
{
	digitalWrite(PA_3,1); 
	digitalWrite(PA_2,1); 
}

void stop_locker()
{
	digitalWrite(PA_3,0); 
	digitalWrite(PA_2,0);
	// Programme Taha : Write_to_output(PE_7,1);Write_to_output(PE_4,1); -> Utilité ?, erreur ?
}
