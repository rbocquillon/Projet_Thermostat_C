/**
 * @file displayTest.c
 * $Id$
 *
 * Created on: 26 sept. 2012
 * @author Christophe TOMBELLE
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "displayTest.h"
#include "display.h"
#include "lcd.h"
#include "simport.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES PROGRAMMES DE TEST
 * ***************************************
 */

IPort _portLCDCmd;
IPort _portLCDData;
ILCD _lcd;

/**
 * Pr�paration des objets partenaires de l'objet test�.
 */
static void setUp() {
	_portLCDCmd = createSimPort();
	_portLCDData = createSimPort();
	_lcd = createLCD(_portLCDCmd, _portLCDData);
}
/**
 * Lib�ration des objets partenaires de l'objet test�.
 */
static void tearDown() {
	free(_portLCDCmd);
	free(_portLCDData);
	free(_lcd);
	_portLCDCmd=NULL;
	_portLCDData=NULL;
	_lcd = NULL;
}

/** Tester l'�tat apr�s cr�ation */
static void testDisplay(CuTest* tc) {
	IDisplay disp;
	printf("%s\n", tc->name);
	setUp();
	disp = createDisplay(_lcd);
	/*                      01234567890123456789012345678901 */
	CuAssertStrEquals(tc,  "                                ", dispAsByteArray(disp));
	CuAssertIntEquals(tc,  0, dispGetCurs(disp));
	tearDown();
}

/** Tester writeChar. */
static void testWriteChar(CuTest* tc) {
	IDisplay disp;
	char *str;
	int iPos;
	printf("%s\n", tc->name);
	setUp();
	disp = createDisplay(_lcd);
	dispWriteChar(disp, '1');
	str = dispAsByteArray(disp);
	/*                      01234567890123456789012345678901 */
	CuAssertStrEquals(tc,  "1                               ", str);
	iPos = dispGetCurs(disp);
	CuAssertIntEquals(tc,  1, iPos);
	tearDown();
}

/** Tester writeString. */
static void testWriteString(CuTest* tc) {
	IDisplay disp;
	char *str, *strExp;
	int iPos;
	printf("%s\n", tc->name);
	setUp();
	disp = createDisplay(_lcd);
	dispWriteChar(disp, '1');
	/*     1234567890123456789012345678901 */
	str = "234567890ABCDEFGHIJKLMNOPQRSTUV";
	dispWriteString(disp, str, strlen(str));
	strExp = "1234567890ABCDEFGHIJKLMNOPQRSTUV";
	str = dispAsByteArray(disp);
	CuAssertStrEquals(tc, strExp, str);
	iPos = dispGetCurs(disp);
	CuAssertIntEquals(tc,  32, iPos);
	tearDown();
}

/** Tester writeCharAt. */
static void testWriteCharAt(CuTest* tc) {
	IDisplay disp;
	char *str;
	int iPos;
	printf("%s\n", tc->name);
	setUp();
	disp = createDisplay(_lcd);
	/*     01234567890123456789012345678901 */
	str = "1234567890ABCDEFGHIJKLMNOPQRSTUV";
	dispWriteString(disp, str, strlen(str));
	dispWriteCharAt(disp, 14, 'b');
	str = dispAsByteArray(disp);
	/*                      01234567890123456789012345678901 */
	CuAssertStrEquals(tc,  "1234567890ABCDbFGHIJKLMNOPQRSTUV", str);
	iPos = dispGetCurs(disp);
	CuAssertIntEquals(tc,  15, iPos);
	tearDown();
}

/** Tester writeStringAt. */
static void testWriteStringAt(CuTest* tc) {
	IDisplay disp;
	char *str;
	int iPos;
	printf("%s\n", tc->name);
	setUp();
	disp = createDisplay(_lcd);
	/*     01234567890123456789012345678901 */
	str = "1234567890ABCDEFGHIJKLMNOPQRSTUV";
	dispWriteString(disp, str, strlen(str));
	str = "onjour";
	dispWriteStringAt(disp, 12, str, strlen(str));
	str = dispAsByteArray(disp);
	/*                      01234567890123456789012345678901 */
	CuAssertStrEquals(tc,  "1234567890ABonjourIJKLMNOPQRSTUV", str);
	iPos = dispGetCurs(disp);
	CuAssertIntEquals(tc,  18, iPos);
	tearDown();
}

