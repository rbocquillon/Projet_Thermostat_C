/**
 * @file simdisplay.h
 * $Id$
 *
 * Created on: 1 oct. 2012
 * @author Christophe TOMBELLE
 */

#ifndef SIMDISPLAY_H_
#define SIMDISPLAY_H_

#include "display.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES FICHIERS D'EN-TETE
 * ***************************************
 */

IDisplay createSimDisplay(void);
int dispGetClearCount(IDisplay disp);
int dispGetCurs(IDisplay disp);

#endif /* SIMDISPLAY_H_ */
