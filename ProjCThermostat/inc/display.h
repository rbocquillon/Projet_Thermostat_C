/*
 * @file display.h
 *
 * Created on: 11 juil. 2012
 * @author tombelle
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "lcd.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES FICHIERS D'EN-TETE
 * ***************************************
 */

typedef struct Display *IDisplay;

void dispClear(IDisplay disp);
void dispHome(IDisplay disp);
void dispWriteChar(IDisplay disp, int i);
void dispWriteCharAt(IDisplay disp, int iAt, int i);
void dispWriteString(IDisplay disp, char *ya, int yaLength);
void dispWriteStringAt(IDisplay disp, int iFrom, char *ya, int yaLength);
char *dispAsByteArray(IDisplay disp);
int dispGetCurs(IDisplay disp);

IDisplay createDisplay(ILCD lcd);

#endif /* DISPLAY_H_ */
