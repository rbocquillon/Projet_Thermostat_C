/**
 * @file thermui_normalTest.c
 * $Id$
 *
 * Created on: 1 oct. 2012
 * @author Christophe TOMBELLE
 */
#include <stdbool.h>
#include <stdio.h>

#include "appalloc.h"
#include "thermui_normalTest.h"
#include "thermui.h"

#include "keyboard.h"
#include "simport.h"
#include "boiler.h"
#include "sensor.h"
#include "simclock.h"
#include "simthermostat.h"
#include "simdisplay.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES PROGRAMMES DE TEST
 * ***************************************
 */

/* objets partenaires de l'objet sous test */
static IPort _portLedsDip;
static IBoiler _boiler;
static IPort _portCAN;
static ISensor _sensor;
static IClock _clock;
static IThermostat _therm;
static IDisplay _disp;


/**
 * Pr�paration des objets partenaires de l'objet test�.
 */
static void setUp() {
	_portLedsDip = createSimPort();
	_boiler = createBoiler(_portLedsDip);
	_portCAN = createSimPort();
	portSimInput(_portCAN, 0);
	_sensor = createSensor(_portCAN);
	_clock = createSimClock();
	_therm = createSimThermostat(_boiler, _sensor, _clock);
	_disp = createSimDisplay();
}

/**
 * Lib�ration des objets partenaires de l'objet test�.
 */
static void tearDown() {
	free(_portLedsDip);
	free(_boiler);
	free(_portCAN);
	free(_sensor);
	free(_clock);
	free(_therm);
	free(_disp);
	_portLedsDip = NULL;
	_boiler = NULL;
	_portCAN = NULL;
	_sensor = NULL;
	_clock = NULL;
	_therm = NULL;
	_disp = NULL;
}

/**
 * Afficher quelque chose sur l'afficheur.
 */
static void testNormalMode(CuTest* tc) {
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	uiUpdate();
	char * str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Diman. 00,0�C00:00:00 H. Gel ", str);
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/**
 * Afficher le num�ro du programme du jour.
 */
static void testDisplayProgram(CuTest* tc) {
	char *str;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	/* simuler le num�ro de programme 1 ce jour */
	thermSimProg(_therm, 1);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P1 Diman. 00,0�C00:00:00 Nuit   ", str);
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/**
 * Afficher le jour de la semaine.
 */
static void testDisplayDay(CuTest* tc) {
	char *str;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	/* Simuler aujourd'hui c'est lundi */
	clkSetTime(_clock, MS_PER_DAY);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Lundi  00,0�C00:00:00 H. Gel ", str);
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/**
 * Afficher la temp�rature ambiante.
 */
static void testDisplayTemperature(CuTest* tc) {
	char *str;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	/* simuler la temp�rature ambiante de 1�C */
	thermSimTemp(_therm, 10);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Diman. 01,0�C00:00:00 H. Gel ", str);
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/**
 * Afficher l'heure qu'il est.
 */
static void testDisplayHour(CuTest* tc) {
	char *str;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	/* Simuler "il est 1 heure" */
	clkSetTime(_clock, MS_PER_HOUR);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Diman. 00,0�C01:00:00 H. Gel ", str);
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/**
 * Afficher la minute actuelle.
 */
static void testDisplayMinute(CuTest* tc) {
	char *str;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	/* Simuler "il est 0 heure et 1 minute" */
	clkSetTime(_clock, MS_PER_MINUTE);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Diman. 00,0�C00:01:00 H. Gel ", str);
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/**
 * Afficher la seconde actuelle.
 */
static void testDisplaySecond(CuTest* tc) {
	char *str;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	/* Simuler "il est 0 heure et 1 seconde" */
	clkSetTime(_clock, 1000);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Diman. 00,0�C00:00:01 H. Gel ", str);
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/**
 * Afficher un type de consigne "Jour".
 */
static void testDisplayThreshKind(CuTest* tc) {
	IProgram prog;
	char *str;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	/* Simuler programme 2 ce jour */
	thermSimProg(_therm, 2);
	prog = thermGetProgram(_therm, 2);
	/* avec consigne "Jour" � 00:00 */
	progSetChoice(prog, true, 0);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P2 Diman. 00,0�C00:00:00 Jour   ", str);
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/**
 * Afficher l'activit� de la chaudi�re.
 */
static void testDisplayBoilerActivity(CuTest* tc) {
	char *str;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	/* simuler chaudi�re allum�e */
	boilerSetOn(_boiler, true);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Diman. 00,0�C00:00:00 H. Gel*", str);
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/**
 * Ajuster l'horloge au jour actuel.
 */
static void testProcessDayKey(CuTest* tc) {
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	uiProcessKey(DAY);
	IClock clock = thermGetClock(_therm);
	CuAssertIntEquals(tc, MS_PER_DAY, clkGetTime(clock));
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/**
 * Ajuster l'horloge � l'heure actuelle.
 */
static void testProcessHourKey(CuTest* tc) {
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	uiProcessKey(HOUR);
	IClock clock = thermGetClock(_therm);
	CuAssertIntEquals(tc, MS_PER_HOUR, clkGetTime(clock));
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/**
 * Ajuster l'horloge � a minute actuelle.
 */
static void testProcessMinuteKey(CuTest* tc) {
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	uiProcessKey(MINUTE);
	IClock clock = thermGetClock(_therm);
	CuAssertIntEquals(tc, MS_PER_MINUTE, clkGetTime(clock));
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/**
 * Passer en mode �dition de consigne.
 */
static void testNormal2EditCons(CuTest* tc) {
	int mode;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	/* demander � passer en mode �dition de consigne */
	uiProcessKey(TEMP);
	mode = uiGetMode();
	CuAssertIntEquals(tc, EDIT_CONS, mode);
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/**
 * Passer en mode �dition de programmes.
 */
static void testNormal2EditProg(CuTest* tc) {
	int mode;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	/* demander � passer en mode �dition de programmes */
	uiProcessKey(PROG);
	mode = uiGetMode();
	CuAssertIntEquals(tc, EDIT_PROG, mode);
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

CuSuite* thermui_normalSuite() {
	CuSuite* suite = CuSuiteNew();
	/* add test functions */
	SUITE_ADD_TEST(suite, testNormalMode);
	SUITE_ADD_TEST(suite, testDisplayProgram);
	SUITE_ADD_TEST(suite, testDisplayDay);
	SUITE_ADD_TEST(suite, testDisplayTemperature);
	SUITE_ADD_TEST(suite, testDisplayHour);
	SUITE_ADD_TEST(suite, testDisplayMinute);
	SUITE_ADD_TEST(suite, testDisplaySecond);
	SUITE_ADD_TEST(suite, testDisplayThreshKind);
	SUITE_ADD_TEST(suite, testDisplayBoilerActivity);
	SUITE_ADD_TEST(suite, testProcessDayKey);
	SUITE_ADD_TEST(suite, testProcessHourKey);
	SUITE_ADD_TEST(suite, testProcessMinuteKey);
	SUITE_ADD_TEST(suite, testNormal2EditCons);
	SUITE_ADD_TEST(suite, testNormal2EditProg);
	return suite;
}

