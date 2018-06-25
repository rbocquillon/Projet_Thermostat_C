/**
 * @file simboard.c
 * $Id$
 *
 * Created on: 24 sept. 2013
 * @author Christophe TOMBELLE
 */
#include <stdlib.h>

#include "simhal.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES PROGRAMMES DE TEST
 * ***************************************
 */

static eic_isr _isr;

void installISR(eic_isr isr) {
	_isr = isr;
}

void uninstallISR(eic_isr isr) {
	if (isr == _isr)
		_isr = NULL;
}

void simISR() {
	if (_isr != NULL)
		_isr();
}

