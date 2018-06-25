/**
 * @file boilerTest.c
 * $Id$
 *
 * Created on: 26 sept. 2012
 * @author Christophe TOMBELLE
 */
#include <stdio.h>
#include <stdlib.h>

#include "boilerTest.h"
#include "boiler.h"
#include "simport.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES PROGRAMMES DE TEST
 * ***************************************
 */

/* La chaudière est pilotée sur le bit 7 du port */
#define MASK_BOILER_ON 0x80
#define MASK_BOILER_OFF 0x7F

static IPort _portLeds;

static void setUp() {
	/* Le port de pilotage chaudière est simulé */
	_portLeds = createSimPort();
	/* Tous bits à 1 (chaudière allumée) */
	portWrite(_portLeds, 0xFF);
}

/**
 * Libération des objets partenaires de l'objet testé.
 */
static void tearDown() {
	free(_portLeds);
	_portLeds = NULL;
}

/** Tester l'état après création. */
static void testBoiler(CuTest* tc) {
	IBoiler boiler;
	bool bOn;
	printf("%s\n", tc->name);
	setUp();
	boiler = createBoiler(_portLeds);
	/* Vérifier que la chaudière est éteinte */
	bOn = boilerIsOn(boiler);
	CuAssertFalse(tc, bOn);
	free(boiler);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Tester isOn. */
static void testIsOn(CuTest* tc) {
	IBoiler boiler;
	bool bOn;
	printf("%s\n", tc->name);
	setUp();
	boiler = createBoiler(_portLeds);
	/* Rallumer la chaudière */
	portWrite(_portLeds, 0xFF);
	/* Vérifier que isOn la voit allumée */
	bOn = boilerIsOn(boiler);
	CuAssertTrue(tc, bOn);
	/* éteindre la chaudière */
	portWrite(_portLeds, MASK_BOILER_OFF);
	/* Vérifier que isOn la voit éteinte */
	bOn = boilerIsOn(boiler);
	CuAssertFalse(tc, bOn);
	free(boiler);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

/** Tester setOn. */
static void testSetOn(CuTest* tc) {
	IBoiler boiler;
	int bits;
	printf("%s\n", tc->name);
	setUp();
	boiler = createBoiler(_portLeds);
	/* tous bits à 1 */
	portWrite(_portLeds, 0xFF);
	/* éteindre chaudière */
	boilerSetOn(boiler, false);
	/* Vérifier chaudière éteinte */
	CuAssertFalse(tc, boilerIsOn(boiler));
	/* Vérifier autres bits non modifiés */
	bits = portReadLatch(_portLeds);
	CuAssertIntEquals(tc, MASK_BOILER_OFF, bits);
	/* tous bits à 0 */
	portWrite(_portLeds, 0);
	/* allumer chaudière */
	boilerSetOn(boiler, true);
	/* Vérifier chaudière allumée */
	CuAssertTrue(tc, boilerIsOn(boiler));
	/* Vérifier autres bits non modifiés */
	bits = portReadLatch(_portLeds);
	CuAssertIntEquals(tc, MASK_BOILER_ON, bits);
	free(boiler);
	tearDown();
	/* Penser à archiver dès que ce test passe OK */
}

CuSuite* boilerSuite() {
	CuSuite* suite = CuSuiteNew();
	/* add test functions */
	SUITE_ADD_TEST(suite, testBoiler);
	SUITE_ADD_TEST(suite, testIsOn);
	SUITE_ADD_TEST(suite, testSetOn);
	return suite;
}
