/**
 * @file time.c
 * $Id$
 *
 * Simule le temps système fourni par le timer hardware.
 *
 * Created on: 13 juil. 2012
 * @author Christophe TOMBELLE
 */

#include "appalloc.h"
#include "simtime.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES PROGRAMMES DE TEST
 * ***************************************
 */

static long long _simNow = 0;

/**
 * Fournir le nombre de millisecondes écoulées depuis un instant de
 * référence.
 * @return Nombre de millisecondes.
 */
static long long _now() {
	return _simNow;
}

long long (* now)(void) = _now;

/**
 * Simuler le temps écoulé depuis l'instant de référence.
 */
void setNow(long long lNow) {
	_simNow = lNow;
}

/**
 * Faire passer le temps de la durée indiquée.
 * @param lDuration Durée à faire passer.
 */
void elapseLong(long lDuration) {
	setNow(now() + lDuration);
}

/**
 * Convertir en millisecondes la durée indiquée par les différentes
 * composantes.
 * @param iDay Nombre de jours [0-6]
 * @param iHour Nombre d'heures [0-23]
 * @param iMin Nombre de minutes [0-59]
 * @param iSec Nombre de secondes [0-59]
 * @param iMs Nombre de millisecondes [0-999]
 * @return Le nombre total de millisecondes
 */
static long toMs(int iDay, int iHour, int iMin, int iSec, int iMs) {
	return ((((iDay * 24) + iHour) * 60 + iMin) * 60 + iSec) * 1000 + iMs;
}

/**
 * Faire passer le temps de la durée indiquée par les différentes
 * composantes.
 * @param iDay Nombre de jours
 * @param iHour Nombre d'heures [0-23]
 * @param iMin Nombre de minutes [0-59]
 * @param iSec Nombre de secondes [0-59]
 * @param iMs Nombre de millisecondes [0-999]
 */
void elapse(int iDay, int iHour, int iMin, int iSec, int iMs) {
	elapseLong(toMs(iDay, iHour, iMin, iSec, iMs));
}
