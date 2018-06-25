/**
 * @file display.c
 *
 * LCD display. Provides more of less the same features as the LCD but
 * the state of the LCD may be read back thanks to asByteArray function.
 *
 * Created on: 11 juil. 2012
 * @author Christophe TOMBELLE
 */
#include <stdio.h>
#include <stdlib.h>

#include "appalloc.h"
#include "display.h"

/*
 * ***************************
 * NE PAS MODIFIER CE FICHIER
 * ***************************
 */

#define MAX 32

struct Display {
	ILCD _lcd;
	char _ya[MAX+1];
	int _iPos;
	int _iPosHard;
};

static void dispSetCurs(IDisplay disp, int iAt);

/**
 * Tout effacer et positionner le curseur en 0 (en haut à gauche).
 *
 * @param disp Ce Display.
 */
void dispClear(IDisplay disp) {
	int i;
	lcdClear(disp->_lcd);
	disp->_iPos = disp->_iPosHard = 0;
	for (i=0; i<MAX; i++) {
		disp->_ya[i]=' ';
	}
	disp->_ya[MAX]='\0';
}

/**
 * Positionner le curseur en position 0 (en haut à gauche).
 *
 * @param disp Ce Display.
 */
void dispHome(IDisplay disp) {
	lcdHome(disp->_lcd);
	disp->_iPos = disp->_iPosHard = 0;
}

/**
 * Ecrire un caractère à la position courante du curseur.
 *
 * @param disp Ce Display.
 * @param car Le caractère à écrire
 */
void dispWriteChar(IDisplay disp, int car) {
	if (disp->_iPos < MAX) {
		if (disp->_ya[disp->_iPos]!=car) {
			dispSetCurs(disp, disp->_iPos);
			disp->_ya[disp->_iPos++]=(char) car;


		    lcdWrite(disp->_lcd, car);


		    disp->_iPosHard++;
		}
		else {
			disp->_iPos++;
		}
	}
}

/**
 * Positionner le curseur à la position spécifiée,
 * puis écrire le caractère spécifié.
 *
 * @param disp Ce Display.
 * @param iAt La position du curseur.
 * @param car Le caractère à écrire
 */
void dispWriteCharAt(IDisplay disp, int iAt, int car) {
	dispSetCurs(disp, iAt);
	dispWriteChar(disp, car);
}

/**
 * Ecrire un tableau de caractères à partir de la position courante du
 * curseur. Les éventuels caractères en trop sont ignorés.
 *
 * @param disp Ce Display.
 * @param ya Le tableau des caractères à écrire.
 * @param yaLength Le nombre de caractères à écrire (dans la limite des
 * places disponibles)
 */
void dispWriteString(IDisplay disp, char *ya, int yaLength) {
	int i=0;
	while (i<yaLength && disp->_iPos<32) {
		dispWriteChar(disp, ya[i++]);
	}
}

/**
 * Définier la position du curseur, puis écrire les caractères. Le éventuels
 * caractères en trop sont ignorés.
 *
 * @param disp Ce Display.
 * @param iFrom La position du curseur.
 * @param ya Le tableau des caractères à écrire.
 * @param yaLength Le nombre de caractères à écrire (dans la limite des
 * places disponibles)
 */
void dispWriteStringAt(IDisplay disp, int iFrom, char *ya, int yaLength) {
	dispSetCurs(disp, iFrom);
	dispWriteString(disp, ya, yaLength);
}

/**
 * Fournir l'image mémoire des caractères acutellement affichés sur le LCD.
 * Pour consultation seulement, modifier le tableau brise la cohérence avec
 * le LCD réel.
 *
 * @param disp Ce Display.
 */
char *dispAsByteArray(IDisplay disp) {
	return disp->_ya;
}

/**
 * Fournir la position actuelle du curseur.
 *
 * @param disp Ce Display.
 * @return La position acutelle du curseur [0..32]. La position est 32 après
 * l'écriture d'un caractère en dernière position (31).
 */
int dispGetCurs(IDisplay disp) {
	return disp->_iPos;
}

static void dispSetCurs(IDisplay disp, int iAt) {
	disp->_iPos=iAt;
	if (disp->_iPos != disp->_iPosHard || disp->_iPos==16) {
		disp->_iPosHard=disp->_iPos;
		lcdSetCurs(disp->_lcd, disp->_iPosHard);
	}
}

/**
 * Créer un Display se fondant sur le LCD spécifié.
 *
 * @param lcd Le LCD à utiliser pour l'affichage.
 * @return L'adresse du Display créé.
 */
IDisplay createDisplay(ILCD lcd) {
	IDisplay disp = (IDisplay) appalloc(sizeof(struct Display), "dispAlloc");
	disp->_lcd = lcd;
	dispClear(disp);
	return disp;
}
