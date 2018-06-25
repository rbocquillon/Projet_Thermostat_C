/*
 * thermui_editcons.h
 *
 *  Created on: 18 oct. 2012
 *      Author: betry
 */

#ifndef THERMUI_EDITCONS_H_
#define THERMUI_EDITCONS_H_

#include "display.h"
#include "thermostat.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES FICHIERS D'EN-TETE
 * ***************************************
 */

void createThermUIEditCons(IThermostat therm, IDisplay disp);
void uicEntering(void);
void uicProcessKey(int iKey );
void uicUpdate(void);

#endif /* THERMUI_EDITCONS_H_ */
