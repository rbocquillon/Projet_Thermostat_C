/**
 * @file keybport.h
 * $Id$
 *
 * Created on: 28 sept. 2012
 * @author Christophe TOMBELLE
 */

#ifndef KEYBPORT_H_
#define KEYBPORT_H_

#include "port.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES FICHIERS D'EN-TETE
 * ***************************************
 */

IPort createKeybPort(void);

void kbpSetLigPort(IPort portLeds, IPort portLig);
void kbpSetCode(IPort portLeds, int iCol, int iLig);
void kbpSimInput(IPort port, int iIn);

#endif /* KEYBPORT_H_ */
