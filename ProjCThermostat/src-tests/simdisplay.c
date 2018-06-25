/**
 * @file simdisplay.c
 * $Id$
 *
 * Created on: 1 oct. 2012
 * @author Christophe TOMBELLE
 */

#include "appalloc.h"
#include "simdisplay.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES PROGRAMMES DE TEST
 * ***************************************
 */

#define MAX 32

typedef struct Display_ {
	char _ya[MAX+1];
	int _iPos;
	int clearCount;
} *Display;

/**
 * Créer une instance de Display utilisant le LCD spécifié.
 *
 */
IDisplay createSimDisplay() {
	Display disp = (Display) appalloc(sizeof(struct Display_), "dispAlloc");
	IDisplay dsp = (IDisplay) disp;
	disp->clearCount = 0;
	dispClear(dsp);
	return dsp;
}

/**
 * Effacer tout et positionner le curseur en 0 (en haut à gauche).
 *
 * @param this Ce Display simulé
 */
void dispClear(IDisplay this) {
	Display disp = (Display) this;
	int i;
	disp->_iPos = 0;
	for (i = 0; i < MAX; i++)
		disp->_ya[i] = (char) ' ';
	disp->_ya[MAX]='\0';
	disp->clearCount++;
}

int dispGetClearCount(IDisplay this) {
	Display disp = (Display) this;
	return disp->clearCount;
}

/**
 * Positionner le curseur en 0 (en haut à gauche).
 *
 * @param this Ce Display simulé
 */
void dispHome(IDisplay this) {
	Display disp = (Display) this;
	disp->_iPos = 0;
}

/**
 * Ecrire un caractère à la position du curseur.
 *
 * @param this Ce Display simulé
 * @param iCar
 *            Caractère à écrire.
 */
void dispWriteChar(IDisplay this, int iCar) {
	Display disp = (Display) this;
	if (disp->_iPos < 32) {
		disp->_ya[disp->_iPos] = (char) iCar;
		disp->_iPos++;
	}
}

/**
 * Placer le curseur à la position spécifiée et y écrire un caractère.
 *
 * @param this Ce Display simulé
 * @param iAt
 *            Position du curseur.
 * @param y
 *            Caractère à écrire.
 */
void dispWriteCharAt(IDisplay this, int iAt, int i) {
	Display disp = (Display) this;
	if (iAt >= 0 && iAt < 32) {
		disp->_iPos = iAt;
		disp->_ya[disp->_iPos] = (char) i;
		disp->_iPos++;
	}
}

/**
 * Ecrire un tableau de caractères à partir de la position du curseur. Les
 * éventuels caractères en trop sont ignorés.
 *
 * @param this Ce Display simulé
 * @param ya
 *            Tableau des caractères à écrire.
 */
void dispWriteString(IDisplay this, char *ya, int iLen) {
	Display disp = (Display) this;
	int i;
	for (i = 0; i < iLen && disp->_iPos < 32; i++, disp->_iPos++)
		disp->_ya[disp->_iPos] = ya[i];
}

/**
 * Placer le curseur à la position spécifiée et y écrire des caractères. Les
 * éventuels caractères en trop sont ignorés.
 *
 * @param this Ce Display simulé
 * @param iFrom
 *            Position du curseur.
 * @param ya
 *            Tableau des caractères à écrire.
 */
void dispWriteStringAt(IDisplay this, int iFrom, char *ya, int iLen) {
	Display disp = (Display) this;
	if (iFrom >= 0 && iFrom < 32) {
		int i;
		disp->_iPos = iFrom;
		for (i = 0; i < iLen && disp->_iPos < 32; i++, disp->_iPos++)
			disp->_ya[disp->_iPos] = ya[i];
	}
}

/**
 * Fournir l'image mémoire des caractères du LCD, pour consultation
 * seulement. Modifier le tableau de bytes détruirait la cohérence entre le
 * LCD réel et son image mémoire.
 *
 * @param this Ce Display simulé
 * @return Tableau des caractères affichés sur le LCD.
 */
char *dispAsByteArray(IDisplay this) {
	Display disp = (Display) this;
	return disp->_ya;
}

/**
 * Fournir la position du curseur.
 *
 * @param this Ce Display simulé
 * @return Position du curseur [0-31]
 */
int dispGetCurs(IDisplay this) {
	Display disp = (Display) this;
	return disp->_iPos;
}
