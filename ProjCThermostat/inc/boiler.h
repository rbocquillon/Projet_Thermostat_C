/*
 * @file boiler.h
 *
 * Created on: 11 juil. 2012
 * @author tombelle
 */

#ifndef BOILER_H_
#define BOILER_H_

#include <stdbool.h>
#include "port.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES FICHIERS D'EN-TETE
 * ***************************************
 */

typedef struct Boiler *IBoiler;

IBoiler createBoiler(IPort port);

void boilerSetOn(IBoiler boiler, bool bOn);
bool boilerIsOn(IBoiler boiler);

#endif /* BOILER_H_ */
