/**
 * @file lcd.h
 * $Id$
 *
 * Created on: 25 sept. 2012
 * @author Christophe TOMBELLE
 */

#ifndef LCD_H_
#define LCD_H_

#include "port.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES FICHIERS D'EN-TETE
 * ***************************************
 */

typedef struct LCD *ILCD;

ILCD createLCD(IPort portCmd, IPort portData);

void lcdClear(ILCD lcd);
void lcdHome(ILCD lcd);
void lcdSetCurs(ILCD lcd, int iAt);
int lcdGetCurs(ILCD lcd);
void lcdWrite(ILCD lcd, char c);

#endif /* LCD_H_ */
