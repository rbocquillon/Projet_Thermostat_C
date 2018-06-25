/**
 * @file thermostatTest.c
 * $Id$
 *
 * Created on: 26 sept. 2012
 * @author Christophe TOMBELLE
 */
#include <stdio.h>

#include "appalloc.h"
#include "thermostatTest.h"
#include "thermostat.h"
#include "simclock.h"
#include "simport.h"

#define SUNDAY 0
/*
 * ***************************************
 * NE PAS MODIFIER LES PROGRAMMES DE TEST
 * ***************************************
 */

static IPort _portLedsDip;
static IBoiler _boiler;
static IPort _portCAN;
static ISensor _sensor;
static IClock _clock;
static IThermostat _therm;

/**
 * Préparation des objets partenaires de l'objet testé.
 */
static void setUp() {
	_portLedsDip = createSimPort();
	_boiler = createBoiler((IPort) _portLedsDip);
	_portCAN = createSimPort();
	_sensor = createSensor((IPort) _portCAN);
	_clock = createSimClock();
	_therm = createThermostat(_boiler, _sensor, (IClock)_clock);
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
	_portLedsDip = NULL;
	_boiler = NULL;
	_portCAN = NULL;
	_sensor = NULL;
	_clock = NULL;
	_therm = NULL;
}

