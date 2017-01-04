/*
 * mode_manager.h
 *
 *  Created on: Jan 4, 2017
 *      Author: EL MEHDI
 */

#ifndef MODE_MANAGER_H_
#define MODE_MANAGER_H_

void initialisation_du_mode_et_du_clock(void);
void aller_au_mode_RUN0(void);
void enlever_watchdog(void);
void faire_fonctionner_l_interuption(void);
void arreter_l_interuption(void);
void aller_au_mode_STOP(void);
void initialisation_du_timer_PIT(uint32_t Timeout );
void PIT_actif(void);
void interruption(void);
void PAD_S_initialisation(void);

#endif /* MODE_MANAGER_H_ */
