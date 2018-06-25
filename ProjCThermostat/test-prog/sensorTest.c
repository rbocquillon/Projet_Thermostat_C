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
 * Pr�paration des objets partenaires de l'objet test�.
 */
static void setUp() {
	/* Le port du capteur de temp�rature est simul� */
	_portADC = createSimPort();
}
/**
 * Lib�ration des objets partenaires de l'objet test�.
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
	/* Penser � archiver d�s que ce test passe OK */
}

CuSuite* sensorSuite() {
	CuSuite* suite = CuSuiteNew();
	// add test functions
	SUITE_ADD_TEST(suite, testGetTemp);
	return suite;
}

