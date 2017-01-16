/*
 * pwm.h
 *
 *  Created on: Jan 14, 2017
 *      Author: Julien, Taha
 */

#ifndef PWM_H_
#define PWM_H_

/******* Fonctions Taha *****************/
// TODO A supprimer une fois nouvelles fonctions validées
void initEMIOS_0(void);
void initEMIOS_0ch23(void);
void initEMIOS_0ch21(int x);  //x varie de 0 à 999 telque le rapport cyclique serait x/1000
void initEMIOS_0ch22(void);


/*
 * Initialize EMIOS0 at 1MHz. All channel of PWM0 have the same frequency.
 */
void init_PWM_0();

/*
 * Initialize a channel at a given duty cycle (0 to 100)
 */
void start_PWM_0(uint8_t channel, uint8_t duty_cycle);




#endif /* PWM_H_ */
