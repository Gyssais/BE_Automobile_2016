/*
 * global.c
 *
 *  Created on: Nov 25, 2016
 *      Author: Julien
 */
#include "define.h"
#include "MPC5604B.h"

// Pour debug
uint8_t LED_status=0;

#ifdef DCM
uint8_t window_position = UNKNOW;
uint8_t window_state = STOPPED;
#endif


