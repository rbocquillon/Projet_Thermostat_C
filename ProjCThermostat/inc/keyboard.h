/*
 * @file keyboard.h
 *
 * Created on: 11 juil. 2012
 * @author tombelle
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "itime.h"
#include "port.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES FICHIERS D'EN-TETE
 * ***************************************
 */

#define TEMP 1
#define DAY 2
#define HOUR 3
#define MINUTE 4
#define PROG 5
#define NPROG 6
#define SUN 7
#define MOON 8
#define OK 9

#define REPEAT_DELAY 300

typedef struct Keyboard *IKeyboard;

IKeyboard createKeyboard(IPort portLeds, IPort portDip);

int kbdGetKey(IKeyboard kbd);

#endif /* KEYBOARD_H_ */
