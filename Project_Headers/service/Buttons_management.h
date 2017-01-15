#ifndef Buttons_H_
#define Buttons_H_

/* 
 * Initialisation de l'ADC pour lecture vitesse (potentiom�tre de la carte)
 */
void init_speed_button();

/*
 * Lecture vitesse via potentiom�tre
 */
uint16_t read_speed();

/*
 * Initialisation LEDs de la carte
 */
void init_LED();

/*
 * Initialisation boutons de la carte
 */
void init_buttons();


/*
 * Test bouton verouillage porti�re (BCM). Renvoie 1 si bouton 1 de la carte appuy�.
 */
uint8_t lock_door();

/*
 * Simulation d�tection pluie. Renvoie 1 si bouton 2 appuy�.
 */
uint8_t det_rain();

/*
 * Simulation test batterie faible. Renvoie 1 si bouton 3 de la carte appuy�.
 */
uint8_t bat_min();

/*
 * Test bouton 4
 */
uint8_t bouton4();

/*
 * Allume LED de la carte
 * arg : num�ro de la LED : 1 � 4
 */
void LED_on(uint8_t);

/*
 * Eteint LED de la carte
 * arg : num�ro de la LED : 1 � 4
 */
void LED_off(uint8_t);

/*
 * Fonction de test de boutons et LEDs : Bouton 1 -> Allume LED 1, B2 -> LED 2,...
 * N�cessaires : 	disableWatchdog(); et initModesAndClock(); d�but du programme
 */
 void test_buttons_management();
 void test_speed();
#endif Buttons_H_
