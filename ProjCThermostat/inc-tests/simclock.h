/**
 * @file simclock.h
 * $Id$
 *
 * Created on: 28 sept. 2012
 * @author Christophe TOMBELLE
 */

#ifndef SIMCLOCK_H_
#define SIMCLOCK_H_

#include "clock.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES FICHIERS D'EN-TETE
 * ***************************************
 */

void simclkSetSecond(IClock clk, int iSecond);

IClock createSimClock(void);

#endif /* SIMCLOCK_H_ */