/** Tester isBoilerOn. */
static void testIsBoilerOn(CuTest* tc) {
	printf("%s\n", tc->name);
	setUp();
	CuAssertFalse(tc, thermIsBoilerOn(_therm));
	boilerSetOn(_boiler, true);
	CuAssertTrue(tc, thermIsBoilerOn(_therm));
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Tester getTemp. */
static void testGetTemp(CuTest* tc) {
	printf("%s\n", tc->name);
	setUp();
	portSimInput(_portCAN, 190);
	CuAssertIntEquals(tc, 190, thermGetTemp(_therm));
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Tester getProg. */
static void testGetClock(CuTest* tc) {
	printf("%s\n", tc->name);
	setUp();
	IClock clock = thermGetClock(_therm);
	CuAssertPtrEquals(tc, _clock, clock);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Tester getProg. */
static void testGetProg(CuTest* tc) {
	int i;
	printf("%s\n", tc->name);
	setUp();
	for (i = 0; i < 7; i++)
		CuAssertIntEquals(tc, 0, thermGetProg(_therm, i));
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Tester setProg. */
static void testSetProg(CuTest* tc) {
	int iDay;
	printf("%s\n", tc->name);
	setUp();
	for (iDay = 0; iDay < 7; iDay++) {
		int iProg = iDay + 1;
		thermSetProg(_therm, iProg, iDay);
	}
	for (iDay = 0; iDay < 7; iDay++) {
		int iProgExp = iDay + 1;
		int iProg = thermGetProg(_therm, iDay);
		CuAssertIntEquals(tc, iProgExp, iProg);
	}
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Tester getCurrentProg. */
static void testGetCurrentProg(CuTest* tc) {
	int iDay;
	printf("%s\n", tc->name);
	setUp();
	for (iDay = 0; iDay < 7; iDay++) {
		int iProg = iDay+1;
		thermSetProg(_therm, iProg, iDay);
	}
	for (iDay = 0; iDay < 7; iDay++) {
		int iProg, iProgExp = iDay+1;
		clkSetDay(_clock, iDay);
		iProg = thermGetCurrentProg(_therm);
		CuAssertIntEquals(tc, iProgExp, iProg);
	}
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Programme 0 NULL. */
static void testProgram0Null(CuTest* tc) {
	IProgram prog;
	printf("%s\n", tc->name);
	setUp();
	prog = thermGetProgram(_therm, 0);
	/* Vérifier que le programme 0 est NULL. */
	CuAssertPtrNull(tc, prog);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Programmes 1 à 6 tout "nuit". */
static void testNightPrograms(CuTest* tc) {
	int iProg;
	printf("%s\n", tc->name);
	setUp();
	for (iProg = 1; iProg < 7; iProg++) {
		int iHour;
		IProgram prog = thermGetProgram(_therm, iProg);
		CuAssertPtrNotNull(tc, prog);
		for (iHour = 0; iHour < 24; iHour++) {
			bool bDay = progIsDay(prog, iHour);
			CuAssertFalse(tc, bDay);
		}
	}
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Programme 7 tout "jour". */
static void testProgram7(CuTest* tc) {
	int iHour;
	IProgram prog;
	printf("%s\n", tc->name);
	setUp();
	prog = thermGetProgram(_therm, 7);
	CuAssertPtrNotNull(tc, prog);
	for (iHour = 0; iHour < 24; iHour++) {
		bool bDay = progIsDay(prog, iHour);
		CuAssertTrue(tc, bDay);
	}
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Programme 1 non modifiable. */
static void testProgram1Fixed(CuTest* tc) {
	IProgram prog;
	printf("%s\n", tc->name);
	setUp();
	/* Vérifier que le programme 1 est non modifiable */
	prog = thermGetProgram(_therm, 1);
	CuAssertFalse(tc, progIsDay(prog, 0));
	progSetChoice(prog, true, 0);
	CuAssertFalse(tc, progIsDay(prog, 0));
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Programme 7 non modifiable. */
static void testProgram7Fixed(CuTest* tc) {
	IProgram prog;
	printf("%s\n", tc->name);
	setUp();
	/* Vérifier que le programme 7 est non modifiable */
	prog = thermGetProgram(_therm, 7);
	CuAssertTrue(tc, progIsDay(prog, 0));
	progSetChoice(prog, false, 0);
	CuAssertTrue(tc, progIsDay(prog, 0));
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Tester getThreshold. */
static void testGetThreshold(CuTest* tc) {
	printf("%s\n", tc->name);
	setUp();
	CuAssertIntEquals(tc, 120, thermGetThreshold(_therm, false));
	CuAssertIntEquals(tc, 180, thermGetThreshold(_therm, true));
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Tester setThreshold. */
static void testSetThreshold(CuTest* tc) {
	printf("%s\n", tc->name);
	setUp();
	thermSetThreshold(_therm, 130, false);
	CuAssertIntEquals(tc, 130, thermGetThreshold(_therm, false));
	thermSetThreshold(_therm, 190, true);
	CuAssertIntEquals(tc, 190, thermGetThreshold(_therm, true));
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Tester régulation autour de la consigne "hors-gel". */
static void testRegulate(CuTest* tc) {
	printf("%s\n", tc->name);
	setUp();
	/* Simuler une température juste suffisante */
	portSimInput(_portCAN, 50);
	thermRegulate(_therm);
	CuAssertFalse(tc, boilerIsOn(_boiler));
	/* Simuler une température juste sous la consigne */
	portSimInput(_portCAN, 49);
	thermRegulate(_therm);
	CuAssertTrue(tc, boilerIsOn(_boiler));
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Tester régulation autour de la consigne "Nuit". */
static void testRegulateNight(CuTest* tc) {
	printf("%s\n", tc->name);
	setUp();
	/* consigne "nuit" : 12,5°C */
	thermSetThreshold(_therm, 125, false);
	/* Dimanche P1 (tout "nuit") */
	thermSetProg(_therm, 1, SUNDAY);
	/* Simuler une température juste suffisante */
	portSimInput(_portCAN, 125);
	thermRegulate(_therm);
	CuAssertFalse(tc, boilerIsOn(_boiler));
	/* Simuler une température juste sous la consigne */
	portSimInput(_portCAN, 124);
	thermRegulate(_therm);
	CuAssertTrue(tc, boilerIsOn(_boiler));
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Tester régulation autour de la consigne "Jour". */
static void testRegulateDay(CuTest* tc) {
	IProgram prog;
	IClock clock;
	printf("%s\n", tc->name);
	setUp();
	/* consigne "jour" : 18,5°C */
	thermSetThreshold(_therm, 185, true);
	/* P2 en "jour" à 08:00 */
	prog = thermGetProgram(_therm, 2);
	progSetChoice(prog, true, 8);
	/* P2 programme du jour */
	thermSetProg(_therm, 2, SUNDAY);
	/* "Il est 08:00" */
	clock = thermGetClock(_therm);
	clkSetHour(clock, 8);
	/* Simuler une température juste suffisante */
	portSimInput(_portCAN, 185);
	thermRegulate(_therm);
	CuAssertFalse(tc, boilerIsOn(_boiler));
	/* Simuler une température juste sous la consigne */
	portSimInput(_portCAN, 184);
	thermRegulate(_therm);
	CuAssertTrue(tc, boilerIsOn(_boiler));
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

CuSuite* thermostatSuite() {
	CuSuite* suite = CuSuiteNew();
	/* add test functions */
	SUITE_ADD_TEST(suite, testIsBoilerOn);
	SUITE_ADD_TEST(suite, testGetTemp);
	SUITE_ADD_TEST(suite, testGetClock);
	SUITE_ADD_TEST(suite, testGetProg);
	SUITE_ADD_TEST(suite, testSetProg);
	SUITE_ADD_TEST(suite, testGetCurrentProg);
	SUITE_ADD_TEST(suite, testProgram0Null);
	SUITE_ADD_TEST(suite, testNightPrograms);
	SUITE_ADD_TEST(suite, testProgram7);
	SUITE_ADD_TEST(suite, testProgram1Fixed);
	SUITE_ADD_TEST(suite, testProgram7Fixed);
	SUITE_ADD_TEST(suite, testGetThreshold);
	SUITE_ADD_TEST(suite, testSetThreshold);
	SUITE_ADD_TEST(suite, testRegulate);
	SUITE_ADD_TEST(suite, testRegulateNight);
	SUITE_ADD_TEST(suite, testRegulateDay);
	return suite;
}
