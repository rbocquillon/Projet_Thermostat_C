/*
 * @file thermostat.h
 *
 * Created on: 11 juil. 2012
 * @author Christophe TOMBELLE
 */

#ifndef THERMOSTAT_H_
#define THERMOSTAT_H_
#include <stdbool.h>


#include "clock.h"
#include "boiler.h"
#include "sensor.h"
#include "program.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES FICHIERS D'EN-TETE
 * ***************************************
 */

typedef struct Thermostat *IThermostat;

IThermostat createThermostat(IBoiler boiler, ISensor sensor, IClock clock);

int thermGetProg(IThermostat therm, int iDay);
int thermGetCurrentProg(IThermostat therm);
void thermSetProg(IThermostat therm, int iProg, int iDay);
int thermGetTemp(IThermostat therm);
IProgram thermGetProgram(IThermostat therm, int iProg);
int thermGetThreshold(IThermostat therm, bool bDay);
void thermSetThreshold(IThermostat therm, int iTemp, bool bDay);
IClock thermGetClock(IThermostat therm);
void thermRegulate(IThermostat therm);
bool thermIsBoilerOn(IThermostat therm);

#endif /* THERMOSTAT_H_ */
