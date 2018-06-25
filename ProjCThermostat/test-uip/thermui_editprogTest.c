/**
 * @file thermui_editprogTest.c
 * $Id$
 *
 * Created on: 1 oct. 2012
 * @author Christophe TOMBELLE
 */
#include <stdbool.h>
#include <stdio.h>

#include "appalloc.h"
#include "thermui_editProgTest.h"
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
 * Préparation des objets partenaires de l'objet testé.
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
 * Libération des objets partenaires de l'objet testé.
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
static void testProgEditMode(CuTest* tc) {
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	uiSetMode(EDIT_PROG);
	uiUpdate();
	char * str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Diman. 00,0°C        00:00 H ", str);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/**
 * Afficher le numéro du programme en cours d'édition.
 */
static void testDisplayProg(CuTest* tc) {
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	/* simuler programme 1 aujourd'hui dimanche */
	thermSimProg(_therm, 1);
	/* on passe en mode édition de consigne */
	uiSetMode(EDIT_PROG);
	/* Le temps passe : "on est lundi" */
	clkSetTime(_clock, MS_PER_DAY);
	uiUpdate();
	char * str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P1 Diman. 00,0°C        00:00 N ", str);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/**
 * Afficher le jour en cours d'édition.
 */
static void testDisplayDay(CuTest* tc) {
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	/* simuler "on est lundi" */
	clkSetTime(_clock, MS_PER_DAY);
	/* on passe en mode édition de programmes */
	uiSetMode(EDIT_PROG);
	/* Le temps passe : "on est mardi" */
	clkSetTime(_clock, 2*MS_PER_DAY);
	uiUpdate();
	char * str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Lundi  00,0°C        00:00 H ", str);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/**
 * Afficher la température ambiante.
 */
static void testDisplayTemperature(CuTest* tc) {
	char *str;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	/* on passe en mode édition de programmes */
	uiSetMode(EDIT_PROG);
	/* simuler température ambiante de 1°C */
	thermSimTemp(_therm, 10);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Diman. 01,0°C        00:00 H ", str);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/**
 * Afficher le créneau en cours d'édition.
 */
static void testDisplaySlot(CuTest* tc) {
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	/* simuler "il est 01:00" */
	clkSetTime(_clock, MS_PER_HOUR);
	/* on passe en mode édition de programmes */
	uiSetMode(EDIT_PROG);
	/* Le temps passe : "il est 02:00" */
	clkSetTime(_clock, 2*MS_PER_DAY);
	uiUpdate();
	char * str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Diman. 00,0°C        01:00 H ", str);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/**
 * Pas d'étoile même si chaudière allumée.
 */
static void testDisplayNoStar(CuTest* tc) {
	char *str;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	/* on passe en mode édition de programmes */
	uiSetMode(EDIT_PROG);
	/* simuler "chaudière allumée" */
	boilerSetOn(_boiler, true);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Diman. 00,0°C        00:00 H ", str);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/**
 * Incrémenter le numéro de programme.
 */
static void testNextProg(CuTest* tc) {
	char *str;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	/* on passe en mode édition de programmes */
	uiSetMode(EDIT_PROG);
	/* P0 -> P1 */
	uiProcessKey(NPROG);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P1 Diman. 00,0°C        00:00 N ", str);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/**
 * Passer au jour suivant.
 */
static void testNextDay(CuTest* tc) {
	char *str;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	/* on passe en mode édition de programmes */
	uiSetMode(EDIT_PROG);
	/* Diman. -> Lundi */
	uiProcessKey(PROG);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Lundi  00,0°C        00:00 H ", str);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/**
 * Passer en créneau Jour.
 */
static void testDaySlot(CuTest* tc) {
	char *str;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	/* on passe en mode édition de programmes */
	uiSetMode(EDIT_PROG);
	/* P0 -> P1 */
	uiProcessKey(NPROG);
	/* P1 -> P2 */
	uiProcessKey(NPROG);
	/* 00:00 J -> 01:00 */
	uiProcessKey(SUN);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P2 Diman. 00,0°C00:00 J 01:00 N ", str);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/**
 * Passer en créneau Nuit.
 */
static void testNightSlot(CuTest* tc) {
	IProgram prog;
	char *str;
	printf("%s\n", tc->name);
	setUp();
	prog = thermGetProgram(_therm, 2);
	progSetChoice(prog, true, 0);
	createThermUI(_therm, _disp);
	/* on passe en mode édition de programmes */
	uiSetMode(EDIT_PROG);
	/* P0 -> P1 */
	uiProcessKey(NPROG);
	/* P1 -> P2 */
	uiProcessKey(NPROG);
	/* 00:00 N -> 01:00 */
	uiProcessKey(MOON);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P2 Diman. 00,0°C00:00 N 01:00 N ", str);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/**
 * Naviguer dans le programme 0.
 */
static void testProg0NextSlot(CuTest* tc) {
	char *str;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	/* on passe en mode édition de programmes */
	uiSetMode(EDIT_PROG);
	/* Naviguer avec MOON */
	uiProcessKey(MOON);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Diman. 00,0°C00:00 H 01:00 H ", str);
	/* Naviguer avec SUN */
	uiProcessKey(SUN);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Diman. 00,0°C01:00 H 02:00 H ", str);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/**
 * Naviguer dans programme non modifiable.
 */
static void testProg1NextSlot(CuTest* tc) {
	char *str;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	/* on passe en mode édition de programmes */
	uiSetMode(EDIT_PROG);
	/* P0 -> P1 */
	uiProcessKey(NPROG);
	/* 00:00 -> 01:00 */
	uiProcessKey(MOON);
	/* 01:00 -> 02:00 */
	uiProcessKey(SUN);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P1 Diman. 00,0°C01:00 N 02:00 N ", str);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/**
 * Passer au jour suivant après navigation.
 */
static void testNextDayAfterNextSlot(CuTest* tc) {
	char *str;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	/* on passe en mode édition de programmes */
	uiSetMode(EDIT_PROG);
	/* P0 -> P1 */
	uiProcessKey(NPROG);
	/* 00:00 -> 01:00 */
	uiProcessKey(SUN);
	/* Diman. -> Lundi */
	uiProcessKey(PROG);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P0 Lundi  00,0°C        01:00 H ", str);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/**
 * Incrémenter le numéro de programme après navigation.
 */
static void testNextProgAfterNextSlot(CuTest* tc) {
	char *str;
	printf("%s\n", tc->name);
	setUp();
	createThermUI(_therm, _disp);
	/* on passe en mode édition de programmes */
	uiSetMode(EDIT_PROG);
	/* P0 -> P1 */
	uiProcessKey(NPROG);
	/* 00:00 -> 01:00 */
	uiProcessKey(SUN);
	/* P1 -> P2 */
	uiProcessKey(NPROG);
	uiUpdate();
	str = dispAsByteArray(_disp);
	/*                     01234567890123456789012345678901 */
	CuAssertStrEquals(tc, "P2 Diman. 00,0°C        01:00 N ", str);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

CuSuite* thermui_editprogSuite() {
	CuSuite* suite = CuSuiteNew();
	/* add test functions */
	SUITE_ADD_TEST(suite, testProgEditMode);
	SUITE_ADD_TEST(suite, testDisplayProg);
	SUITE_ADD_TEST(suite, testDisplayDay);
	SUITE_ADD_TEST(suite, testDisplayTemperature);
	SUITE_ADD_TEST(suite, testDisplaySlot);
	SUITE_ADD_TEST(suite, testDisplayNoStar);
	SUITE_ADD_TEST(suite, testNextProg);
	SUITE_ADD_TEST(suite, testNextDay);
	SUITE_ADD_TEST(suite, testDaySlot);
	SUITE_ADD_TEST(suite, testNightSlot);
	SUITE_ADD_TEST(suite, testProg0NextSlot);
	SUITE_ADD_TEST(suite, testProg1NextSlot);
	SUITE_ADD_TEST(suite, testNextDayAfterNextSlot);
	SUITE_ADD_TEST(suite, testNextProgAfterNextSlot);
	return suite;
}

