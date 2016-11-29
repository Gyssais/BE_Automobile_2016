/*
 * config.h
 *
 *  Created on: Jan 18, 2015
 *      Author: Antoine
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define BCM // BCM or LCM

// lights definiton
#define BIT_NAVIGATION_LIGHTS 		0x01
#define BIT_DIMMED_HEADLIGHT 		0x02
#define BIT_HEADLIGHT 				0x04
#define BIT_FLASHING_LIGHT_RIGHT 	0x08
#define BIT_FLASHING_LIGHT_LEFT 	0x10
#define BIT_DIRECTION_LIGHT_RIGHT 	0x20
#define BIT_DIRECTION_LIGHT_LEFT 	0x40

//button definition
#define BIT_BUTTON_NAVIGATION			0x01
#define BIT_BUTTON_DIMMED_HEADLIGHT		0x02
#define BIT_BUTTON_HEADLIGHT			0x04
#define BIT_BUTTON_FLASHING_LIGHT_RIGHT 0x08
#define BIT_BUTTON_FLASHING_LIGHT_LEFT	0x10

//definition CAN id
#define ID_CAN_BCM 	0b01000000001
#define ID_CAN_LCM	0b01000000010

#define LIGHT_MESSAGE 0x33
#define CURRENT_MESSAGE 0x44
#define SWITCH_MESSAGE 0x55


//Board led definition
#define PIN_LED_1 0x01000000 // PIN 1 for state blinking
#define PIN_LED_2 0x02000000
#define PIN_LED_3 0x04000000
#define PIN_LED_4 0x08000000

#define NORMAL_MODE 		0x01
#define LOW_POWER_MODE 		0x02
#define FAILSOFT_MODE 		0x03

#define LISTEN_ONLY_TIME	 20 //number of cycle before going to Listen only mode
#define SLEEP_TIME			 100//number of cycle before going to Sleep mode
#define MIN_LUMINOSITY 		0x0F
#define THRESHOLD_LUMINOSITY 	925
#define THRESHOLD_LEFT_HIGHT 			100
#define THRESHOLD_RIGHT_HIGHT 		310
#define THRESHOLD_LEFT_LOW 			80
#define THRESHOLD_RIGHT_LOW 		270
#define DIRECTION_RIGHT			1
#define DIRECTION_LEFT			2
#define MIN_VOLTAGE_NORMAL_MODE 0x1F



#endif /* CONFIG_H_ */
