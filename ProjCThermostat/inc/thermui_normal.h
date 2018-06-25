/*
 * thermui_normal.h
 *
 *  Created on: 17 oct. 2012
 *      Author: betry
 */

#ifndef THERMUI_NORMAL_H_
#define THERMUI_NORMAL_H_

#include "display.h"
#include "thermostat.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES FICHIERS D'EN-TETE
 * ***************************************
 */

void createThermUINormal(IThermostat therm, IDisplay disp);
void uinEntering(void);
void uinProcessKey(int iKey );
void uinUpdate(void);

#endif /* THERMUI_NORMAL_H_ */
