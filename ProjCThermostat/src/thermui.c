/**
 * @file thermui.c
 * L'interface utilisateur du thermostat ; chargée de réagir aux événements
 * clavier et mettre à jour l'affichage du LCD. La réaction et l'affichage ne
 * sont pas les mêmes selon le mode. Il y a 3 modes "normal", "édition de
 * consigne" et "édition de programme". Le module ThermUI délègue aux modules
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
 * Réagir à l'événement clavier en fonction du mode.
 *
 * @param iKey Le code de touche de cet événement.
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
 * Mettre à jour l'affichage en fonction du mode.
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
 * Définir le mode.
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
 * Créer le ThermUI avec ses 3 modes.
 *
 * @param therm Le Thermostat à utiliser.
 * @param disp Le Display à utiliser.
 */
void createThermUI(IThermostat therm, IDisplay disp) {
	_mode = NORMAL;
	createThermUINormal(therm, disp);
	createThermUIEditCons(therm, disp);
	createThermUIEditProg(therm, disp);
}
