/**
 * @file thermui.c
 * L'interface utilisateur du thermostat ; charg�e de r�agir aux �v�nements
 * clavier et mettre � jour l'affichage du LCD. La r�action et l'affichage ne
 * sont pas les m�mes selon le mode. Il y a 3 modes "normal", "�dition de
 * consigne" et "�dition de programme". Le module ThermUI d�l�gue aux modules
 * selon le mode actuel.
 *
 * Created on: 11 juil. 2012
 * @author Christophe TOMBELLE
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "thermui.h"
#include "thermui_normal.h"
#include "thermui_editcons.h"
#include "thermui_editprog.h"

static int _mode;

/**
 * R�agir � l'�v�nement clavier en fonction du mode.
 *
 * @param iKey Le code de touche de cet �v�nement.
 */
void uiProcessKey(int iKey) {
	switch (_mode) {
	case NORMAL :
		uinProcessKey(iKey);
		break;
	case EDIT_CONS :
		uicProcessKey(iKey);
		break;
	case EDIT_PROG :
		uipProcessKey(iKey);
		break;
	}
}

/**
 * Mettre � jour l'affichage en fonction du mode.
 */
void uiUpdate() {
	switch (_mode) {
	case NORMAL :
		uinUpdate();
		break;
	case EDIT_CONS :
		uicUpdate();
		break;
	case EDIT_PROG :
		uipUpdate();
		break;
	}
}

/**
 * D�finir le mode.
 *
 * @param mode Le nouveau mode.
 */
void uiSetMode(int mode) {
	_mode = mode;
	switch (_mode) {
	case NORMAL :
		uinEntering();
		break;
	case EDIT_CONS :
		uicEntering();
		break;
	case EDIT_PROG :
		uipEntering();
		break;
	}
}

/**
 * Fournir le mode.
 *
 * @return Le mode parmi NORMAL, EDIT_CONS, EDIT_PROG.
 */
int uiGetMode() {
	return _mode;
}

/**
 * Cr�er le ThermUI avec ses 3 modes.
 *
 * @param therm Le Thermostat � utiliser.
 * @param disp Le Display � utiliser.
 */
void createThermUI(IThermostat therm, IDisplay disp) {
	_mode = NORMAL;
	createThermUINormal(therm, disp);
	createThermUIEditCons(therm, disp);
	createThermUIEditProg(therm, disp);
}
