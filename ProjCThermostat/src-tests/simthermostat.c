/**
 * @file simthermostat.c
 * $Id$
 *
 * Created on: 1 oct. 2012
 * @author Christophe TOMBELLE
 */
#include <stdlib.h>

#include "appalloc.h"
#include "simthermostat.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES PROGRAMMES DE TEST
 * ***************************************
 */

typedef struct Thermostat_ {
	IProgram _prga[8];
	int _iProg, _iNightThresh, _iDayThresh, _iTemp, _iSimDay, _iSimProg;
	/** Chaudière */
	IBoiler _boiler;
	/** Horloge hebdomadaire */
	IClock _clock;
} *Thermostat;

/**
 * Créer un Thermostat simulé.
 *
 * @param boiler Le Boiler de ce Thermostat.
 * @param sensor Le Sensor de ce Thermostat.
 * @param clock La Clock de ce Thermostat.
 * @return L'adresse du Thermostat créé.
 */
IThermostat createSimThermostat( IBoiler boiler, ISensor sensor, IClock clock ) {
	int i;
	IThermostat thrm = (IThermostat) appalloc(sizeof(struct Thermostat_), "simth_alloc");
	Thermostat therm = (Thermostat) thrm;
	therm->_boiler = boiler;
	therm->_clock = clock;
	therm->_prga[0] = NULL; // pas de programme car hors-gel
	for (i=1; i<8; i++)
		therm->_prga[i] = createProgram();
	for (i=0; i<24; i++)
		progSetChoice(therm->_prga[7], true, i );
	progSetFixed(therm->_prga[1]);
	progSetFixed(therm->_prga[7]);
	therm->_iDayThresh=0;
	therm->_iNightThresh=0;
	therm->_iProg=0;
	therm->_iSimDay=0;
	therm->_iSimProg=0;
	therm->_iTemp=0;
	return thrm;
}

/**
 * Fournir le numéro du programme en fonction du jour de la semaine
 * (attention à ne pas confondre thermGetProg et thermGetProgram).
 *
 * @param thrm Ce Thermostat
 * @param iDay Le jour de la semaine [0-6] avec 0 pour Dimanche.
 * @return Le numéro de programme [0-7].
 */
int thermGetProg(IThermostat thrm, int iDay) {
	Thermostat therm = (Thermostat) thrm;
	return therm->_iProg;
}

/**
 * Simuler le programme du jour.
 *
 * @param thrm Ce thermostat
 * @param iProg Numéro du programme à simuler.
 */
void thermSimProg(IThermostat thrm, int iProg ) {
	Thermostat therm = (Thermostat) thrm;
	therm->_iProg = iProg;
}

/**
 * Fournir le numéro de programme du jour.
 *
 * @param thrm Ce Thermostat.
 * @return Le numéro de programme du jour [0-7].
 */
int thermGetCurrentProg(IThermostat thrm) {
	Thermostat therm = (Thermostat) thrm;
	return therm->_iProg;
}

/**
 * Fournir le Program correspondant au numéro spécifié (attention à
 * ne pas confondre thermGetProg et thermGetProgram)
 *
 * @param thrm Ce Thermostat.
 * @param iProg Le numéro de programme [0-7].
 * @return Le Program.
 */
IProgram thermGetProgram(IThermostat thrm, int iProg ) {
	Thermostat therm = (Thermostat) thrm;
	return therm->_prga[iProg];
}

/**
 * Fournir la consigne de température "jour" ou "nuit".
 *
 * @param thrm Ce Thermostat.
 * @param bDay false pour obtenir la consigne "nuit", true pour obtenir
 * la consigne "jour".
 * @return La consigne de température en dixièmes de degré Celsius.
 */
int thermGetThreshold(IThermostat thrm, bool bDay) {
	Thermostat therm = (Thermostat) thrm;
	return bDay ? therm->_iDayThresh : therm->_iNightThresh;
}

/**
 * Définir le numéro de programme pour un jour donné.
 *
 * @param thrm Ce Thermostat.
 * @param iProg Le numéro de programme [0-7].
 * @param iDay Le jour de la seimaine [0-6] avec 0 pour Dimanche.
 */
void thermSetProg(IThermostat thrm, int iProg, int iDay ) {
	Thermostat therm = (Thermostat) thrm;
	therm->_iSimProg = iProg;
	therm->_iSimDay = iDay;
}

/**
 * Fournir le dernier numéro de programme défini pour un jour donné.
 *
 * @param thrm Ce Thermostat.
 * @return Le numéro de programme [0-7].
 */
int thermGetSimProg(IThermostat thrm) {
	Thermostat therm = (Thermostat) thrm;
	return therm->_iSimProg;
}

/**
 * Fournir le dernier jour en date affecté d'un numéro de programme.
 *
 * @return Le jour [0-6].
 */
int thermGetSimDay(IThermostat thrm) {
	Thermostat therm = (Thermostat) thrm;
	return therm->_iSimDay;
}

/**
 * Commander l'allumage ou l'extension de la chaudière en fonction des
 * valeurs comparées de la température et de la consigne de température
 * actuelles.
 *
 * @param thrm Ce Thermostat.
 */
void thermRegulate(IThermostat therm) {
}

/**
 * Définir la consigne de température "jour" ou "nuit".
 *
 * @param thrm Ce Thermostat.
 * @param bDay false pour définir la consigne "nuit", true pour définir
 * la consigne "jour".
 * @param iTemp La consigne de température en dixièmes de degré Celsius.
 */
void thermSetThreshold(IThermostat thrm, int iTemp, bool bDay ) {
	Thermostat therm = (Thermostat) thrm;
	if (bDay)
		therm->_iDayThresh = iTemp;
	else
		therm->_iNightThresh = iTemp;
}

/**
 * Fournir la Clock de ce Thermostat.
 *
 * @param thrm Ce Thermostat.
 * @return La Clock.
 */
IClock thermGetClock(IThermostat thrm) {
	Thermostat therm = (Thermostat) thrm;
	return therm->_clock;
}

/**
 * Fournir la température acutelle de la pièce.
 *
 * @param thrm Ce Thermostat.
 * @return La température actuelle en dixièmes de degré Celsius.
 */
int thermGetTemp(IThermostat thrm) {
	Thermostat therm = (Thermostat) thrm;
	return therm->_iTemp;
}

/**
 * Simuler la température ambiante.
 *
 * @param thrm Ce Thermostat.
 * @param iTemp La température.
 */
void thermSimTemp(IThermostat thrm, int iTemp ) {
	Thermostat therm = (Thermostat) thrm;
	therm->_iTemp = iTemp;
}

/**
 * Fournir l'état de fonctionnement de la chaudière.
 *
 * @param thrm Ce Thermostat.
 * @return true si la chaudière chauffe, false si elle est
 * éteinte.
 */
bool thermIsBoilerOn(IThermostat thrm) {
	Thermostat therm = (Thermostat) thrm;
	return boilerIsOn(therm->_boiler);
}

