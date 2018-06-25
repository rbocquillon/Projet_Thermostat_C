/**
 * @file thermostat.c
 * Le thermostat muet (hors interface utilisateur).
 *
 * Created on: 11 juil. 2012
 * @author Christophe TOMBELLE
 */
#include "appalloc.h"
#include "thermostat.h"

#define PROG_COUNT 8

struct Thermostat {

	IBoiler boiler;
	IClock horloge;
	ISensor sensor;
	IProgram programme[8];
	int numeroprog[8];
	int consignenuit;
	int consignejour;
};

/**
 * Créer un Thermostat.
 *
 * @param boiler Le Boiler de ce Thermostat.
 * @param sensor Le Sensor de ce Thermostat.
 * @param clock La Clock de ce Thermostat.
 * @return L'adresse du Thermostat créé.
 */
IThermostat createThermostat(IBoiler boiler, ISensor sensor, IClock clock) {
	int i;
	IThermostat therm = malloc(sizeof(struct Thermostat));
	for (i=0;i < 8; i++){
			therm->numeroprog[i] = 0;
		}
	therm->programme[0] = NULL;
	for(i=1;i<8;i++){
			therm->programme[i] = createProgram();
		}
	for(i = 0; i<24; i++){
			progSetChoice(therm->programme[7] , true, i);
		}
	progSetFixed(therm->programme[1]);
	progSetFixed(therm->programme[7]);

	therm->boiler = boiler;
	therm->horloge = clock;
	therm->sensor = sensor;
	therm->consignenuit = 120;
	therm->consignejour = 180;

	return therm;
}

/**
 * Fournir l'état de fonctionnement de la chaudière.
 *
 * @param therm Ce Thermostat.
 * @return true si la chaudière chauffe, false si elle est
 * éteinte.
 */
bool thermIsBoilerOn(IThermostat therm) {

	return boilerIsOn(therm->boiler);
}

/**
 * Fournir la température acutelle de la pièce.
 *
 * @param therm Ce Thermostat.
 * @return La température actuelle en dixièmes de degré Celsius.
 */
int thermGetTemp(IThermostat therm) {
	return sensorGetTemp(therm->sensor);
}

/**
 * Fournir la Clock de ce Thermostat.
 *
 * @param therm Ce Thermostat.
 * @return La Clock.
 */
IClock thermGetClock(IThermostat therm) {
	return therm->horloge;
}

/**
 * Fournir le numéro du programme en fonction du jour de la semaine
 * (attention à ne pas confondre thermGetProg et thermGetProgram).
 *
 * @param therm Ce Thermostat
 * @param iDay Le jour de la semaine [0-6] avec 0 pour Dimanche.
 * @return Le numéro de programme [0-7].
 */
int thermGetProg(IThermostat therm, int iDay) {
	return therm->numeroprog[iDay];
}

/**
 * Définir le numéro de programme pour un jour donné.
 *
 * @param therm Ce Thermostat.
 * @param iProg Le numéro de programme [0-7].
 * @param iDay Le jour de la seimaine [0-6] avec 0 pour Dimanche.
 */
void thermSetProg(IThermostat therm, int iProg, int iDay) {
	therm->numeroprog[iDay] = iProg;
}

/**
 * Fournir le numéro de programme du jour.
 *
 * @param therm Ce Thermostat.
 * @return Le numéro de programme du jour [0-7].
 */
int thermGetCurrentProg(IThermostat therm) {
	return therm->numeroprog[clkGetDay(therm->horloge)];
}

/**
 * Fournir le Program correspondant au numéro spécifié (attention à
 * ne pas confondre thermGetProg et thermGetProgram)
 *
 * @param therm Ce Thermostat.
 * @param iProg Le numéro de programme [0-7].
 * @return Le Program.
 */
IProgram thermGetProgram(IThermostat therm, int iProg) {
	return therm->programme[iProg];
}

/**
 * Fournir la consigne de température "jour" ou "nuit".
 *
 * @param therm Ce Thermostat.
 * @param bDay false pour obtenir la consigne "nuit", true pour obtenir
 * la consigne "jour".
 * @return La consigne de température en dixièmes de degré Celsius.
 */
int thermGetThreshold(IThermostat therm, bool bDay) {
	if(bDay == false)
	return therm->consignenuit;
	else return therm->consignejour;
}

/**
 * Définir la consigne de température "jour" ou "nuit".
 *
 * @param therm Ce Thermostat.
 * @param bDay false pour définir la consigne "nuit", true pour définir
 * la consigne "jour".
 * @param iTemp La consigne de température en dixièmes de degré Celsius.
 */
void thermSetThreshold(IThermostat therm, int iTemp, bool bDay) {
	if(bDay == false)
		therm->consignenuit = iTemp;
		else therm->consignejour = iTemp;
}

/**
 * Commander l'allumage ou l'extension de la chaudière en fonction des
 * valeurs comparées de la température et de la consigne de température
 * actuelles.
 *
 * @param therm Ce Thermostat.
 */
void thermRegulate(IThermostat therm) {
	int currentprog = thermGetCurrentProg(therm);
	IClock clock = thermGetClock(therm);
	int hour = clkGetHour(clock);

	if(currentprog == 0){
		if(sensorGetTemp(therm->sensor) < 50)
			boilerSetOn(therm->boiler, true);
		else
			boilerSetOn(therm->boiler, false);
	}
	else if(progIsDay(therm->programme[currentprog],hour)){
		if(sensorGetTemp(therm->sensor) < therm->consignejour)
					boilerSetOn(therm->boiler, true);
				else
					boilerSetOn(therm->boiler, false);
	}
	else{
		if(sensorGetTemp(therm->sensor) < therm->consignenuit)
			boilerSetOn(therm->boiler, true);
		else
			boilerSetOn(therm->boiler, false);
	}
}