/**
 * Tester les optimisations de disp par rapport � lcd :
 * disp tient � jour une image m�moire du lcd gr�ce auquel
 * on n'�crit sur le lcd pas les caract�res d�j� �crits
 * => le curseur du lcd se d�place de fa�on paresseuse.
 */
static void testWriteOptimizations(CuTest* tc) {
	IDisplay disp;
	char *str, *strExp;
	int iPos;
	printf("%s\n", tc->name);
	setUp();
	disp = createDisplay(_lcd);
/* �crire 2 caract�res au d�but, espace sur espace existant */
	dispWriteChar(disp,' ');
	dispWriteChar(disp,' ');
	str = dispAsByteArray(disp);
	/*        01234567890123456789012345678901 */
	strExp = "                                ";
	CuAssertStrEquals(tc,  strExp, str);
	iPos = dispGetCurs(disp);
	CuAssertIntEquals(tc, 2, iPos);
	/* optimisation : le curseur lcd n'a pas boug� */
	iPos = lcdGetCurs(_lcd);
	CuAssertIntEquals(tc, 0, iPos);
/* �crire 30 caract�res � la suite */
	/*     234567890123456789012345678901 */
	str = "                              ";
	dispWriteString(disp, str, strlen(str));
	str = dispAsByteArray(disp);
	CuAssertStrEquals(tc, strExp, str);
	iPos = dispGetCurs(disp);
	CuAssertIntEquals(tc, 32, iPos);
	/* optimisation : le curseur lcd n'a pas boug� */
	iPos = lcdGetCurs(_lcd);
	CuAssertIntEquals(tc, 0, iPos);
/* �crire en fin de 1�re ligne, espace sur espace */
	dispWriteCharAt(disp, 15, ' ');
	CuAssertStrEquals(tc,  strExp, dispAsByteArray(disp));
	iPos = dispGetCurs(disp);
	CuAssertIntEquals(tc, 16, iPos);
	iPos = lcdGetCurs(_lcd);
	CuAssertIntEquals(tc, 15, iPos);
/* �crire � la suite */
	dispWriteChar(disp, '0');
	iPos = dispGetCurs(disp);
	CuAssertIntEquals(tc, 17, iPos);
	iPos = lcdGetCurs(_lcd);
	CuAssertIntEquals(tc, 17, iPos);
	tearDown();
}

/** Tester dispClear. */
static void testClear(CuTest* tc) {
	IDisplay disp;
	char *str = "1234567890ABCDEFGHIJKLMNOPQRSTUV";
	printf("%s\n", tc->name);
	setUp();
	disp = createDisplay(_lcd);
	/*            01234567890123456789012345678901 */
	dispWriteString(disp, str, strlen(str));
	dispClear(disp);
	str = dispAsByteArray(disp);
	CuAssertStrEquals(tc,  "                                ", str);
	CuAssertIntEquals(tc,  0, dispGetCurs(disp));
	tearDown();
}

/** Tester dispHome. */
static void testHome(CuTest* tc) {
	IDisplay disp;
	printf("%s\n", tc->name);
	setUp();
	disp = createDisplay(_lcd);
	/*   01234567890123456789012345678901 */
	dispWriteStringAt(disp, 12, "onjour", strlen("onjour"));
	dispHome(disp);
	/*             01234567890123456789012345678901 */
	CuAssertStrEquals(tc,  "            onjour              ",dispAsByteArray(disp));
	CuAssertIntEquals(tc,  0, dispGetCurs(disp));
	tearDown();
}

CuSuite* displaySuite() {
	CuSuite* suite = CuSuiteNew();
	/* add test functions */
	SUITE_ADD_TEST(suite, testDisplay);
	SUITE_ADD_TEST(suite, testWriteChar);
	SUITE_ADD_TEST(suite, testWriteString);
	SUITE_ADD_TEST(suite, testWriteCharAt);
	SUITE_ADD_TEST(suite, testWriteStringAt);
	SUITE_ADD_TEST(suite, testWriteOptimizations);
	SUITE_ADD_TEST(suite, testClear);
	SUITE_ADD_TEST(suite, testHome);
	return suite;
}
