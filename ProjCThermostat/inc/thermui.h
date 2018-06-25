/*
 * @file thermui.h
 *
 * Created on: 11 juil. 2012
 * @author Christophe TOMBELLE
 */

#ifndef THERMUI_H_
#define THERMUI_H_

#include "display.h"
#include "thermostat.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES FICHIERS D'EN-TETE
 * ***************************************
 */

#define NORMAL 0
#define EDIT_CONS 1
#define EDIT_PROG 2

void createThermUI(IThermostat therm, IDisplay disp);
void uiProcessKey(int iKey);
void uiUpdate(void);
void uiSetMode(int mode);
int uiGetMode(void);

#endif /* THERMUI_H_ */
