/*
 * moteurs.h
 *
 *  Created on: Jan 15, 2017
 *      Author: Julien
 */

#ifndef MOTEURS_H_
#define MOTEURS_H_

#include "MPC5604B.h"
#include "pin.h"

void init_PWM_moteurs();

void init_pins_moteurs();

void test_moteurs()
{
	//-------leve vitre--------//
	if (Read_input(PE_0) == 0)   
	{ 
		Write_to_output(PA_1,1);  
		Write_to_output(PA_0,0);   
		initEMIOS_0ch21(900);                           
	}     
	//------descente vitre---------//
	else if (Read_input(PE_1) == 0)  //descente vitre
	{
		Write_to_output(PA_1,1);
		Write_to_output(PA_0,1); 
		initEMIOS_0ch21(300);
	} 
	//----------verrouillage porte---------//
	else if (Read_input(PE_2) == 0)  //verrouillage porte
	{
		Write_to_output(PA_3,1);
		Write_to_output(PA_7,0);  
	} 
	//-----------déverrouillage------------//
	else if (Read_input(PE_3) == 0)  //deverrouillage porte
	{
		Write_to_output(PA_3,1);
		Write_to_output(PA_2,1); 
	} 
	//-----------Etat de repos----------------/
	else
	{
		Write_to_output(PA_1,0);
		Write_to_output(PA_3,0);  // les enables à 0
	    Write_to_output(PA_0,0);
	    Write_to_output(PA_2,0);  //les IN2 à 0 question de consommation d'energie
	    Write_to_output(PE_7,1);
	    Write_to_output(PE_4,1);
	}
}



#endif /* MOTEURS_H_ */
