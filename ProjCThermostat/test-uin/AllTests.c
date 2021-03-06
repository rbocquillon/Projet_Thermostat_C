/**
 * @file CThermostatTests.c
 * $Id$
 *
 * Created on: 11 juil. 2012
 * @author Christophe TOMBELLE
 */
#include <stdio.h>

#include "thermui_normalTest.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES PROGRAMMES DE TEST
 * ***************************************
 */

void runAllTests(void) {
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	CuSuiteAddSuite(suite, thermui_normalSuite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
}


int main() {
	runAllTests();
	return 0;
}
