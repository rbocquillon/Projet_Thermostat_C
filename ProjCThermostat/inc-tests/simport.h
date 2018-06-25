/*
 * @file port.h
 *
 * Created on: 11 juil. 2012
 * @author tombelle
 */

#ifndef SIMPORT_H_
#define SIMPORT_H_

#include "port.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES FICHIERS D'EN-TETE
 * ***************************************
 */

void portSimInput(IPort port, int iIn);
IPort createSimPort(void);

#endif /* SIMPORT_H_ */
