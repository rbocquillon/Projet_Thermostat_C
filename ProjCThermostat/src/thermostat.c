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
 * Cr�er un Thermostat.
 *
 * @param boiler Le Boiler de ce Thermostat.
 * @param sensor Le Sensor de ce Thermostat.
 * @param clock La Clock de ce Thermostat.
 * @return L'adresse du Thermostat cr��.
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
 * Fournir l'�tat de fonctionnement de la chaudi�re.
 *
 * @param therm Ce Thermostat.
 * @return true si la chaudi�re chauffe, false si elle est
 * �teinte.
 */
bool thermIsBoilerOn(IThermostat therm) {

	return boilerIsOn(therm->boiler);
}

/**
 * Fournir la temp�rature acutelle de la pi�ce.
 *
 * @param therm Ce Thermostat.
 * @return La temp�rature actuelle en dixi�mes de degr� Celsius.
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
 * Fournir le num�ro du programme en fonction du jour de la semaine
 * (attention � ne pas confondre thermGetProg et thermGetProgram).
 *
 * @param therm Ce Thermostat
 * @param iDay Le jour de la semaine [0-6] avec 0 pour Dimanche.
 * @return Le num�ro de programme [0-7].
 */
int thermGetProg(IThermostat therm, int iDay) {
	return therm->numeroprog[iDay];
}

/**
 * D�finir le num�ro de programme pour un jour donn�.
 *
 * @param therm Ce Thermostat.
 * @param iProg Le num�ro de programme [0-7].
 * @param iDay Le jour de la seimaine [0-6] avec 0 pour Dimanche.
 */
void thermSetProg(IThermostat therm, int iProg, int iDay) {
	therm->numeroprog[iDay] = iProg;
}

/**
 * Fournir le num�ro de programme du jour.
 *
 * @param therm Ce Thermostat.
 * @return Le num�ro de programme du jour [0-7].
 */
int thermGetCurrentProg(IThermostat therm) {
	return therm->numeroprog[clkGetDay(therm->horloge)];
}

/**
 * Fournir le Program correspondant au num�ro sp�cifi� (attention �
 * ne pas confondre thermGetProg et thermGetProgram)
 *
 * @param therm Ce Thermostat.
 * @param iProg Le num�ro de programme [0-7].
 * @return Le Program.
 */
IProgram thermGetProgram(IThermostat therm, int iProg) {
	return therm->programme[iProg];
}

/**
 * Fournir la consigne de temp�rature "jour" ou "nuit".
 *
 * @param therm Ce Thermostat.
 * @param bDay false pour obtenir la consigne "nuit", true pour obtenir
 * la consigne "jour".
 * @return La consigne de temp�rature en dixi�mes de degr� Celsius.
 */
int thermGetThreshold(IThermostat therm, bool bDay) {
	if(bDay == false)
	return therm->consignenuit;
	else return therm->consignejour;
}

/**
 * D�finir la consigne de temp�rature "jour" ou "nuit".
 *
 * @param therm Ce Thermostat.
 * @param bDay false pour d�finir la consigne "nuit", true pour d�finir
 * la consigne "jour".
 * @param iTemp La consigne de temp�rature en dixi�mes de degr� Celsius.
 */
void thermSetThreshold(IThermostat therm, int iTemp, bool bDay) {
	if(bDay == false)
		therm->consignenuit = iTemp;
		else therm->consignejour = iTemp;
}

/**
 * Commander l'allumage ou l'extension de la chaudi�re en fonction des
 * valeurs compar�es de la temp�rature et de la consigne de temp�rature
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
