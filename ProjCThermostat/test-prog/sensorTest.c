/**
 * @file sensorTest.c
 * $Id$
 *
 * Created on: 26 sept. 2012
 * @author Christophe TOMBELLE
 */
#include <stdio.h>

#include "appalloc.h"
#include "sensorTest.h"
#include "sensor.h"
#include "simport.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES PROGRAMMES DE TEST
 * ***************************************
 */

static IPort _portADC;

/**
 * Préparation des objets partenaires de l'objet testé.
 */
static void setUp() {
	/* Le port du capteur de température est simulé */
	_portADC = createSimPort();
}
/**
 * Libération des objets partenaires de l'objet testé.
 */
static void tearDown() {
	free(_portADC);
	_portADC = NULL;
}

/** Tester getTemp. */
static void testGetTemp(CuTest* tc) {
	ISensor sensor;
	int iTemp;
	printf("%s\n", tc->name);
	setUp();
	sensor = createSensor(_portADC);
	portSimInput(_portADC, 50);
	iTemp = sensorGetTemp(sensor);
	CuAssertIntEquals(tc, 50, iTemp);
	portSimInput(_portADC, 255);
	iTemp = sensorGetTemp(sensor);
	CuAssertIntEquals(tc, 255, iTemp);
	free(sensor);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

CuSuite* sensorSuite() {
	CuSuite* suite = CuSuiteNew();
	// add test functions
	SUITE_ADD_TEST(suite, testGetTemp);
	return suite;
}

