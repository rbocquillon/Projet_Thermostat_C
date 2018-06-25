/**
 * @file simclock.c
 * $Id$
 *
 * Created on: 1 oct. 2012
 * @author Christophe TOMBELLE
 */
#include <stdlib.h>

#include "appalloc.h"
#include "simclock.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES PROGRAMMES DE TEST
 * ***************************************
 */

typedef struct Clock_ {
	int _iDay;
	int _iHour;
	int _iMinute;
	int _iSecond;
} *Clock;

static long toMs(int iDay, int iHour, int iMin, int iSec, int iMs) {
	return ((((iDay * 24) + iHour) * 60 + iMin) * 60 + iSec) * 1000 + iMs;
}

long long clkGetTime(IClock clock) {
	Clock clk = (Clock) clock;
	return toMs(clk->_iDay, clk->_iHour, clk->_iMinute, clk->_iSecond, 0);
}

void clkSetTime(IClock clock, long long lNow) {
	Clock clk = (Clock) clock;
	clk->_iDay = lNow / MS_PER_DAY;
	clk->_iHour = (lNow / MS_PER_HOUR) % HOURS_PER_DAY;
	clk->_iMinute = (lNow / MS_PER_MINUTE) % MINUTES_PER_HOUR;
	clk->_iSecond = (lNow / MS_PER_SECOND) % SECONDS_PER_MINUTE;
}

void clkSetDay(IClock clock, int iDay) {
	Clock clk = (Clock) clock;
	clk->_iDay = iDay;
}

int clkGetDay(IClock clock) {
	Clock clk = (Clock) clock;
	return clk->_iDay;
}

void clkSetHour(IClock clock, int iHour) {
	Clock clk = (Clock) clock;
	clk->_iHour = iHour;
}

int clkGetHour(IClock clock) {
	Clock clk = (Clock) clock;
	return clk->_iHour;
}

void clkSetMinute(IClock clock, int iMinute) {
	Clock clk = (Clock) clock;
	clk->_iMinute = iMinute;
}

int clkGetMinute(IClock clock) {
	Clock clk = (Clock) clock;
	return clk->_iMinute;
}

int clkGetSecond(IClock clock) {
	Clock clk = (Clock) clock;
	return clk->_iSecond;
}

void simclkSetSecond(IClock clock, int iSecond) {
	Clock clk = (Clock) clock;
	clk->_iSecond = iSecond;
}

/**
 * Construire un objet Clock simulé.
 */
IClock createSimClock() {
	IClock clock = (IClock) appalloc(sizeof(struct Clock_), "simclk_alloc");
	Clock clk = (Clock) clock;
//	clk->_lTime = 0;
	clk->_iDay = 0;
	clk->_iHour = 0;
	clk->_iMinute = 0;
	clk->_iSecond = 0;
	return clock;
}
