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
 * Tout effacer et positionner le curseur en 0 (en haut � gauche).
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
 * Positionner le curseur en position 0 (en haut � gauche).
 *
 * @param disp Ce Display.
 */
void dispHome(IDisplay disp) {
	lcdHome(disp->_lcd);
	disp->_iPos = disp->_iPosHard = 0;
}

/**
 * Ecrire un caract�re � la position courante du curseur.
 *
 * @param disp Ce Display.
 * @param car Le caract�re � �crire
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
 * Positionner le curseur � la position sp�cifi�e,
 * puis �crire le caract�re sp�cifi�.
 *
 * @param disp Ce Display.
 * @param iAt La position du curseur.
 * @param car Le caract�re � �crire
 */
void dispWriteCharAt(IDisplay disp, int iAt, int car) {
	dispSetCurs(disp, iAt);
	dispWriteChar(disp, car);
}

/**
 * Ecrire un tableau de caract�res � partir de la position courante du
 * curseur. Les �ventuels caract�res en trop sont ignor�s.
 *
 * @param disp Ce Display.
 * @param ya Le tableau des caract�res � �crire.
 * @param yaLength Le nombre de caract�res � �crire (dans la limite des
 * places disponibles)
 */
void dispWriteString(IDisplay disp, char *ya, int yaLength) {
	int i=0;
	while (i<yaLength && disp->_iPos<32) {
		dispWriteChar(disp, ya[i++]);
	}
}

/**
 * D�finier la position du curseur, puis �crire les caract�res. Le �ventuels
 * caract�res en trop sont ignor�s.
 *
 * @param disp Ce Display.
 * @param iFrom La position du curseur.
 * @param ya Le tableau des caract�res � �crire.
 * @param yaLength Le nombre de caract�res � �crire (dans la limite des
 * places disponibles)
 */
void dispWriteStringAt(IDisplay disp, int iFrom, char *ya, int yaLength) {
	dispSetCurs(disp, iFrom);
	dispWriteString(disp, ya, yaLength);
}

/**
 * Fournir l'image m�moire des caract�res acutellement affich�s sur le LCD.
 * Pour consultation seulement, modifier le tableau brise la coh�rence avec
 * le LCD r�el.
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
 * @return La position acutelle du curseur [0..32]. La position est 32 apr�s
 * l'�criture d'un caract�re en derni�re position (31).
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
 * Cr�er un Display se fondant sur le LCD sp�cifi�.
 *
 * @param lcd Le LCD � utiliser pour l'affichage.
 * @return L'adresse du Display cr��.
 */
IDisplay createDisplay(ILCD lcd) {
	IDisplay disp = (IDisplay) appalloc(sizeof(struct Display), "dispAlloc");
	disp->_lcd = lcd;
	dispClear(disp);
	return disp;
}
