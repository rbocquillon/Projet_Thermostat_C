/**
 * @file programTest.c
 * $Id$
 *
 * Created on: 11 juil. 2012
 * @author Christophe TOMBELLE
 */
#include <stdio.h>

#include "appalloc.h"
#include "programTest.h"
#include "program.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES PROGRAMMES DE TEST
 * ***************************************
 */

/**
 * Tester l'�tat apr�s creation.
 */
static void testIsDay(CuTest* tc) {
	IProgram prog;
	int iHour;
	printf("%s\n", tc->name);
	prog = createProgram();
	/* V�rifier consigne "Nuit" � toute heure */
	for (iHour=0; iHour<HOURS_PER_DAY; iHour++) {
		bool bDay = progIsDay(prog, iHour);
		CuAssertFalse(tc, bDay);
	}
	free(prog);
	/* Penser � archiver d�s que ce test passe OK */
}

/**
 * Tester la programmabilit� avec setChoice.
 */
static void testSetChoice(CuTest* tc) {
	IProgram prog;
	int iHour;
	printf("%s\n", tc->name);
	prog = createProgram();
	/* Heures paires en consigne "Jour" */
	for (iHour=0; iHour<HOURS_PER_DAY; iHour+=2)
		progSetChoice(prog, true, iHour );
	/* V�rifier heures paires en consigne "Jour", impaires en consigne "Nuit" */
	for (iHour=0; iHour<HOURS_PER_DAY; iHour++) {
		bool bDay = progIsDay(prog, iHour);
		CuAssertIntEquals(tc, iHour%2==0, bDay);
	}
	free(prog);
	/* Penser � archiver d�s que ce test passe OK */
}

/**
 * Tester la non modifiabilit� avec setFixed.
 */
static void testSetFixed(CuTest* tc) {
	IProgram prog;
	printf("%s\n", tc->name);
	prog = createProgram();
	progSetFixed(prog);
	/* Tenter de modifier un programme non modifiable */
	progSetChoice(prog, true, 1);
	/* V�rifier que setChoice a �t� inop�rante */
	CuAssertFalse(tc, progIsDay(prog, 1));
	free(prog);
	/* Penser � archiver d�s que ce test passe OK */
}

CuSuite* programSuite() {
	CuSuite* suite = CuSuiteNew();
	/* add test functions */
	SUITE_ADD_TEST(suite, testIsDay);
	SUITE_ADD_TEST(suite, testSetChoice);
	SUITE_ADD_TEST(suite, testSetFixed);
	return suite;
}
