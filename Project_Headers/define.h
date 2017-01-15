/*
 * define.h
 *
 *  Created on: Nov 24, 2016
 *      Author: Julien
 */

#ifndef DEFINE_H_
#define DEFINE_H_

#define DCM

#include "MPC5604B.h"

/*
 * Application global declarations. Must at the end replace config.h. Functions of main.c must be declared here.
 */
#ifdef DCM

/* PIT utilization */
#define PIT_ADC			3  // can't be changed !
#define PIT_WTCH_TEMPO	1 // PIT used for tempo between motor start and start current monitoring
#define PIT_MODE_W		2 // PIT used to select automatic or manual mode. 

/* DCM pin mapping */

// window H-bridge 
#define IN1_W		PC_4
#define IN2_W		PC_5
#define EN_W		PC_6
#define D2_W		PC_7
#define FS_W		PC_8
#define FB_W		PB_4

// window buttons
#define BUTTON_UP	PA_6		
#define BUTTON_DOWN PA_7


/* DCM ISR priority */

#define WTCH_ISR_PRIORITY		10
#define EOCTU_ISR_PRIORITY		5
#define PIT_WTCH_TEMPO_PRIORITY 6
#define EIRQ0_PRIORITY			4





/* windows state definition */
#define STOPPED		0
#define UP			1
#define DOWN		2
#define CLOSED		3
#define OPEN		4
#define UNKNOW		5

extern uint8_t window_position;
extern uint8_t window_state;

#endif
/*
 * Test functions
 */




#endif /* DEFINE_H_ */
