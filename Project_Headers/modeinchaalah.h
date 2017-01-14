/*
 * mode_inchaalah.h
 *
 *  Created on: Jan 13, 2017
 *      Author: EL MEHDI
 */

#include "MPC5604B.h"
#ifndef MODE_INCHAALAH_H_
#define MODE_INCHAALAH_H_



void aller_au_mode_RUN0(void);
void initialisation_du_mode_et_du_clock(void);
void enlever_watchdog(void);
void faire_fonctionner_l_interuption(void);
void aller_au_mode_STOP(void);
void initialisation_du_timer_PIT(uint32_t Timeout );
void PIT_actif(void);
void PIT_desactive(void);
void interruption(void);
//void arreterl_interuption(void);

//void reveilledemodestp(void);

#endif /* MODE_INCHAALAH_H_ */
