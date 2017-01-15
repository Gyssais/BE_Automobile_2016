/*
 * MC33887.h
 *
 *  Created on: Jan 11, 2017
 *      Author: Theo
 */

#ifndef MC33887_H_
#define MC33887_H_

#include "define.h"
#include "driver/MPC5604B.h"

#define SENS1		0
#define SENS2		1

struct MC33887_pinout 
{
	uint8_t IN1;
	uint8_t IN2;
	uint8_t D2;
	uint8_t EN;
	uint8_t FS;
};
typedef struct MC33887_pinout MC33887_pinout;

void init_HBridge(const MC33887_pinout * pinout);
void stop_HBridge(const MC33887_pinout * pinout);
void start_HBridge(const MC33887_pinout * pinout, int sens);

#endif /* MC33887_H_ */
