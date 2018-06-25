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

/* La chaudi�re est pilot�e sur le bit 7 du port */
#define MASK_BOILER_ON 0x80
#define MASK_BOILER_OFF 0x7F

static IPort _portLeds;

static void setUp() {
	/* Le port de pilotage chaudi�re est simul� */
	_portLeds = createSimPort();
	/* Tous bits � 1 (chaudi�re allum�e) */
	portWrite(_portLeds, 0xFF);
}

/**
 * Lib�ration des objets partenaires de l'objet test�.
 */
static void tearDown() {
	free(_portLeds);
	_portLeds = NULL;
}

/** Tester l'�tat apr�s cr�ation. */
static void testBoiler(CuTest* tc) {
	IBoiler boiler;
	bool bOn;
	printf("%s\n", tc->name);
	setUp();
	boiler = createBoiler(_portLeds);
	/* V�rifier que la chaudi�re est �teinte */
	bOn = boilerIsOn(boiler);
	CuAssertFalse(tc, bOn);
	free(boiler);
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/** Tester isOn. */
static void testIsOn(CuTest* tc) {
	IBoiler boiler;
	bool bOn;
	printf("%s\n", tc->name);
	setUp();
	boiler = createBoiler(_portLeds);
	/* Rallumer la chaudi�re */
	portWrite(_portLeds, 0xFF);
	/* V�rifier que isOn la voit allum�e */
	bOn = boilerIsOn(boiler);
	CuAssertTrue(tc, bOn);
	/* �teindre la chaudi�re */
	portWrite(_portLeds, MASK_BOILER_OFF);
	/* V�rifier que isOn la voit �teinte */
	bOn = boilerIsOn(boiler);
	CuAssertFalse(tc, bOn);
	free(boiler);
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/** Tester setOn. */
static void testSetOn(CuTest* tc) {
	IBoiler boiler;
	int bits;
	printf("%s\n", tc->name);
	setUp();
	boiler = createBoiler(_portLeds);
	/* tous bits � 1 */
	portWrite(_portLeds, 0xFF);
	/* �teindre chaudi�re */
	boilerSetOn(boiler, false);
	/* V�rifier chaudi�re �teinte */
	CuAssertFalse(tc, boilerIsOn(boiler));
	/* V�rifier autres bits non modifi�s */
	bits = portReadLatch(_portLeds);
	CuAssertIntEquals(tc, MASK_BOILER_OFF, bits);
	/* tous bits � 0 */
	portWrite(_portLeds, 0);
	/* allumer chaudi�re */
	boilerSetOn(boiler, true);
	/* V�rifier chaudi�re allum�e */
	CuAssertTrue(tc, boilerIsOn(boiler));
	/* V�rifier autres bits non modifi�s */
	bits = portReadLatch(_portLeds);
	CuAssertIntEquals(tc, MASK_BOILER_ON, bits);
	free(boiler);
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

CuSuite* boilerSuite() {
	CuSuite* suite = CuSuiteNew();
	/* add test functions */
	SUITE_ADD_TEST(suite, testBoiler);
	SUITE_ADD_TEST(suite, testIsOn);
	SUITE_ADD_TEST(suite, testSetOn);
	return suite;
}
