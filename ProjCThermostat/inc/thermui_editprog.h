/*
 * thermui_editprog.h
 *
 *  Created on: 18 oct. 2012
 *      Author: betry
 */

#ifndef THERMUI_EDITPROG_H_
#define THERMUI_EDITPROG_H_

#include "display.h"
#include "thermostat.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES FICHIERS D'EN-TETE
 * ***************************************
 */

void createThermUIEditProg(IThermostat therm, IDisplay disp);
void uipEntering(void);
void uipProcessKey(int iKey);
void uipUpdate(void);
void uipDispSlot(int iHour, int iAt);

#endif /* THERMUI_EDITPROG_H_ */
