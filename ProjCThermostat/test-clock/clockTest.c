/**
 * @file clockTest.c
 * $Id$
 *
 * Created on: 26 sept. 2012
 * @author Christophe TOMBELLE
 */
#include <stdio.h>
#include <stdlib.h>

#include "clockTest.h"
#include "clock.h"
#include "simtime.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES PROGRAMMES DE TEST
 * ***************************************
 */

static IClock _clock;

/**
 * Préparation de l'objet testé.
 */
static void setUp() {
	/* Simuler l'instant (quelconque) fourni par now() */
	setNow(60480034622L);
	_clock = createClock();
}

/**
 * Libération de l'objet testé.
 */
static void tearDown() {
	free(_clock);
	_clock = NULL;
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

/** Tester getTime. */
static void testGetTime(CuTest* tc) {
	long long lTime;
	printf("%s\n", tc->name);
	setUp();
	/* Vérifier qu'il est Dimanche 00:00:00 et 0 ms (début de semaine) */
	lTime = clkGetTime(_clock);
	CuAssertLongLongEquals(tc, 0L, lTime);
	/* Simuler le passage du temps (1 semaine) */
	elapseLong(MS_PER_WEEK);
	/* Vérifier le fonctionnement "modulo 1 semaine" */
	lTime = clkGetTime(_clock);
	CuAssertLongLongEquals(tc, 0L, lTime);
	/* Simuler le passage du temps (1/2 semaine) */
	elapseLong(MS_PER_WEEK/2);
	/* Vérifier qu'on est à 1/2 semaine du début de la semaine */
	lTime = clkGetTime(_clock);
	CuAssertLongLongEquals(tc, MS_PER_WEEK/2, lTime);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Tester setTime. */
static void testSetTime(CuTest* tc) {
	long long lTime, lTimeExp;
	printf("%s\n", tc->name);
	setUp();
	/* Mettre à l'heure : "Lundi 02:03:04 et 5ms" */
	lTimeExp = toMs(1, 2, 3, 4, 5);
	clkSetTime(_clock, lTimeExp);
	/* Vérifier la mise à l'heure */
	lTime = clkGetTime(_clock);
	CuAssertLongLongEquals(tc, lTimeExp, lTime);
	/* Simuler le passage du temps : 1 jour et 2 heures etc... */
	elapse(1, 2, 3, 4, 5);
	/* il doit donc être : Mardi 04:06:08 et 10 ms */
	lTimeExp = toMs(2,4,6,8,10);
	/* Vérifier la mise à l'heure */
	lTime = clkGetTime(_clock);
	CuAssertLongLongEquals(tc, lTimeExp, lTime);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Tester getDay. */
static void testGetDay(CuTest* tc) {
	int iDay;
	printf("%s\n", tc->name);
	setUp();
	/* Vérifier qu'on est Dimanche (day=0) */
	CuAssertIntEquals(tc, 0, clkGetDay(_clock));
	/* Simuler le passage du temps : 1 jour et 2 heures etc... */
	elapse(1, 2, 3, 4, 5);
	/* Vérifier qu'on est Lundi */
	iDay = clkGetDay(_clock);
	CuAssertIntEquals(tc, 1, iDay);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Tester getHour. */
static void testGetHour(CuTest* tc) {
	int iHour;
	printf("%s\n", tc->name);
	setUp();
	/* Vérifier qu'il est 00 heures */
	iHour = clkGetHour(_clock);
	CuAssertIntEquals(tc, 0, iHour);
	/* Simuler le passage du temps : 1 jour et 2 heures etc... */
	elapse(1, 2, 3, 4, 5);
	/* Vérifier qu'il est 02 heures */
	iHour = clkGetHour(_clock);
	CuAssertIntEquals(tc, 2, iHour);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Tester getMinute. */
static void testGetMinute(CuTest* tc) {
	int iMin;
	printf("%s\n", tc->name);
	setUp();
	/* Vérifier 00 mn de l'heure en cours */
	iMin = clkGetMinute(_clock);
	CuAssertIntEquals(tc, 0, iMin);
	/* Simuler le passage du temps : 1 jour et 2 heures etc... */
	elapse(1, 2, 3, 4, 5);
	/* Vérifier 03 mn de l'heure en cours */
	iMin = clkGetMinute(_clock);
	CuAssertIntEquals(tc, 3, iMin);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Tester getSecond. */
static void testGetSecond(CuTest* tc) {
	int iSec;
	printf("%s\n", tc->name);
	setUp();
	/* Vérifier 00 s de la minute en cours */
	iSec = clkGetSecond(_clock);
	CuAssertIntEquals(tc, 0, iSec);
	/* Simuler le passage du temps : 1 jour et 2 heures etc... */
	elapse(1, 2, 3, 4, 5);
	/* Vérifier 04 s de la minute en cours */
	iSec = clkGetSecond(_clock);
	CuAssertIntEquals(tc, 4, iSec);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Tester setDay. */
static void testSetDay(CuTest* tc) {
	long long lTime, lTimeExp;
	int iDay, iHour, iMin, iSec;
	printf("%s\n", tc->name);
	setUp();
	/* Simuler le passage du temps : 1 jour et 2 heures etc... */
	elapse(1, 2, 3, 4, 5);
	/* "On est Samedi" */
	clkSetDay(_clock, 6);
	/* Vérifier la mise à l'heure via les différentes composantes */
	lTime = clkGetTime(_clock);
	lTimeExp = toMs(6, 2, 3, 4, 5);
	CuAssertLongLongEquals(tc, lTimeExp, lTime);
	iDay = clkGetDay(_clock);
	CuAssertIntEquals(tc, 6, iDay);
	iHour = clkGetHour(_clock);
	CuAssertIntEquals(tc, 2, iHour);
	iMin = clkGetMinute(_clock);
	CuAssertIntEquals(tc, 3, iMin);
	iSec = clkGetSecond(_clock);
	CuAssertIntEquals(tc, 4, iSec);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Tester setHour. */
static void testSetHour(CuTest* tc) {
	long long lTime, lTimeExp;
	int iDay, iHour, iMin, iSec;
	printf("%s\n", tc->name);
	setUp();
	elapse(1, 2, 3, 4, 5);
	/* Changer la composante "heure" */
	clkSetHour(_clock, 6);
	/* Vérifier la mise à l'heure via les différentes composantes */
	lTime = clkGetTime(_clock);
	lTimeExp = toMs(1, 6, 3, 4, 5);
	CuAssertLongLongEquals(tc, lTimeExp, lTime);
	iDay = clkGetDay(_clock);
	CuAssertIntEquals(tc, 1, iDay);
	iHour = clkGetHour(_clock);
	CuAssertIntEquals(tc, 6, iHour);
	iMin = clkGetMinute(_clock);
	CuAssertIntEquals(tc, 3, iMin);
	iSec = clkGetSecond(_clock);
	CuAssertIntEquals(tc, 4, iSec);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Tester setMinute. */
static void testSetMinute(CuTest* tc) {
	long long lTime, lTimeExp;
	int iDay, iHour, iMin, iSec;
	printf("%s\n", tc->name);
	setUp();
	elapse(1, 2, 3, 4, 5);
	/* Changer la composante "minute" */
	clkSetMinute(_clock, 6);
	/* Vérifier la mise à l'heure via les différentes composantes */
	lTime = clkGetTime(_clock);
	/* NB : setMinute remet à 0 les secondes et millisecondes */
	lTimeExp = toMs(1, 2, 6, 0, 0);
	CuAssertLongLongEquals(tc, lTimeExp, lTime);
	iDay = clkGetDay(_clock);
	CuAssertIntEquals(tc, 1, iDay);
	iHour = clkGetHour(_clock);
	CuAssertIntEquals(tc, 2, iHour);
	iMin = clkGetMinute(_clock);
	CuAssertIntEquals(tc, 6, iMin);
	iSec = clkGetSecond(_clock);
	CuAssertIntEquals(tc, 0, iSec);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

CuSuite* clockSuite() {
	CuSuite* suite = CuSuiteNew();
	/* add test functions */
	SUITE_ADD_TEST(suite, testGetTime);
	SUITE_ADD_TEST(suite, testSetTime);
	SUITE_ADD_TEST(suite, testGetDay);
	SUITE_ADD_TEST(suite, testGetMinute);
	SUITE_ADD_TEST(suite, testGetSecond);
	SUITE_ADD_TEST(suite, testGetHour);
	SUITE_ADD_TEST(suite, testSetDay);
	SUITE_ADD_TEST(suite, testSetHour);
	SUITE_ADD_TEST(suite, testSetMinute);
	return suite;
}
