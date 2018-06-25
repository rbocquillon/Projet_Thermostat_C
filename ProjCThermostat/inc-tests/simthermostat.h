/**
 * @file simthermostat.h
 * $Id$
 *
 * Created on: 1 oct. 2012
 * @author Christophe TOMBELLE
 */

#ifndef SIMTHERMOSTAT_H_
#define SIMTHERMOSTAT_H_
#include <stdbool.h>

#include "thermostat.h"
#include "simclock.h"
#include "program.h"
#include "boiler.h"
#include "sensor.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES FICHIERS D'EN-TETE
 * ***************************************
 */

IThermostat createSimThermostat( IBoiler _boiler, ISensor _sensor, IClock _clock );

int thermGetSimProg(IThermostat therm);
int thermGetSimDay(IThermostat therm);
void thermSimProg(IThermostat therm, int iProg );
void thermSimTemp(IThermostat therm, int iTemp );

#endif /* SIMTHERMOSTAT_H_ */
