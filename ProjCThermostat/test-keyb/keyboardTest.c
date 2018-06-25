/**
 * @file keyboardTest.c
 * $Id$
 *
 * Created on: 26 sept. 2012
 * @author Christophe TOMBELLE
 */
#include <stdio.h>
#include <stdlib.h>

#include "simhal.h"
#include "keyboard.h"
#include "keyboardTest.h"
#include "keybport.h"
#include "simtime.h"

#define MASK_BOILER_ON 0x80

/*
 * ***************************************
 * NE PAS MODIFIER LES PROGRAMMES DE TEST
 * ***************************************
 */

IPort _portDip;
IPort _portLeds;

/**
 * Pr�paration des objets partenaires de l'objet test�.
 */
static void setUp() {
	_portDip = createKeybPort();
	_portLeds = createKeybPort();
	kbpSetLigPort(_portLeds, _portDip );
	/* simuler instant (quelconque) fourni par now() */
	setNow(60480034622L);
}
/**
 * Lib�ration des objets partenaires de l'objet test�.
 */
static void tearDown() {
	free(_portDip);
	free(_portLeds);
	_portDip = NULL;
	_portLeds = NULL;
}

/**
 * Tester touches de la colonne du bit 0.
 */
static void testCol0Keys(CuTest* tc) {
	IKeyboard kbd;
	int iLig, iKey;
	/* 1er code de la colonne du bit 0 */
	int iKeyExp = TEMP;
	printf("%s\n", tc->name);
	setUp();
	portWrite(_portLeds, MASK_BOILER_ON);
	kbd = createKeyboard(_portLeds, _portDip);
	/* pour iLig = 1, 2, 4, 8 */
	for (iLig=1; iLig<9; iLig <<= 1) {
		/* simuler touche appuy�e */
		kbpSetCode(_portLeds, 1, iLig );
		/* v�rifier le code fourni par getKey est celui de la touche appuy�e */
		iKey = kbdGetKey(kbd);
		CuAssertIntEquals(tc, iKeyExp, iKey);
		/* v�rifier que la chaudi�re n'est pas perturb�e par le clavier */
		CuAssertIntEquals(tc, MASK_BOILER_ON, portReadLatch(_portLeds) & MASK_BOILER_ON);
		/* simuler touche rel�ch�e */
		kbpSetCode(_portLeds, 0, 0 );
		/* v�rifier que le code fourni par getKey est 0 */
		iKey = kbdGetKey(kbd);
		CuAssertIntEquals(tc, 0, iKey);
		iKeyExp++;
	}
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/**
 * Tester touches de la colonne du bit 1.
 */
static void testCol1Keys(CuTest* tc) {
	IKeyboard kbd;
	int iLig, iKey;
	/* 1er code de la colonne du bit 1 */
	int iKeyExp = PROG;
	printf("%s\n", tc->name);
	setUp();
	portWrite(_portLeds, MASK_BOILER_ON);
	kbd = createKeyboard(_portLeds, _portDip);
	/* pour iLig = 1, 2, 4, 8 */
	for (iLig=1; iLig<9; iLig <<= 1) {
		/* simuler touche appuy�e */
		kbpSetCode(_portLeds, 2, iLig );
		/* v�rifier le code fourni par getKey est celui de la touche appuy�e */
		iKey = kbdGetKey(kbd);
		CuAssertIntEquals(tc, iKeyExp, iKey);
		/* v�rifier que la chaudi�re n'est pas perturb�e par le clavier */
		CuAssertIntEquals(tc, MASK_BOILER_ON, portReadLatch(_portLeds) & MASK_BOILER_ON);
		/* simuler touche rel�ch�e */
		kbpSetCode(_portLeds, 0, 0 );
		/* v�rifier que le code fourni par getKey est 0 */
		iKey = kbdGetKey(kbd);
		CuAssertIntEquals(tc, 0, iKey);
		iKeyExp++;
	}
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/**
 * Tester touche OK (broche d'interruption).
 */
static void testOkKey(CuTest* tc) {
	IKeyboard kbd;
	int iKey;
	printf("%s\n", tc->name);
	setUp();
	kbd = createKeyboard(_portLeds, _portDip);
	/* simuler une interruption */
	simISR();
	iKey = kbdGetKey(kbd);
	CuAssertIntEquals(tc, OK, iKey);
	iKey = kbdGetKey(kbd);
	CuAssertIntEquals(tc, 0, iKey);
	tearDown();
	/* Penser � archiver d�s que ce test passe OK */
}

/** Tester la r�p�tition sur d�lai. */
static void testGetKeyTime(CuTest* tc) {
	IKeyboard kbd;
	int iKey;
	printf("%s\n", tc->name);
	setUp();
	kbd = createKeyboard(_portLeds, _portDip);
	/* simuler touche appuy�e en col 0, ligne 1 (code 2) */
	kbpSetCode(_portLeds, 1, 2);
	iKey = kbdGetKey(kbd);
	CuAssertIntEquals(tc, 2, iKey);
	/* essayer trop t�t */
	iKey = kbdGetKey(kbd);
	CuAssertIntEquals(tc, 0, iKey);
	iKey = kbdGetKey(kbd);
	CuAssertIntEquals(tc, 0, iKey);
	/* essayer apr�s le bon REPEAT_DELAY */
	elapseLong(REPEAT_DELAY);
	iKey = kbdGetKey(kbd);
	CuAssertIntEquals(tc, 2, iKey);
	/* essayer trop t�t */
	iKey = kbdGetKey(kbd);
	CuAssertIntEquals(tc, 0, iKey);
	iKey = kbdGetKey(kbd);
	CuAssertIntEquals(tc, 0, iKey);
	/* essayer apr�s le bon REPEAT_DELAY */
	elapseLong(REPEAT_DELAY);
	iKey = kbdGetKey(kbd);
	CuAssertIntEquals(tc, 2, iKey);
	/* simuler touche rel�ch�e */
	elapseLong(REPEAT_DELAY);
	kbpSetCode(_portLeds, 0, 0);
	iKey = kbdGetKey(kbd);
	CuAssertIntEquals(tc, 0, iKey);
	tearDown();
}

CuSuite* keyboardSuite() {
	CuSuite* suite = CuSuiteNew();
	// add test functions
	SUITE_ADD_TEST(suite, testCol0Keys);
	SUITE_ADD_TEST(suite, testCol1Keys);
	SUITE_ADD_TEST(suite, testOkKey);
	SUITE_ADD_TEST(suite, testGetKeyTime);
	return suite;
}
