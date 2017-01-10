/*
 * define.h
 *
 *  Created on: Nov 24, 2016
 *      Author: Julien
 */

#ifndef DEFINE_H_
#define DEFINE_H_

#include "MPC5604B.h"
#define AD_PIN PB_4
/*
 * Application global declarations. Must at the end replace config.h. Functions of main.c must be declared here.
 */

/* windows state definition */
#define STOPPED		0
#define UP			1
#define DOWN		2
#define CLOSED		3
#define OPEN		4
#define UNKNOW		5

extern uint8_t  window_position;
extern uint8_t window_state;

/*
 * Test functions
 */




#endif /* DEFINE_H_ */
