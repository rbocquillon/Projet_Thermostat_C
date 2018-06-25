/**
 * @file thermui_editconsTest.c
 * $Id$
 *
 * Created on: 1 oct. 2012
 * @author Christophe TOMBELLE
 */
#include <stdbool.h>
#include <stdio.h>

#include "appalloc.h"
#include "thermui_editconsTest.h"
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
	_boiler = createBoiler((IPort) _portLedsDip);
	_portCAN = createSimPort();
	portSimInput(_portCAN, 0);
	_sensor = createSensor((IPort) _portCAN);
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
 * Afficher le num�ro du programme du jour.
 */
static void testDisplayProgram(CuTest* tc) {
	char *str;
	printf("%s\n", tc->name);
	setUp();
	createThermUI( _therm, _disp);
	thermSimProg(_therm, 1);
	uiSetMode(EDIT_CONS);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P1 Diman. 00,0�C00:00:00 Jour   ", str);
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
	uiSetMode(EDIT_CONS);
	clkSetTime(_clock, MS_PER_DAY);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Lundi  00,0�C00:00:00 Jour   ", str);
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/**
 * Afficher la temp�rature de consigne en cours d'�dition.
 */
static void testDisplayDayThresh(CuTest* tc) {
	char *str;
	printf("%s\n", tc->name);
	setUp();
	createThermUI( _therm, _disp);
	uiSetMode(EDIT_CONS);
	thermSetThreshold(_therm, 180, true);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Diman. 18,0�C00:00:00 Jour   ", str);
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
	uiSetMode(EDIT_CONS);
	/* Simuler "il est 1 heure" */
	clkSetTime(_clock, MS_PER_HOUR);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Diman. 00,0�C01:00:00 Jour   ", str);
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
	uiSetMode(EDIT_CONS);
	/* Simuler "il est 0 heure et 1 minute" */
	clkSetTime(_clock, MS_PER_MINUTE);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Diman. 00,0�C00:01:00 Jour   ", str);
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
	uiSetMode(EDIT_CONS);
	/* Simuler "il est 0 heure et 1 seconde" */
	clkSetTime(_clock, 1000);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Diman. 00,0�C00:00:01 Jour   ", str);
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/**
 * Pas d'�toile m�me si chaudi�re allum�e.
 */
static void testDisplayNoStar(CuTest* tc) {
	char *str;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	uiSetMode(EDIT_CONS);
	/* simuler une chaudi�re allum�e */
	boilerSetOn(_boiler, true);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Diman. 00,0�C00:00:00 Jour   ", str);
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/**
 * Changer de temp�rature de consigne "Jour" -> "Nuit".
 */
static void testDisplayDay2Night(CuTest* tc) {
	int iThresh;
	char *str;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	uiSetMode(EDIT_CONS);
	thermSetThreshold(_therm, 120, false);
	thermSetThreshold(_therm, 180, true);
	uiProcessKey(MOON);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Diman. 12,0�C00:00:00 Nuit   ", str);
	/* Les temp�ratures de consigne n'ont pas chang� */
	iThresh = thermGetThreshold(_therm, false);
	CuAssertIntEquals(tc, 120, iThresh);
	iThresh = thermGetThreshold(_therm, true);
	CuAssertIntEquals(tc, 180, iThresh);
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/**
 * Changer de temp�rature de consigne "Jour" -> "Nuit" -> "Jour".
 */
static void testDisplayNight2Day(CuTest* tc) {
	int iThresh;
	char *str;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	uiSetMode(EDIT_CONS);
	thermSetThreshold(_therm, 120, false);
	thermSetThreshold(_therm, 180, true);
	uiProcessKey(MOON);
	uiProcessKey(SUN);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Diman. 18,0�C00:00:00 Jour   ", str);
	/* Les temp�ratures de consigne n'ont pas chang� */
	iThresh = thermGetThreshold(_therm, false);
	CuAssertIntEquals(tc, 120, iThresh);
	iThresh = thermGetThreshold(_therm, true);
	CuAssertIntEquals(tc, 180, iThresh);
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/**
 * Incr�menter la temp�rature de consigne "Jour".
 */
static void testIncrDayThresh(CuTest* tc) {
	int iThresh;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	uiSetMode(EDIT_CONS);
	thermSetThreshold(_therm, 120, false);
	thermSetThreshold(_therm, 180, true);
	uiProcessKey(SUN);
	iThresh = thermGetThreshold(_therm, false);
	CuAssertIntEquals(tc, 120, iThresh);
	iThresh = thermGetThreshold(_therm, true);
	CuAssertIntEquals(tc, 185, iThresh);
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/**
 * Incr�menter la temp�rature de consigne "Nuit".
 */
static void testIncrNightThresh(CuTest* tc) {
	int iThresh;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	uiSetMode(EDIT_CONS);
	uiProcessKey(MOON);
	thermSetThreshold(_therm, 120, false);
	thermSetThreshold(_therm, 180, true);
	uiProcessKey(MOON);
	uiUpdate();
	iThresh = thermGetThreshold(_therm, false);
	CuAssertIntEquals(tc, 125, iThresh);
	iThresh = thermGetThreshold(_therm, true);
	CuAssertIntEquals(tc, 180, iThresh);
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

CuSuite* thermui_editconsSuite() {
	CuSuite* suite = CuSuiteNew();
	/* add test functions */
	SUITE_ADD_TEST(suite, testDisplayProgram);
	SUITE_ADD_TEST(suite, testDisplayDay);
	SUITE_ADD_TEST(suite, testDisplayDayThresh);
	SUITE_ADD_TEST(suite, testDisplayHour);
	SUITE_ADD_TEST(suite, testDisplayMinute);
	SUITE_ADD_TEST(suite, testDisplaySecond);
	SUITE_ADD_TEST(suite, testDisplayNoStar);
	SUITE_ADD_TEST(suite, testDisplayDay2Night);
	SUITE_ADD_TEST(suite, testDisplayNight2Day);
	SUITE_ADD_TEST(suite, testIncrDayThresh);
	SUITE_ADD_TEST(suite, testIncrNightThresh);
	return suite;
}

