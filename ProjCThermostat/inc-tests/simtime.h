/**
 * @file simtime.h
 * $Id$
 *
 * Created on: 26 sept. 2012
 * @author Christophe TOMBELLE
 */

#ifndef SIMTIME_H_
#define SIMTIME_H_

#include "itime.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES FICHIERS D'EN-TETE
 * ***************************************
 */

void simNow(void);
void setNow(long long llNow);
void elapseLong(long lDuration);
void elapse(int iDay, int iHour, int iMin, int iSec, int iMs);

#endif /* SIMTIME_H_ */
