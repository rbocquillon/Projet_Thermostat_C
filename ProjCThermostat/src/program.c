/**
 * @file program.c
 * Programme journalier. D�finit le choix heure par heure entre les consignes Jour
 * et Nuit.
 *
 * Created on: 11 juil. 2012
 * @author Christophe TOMBELLE
 */
#include "appalloc.h"
#include "program.h"

struct Program {
	// tableau contenant un booleen en fonction de l'heure
	bool hour[HOURS_PER_DAY];
	// booleen qui renvoit true si setchoice est actif
	bool choice;
};

/**
 * Cr�er un Program.
 *
 * @return L'adresse du Program cr��.
 */
IProgram createProgram() {
	int i;
	IProgram prog = malloc(sizeof(struct Program));
	for( i=0;i< HOURS_PER_DAY; i++){
		prog->hour[i] = false;
		prog->choice = true;
	}
	return prog ;
}

/**
 * Rendre le programme non modifiable (op�ration non r�versible).
 * @param prog L'adresse du programme
 */
void progSetFixed( IProgram prog ) {
	prog->choice = false;
}

/**
 * D�finir le choix de consigne pour l'heure sp�cifi�e.
 * @param prog L'adresse du programme
 * @param bDay false pour choisir la consigne Nuit, true pour la consigne Jour.
 * @param iHour Heure [0-23]
 */
void progSetChoice( IProgram prog, bool bDay, int iHour ) {
	if(prog->choice)
	prog->hour[iHour] = bDay;
}

/**
 * Fournir la consigne pour l'heure sp�cifi�e.
 * @param prog L'adresse du programme
 * @param iHour Heure [0-23].
 * @return true pour consigne Jour, false pour consigne Nuit.
 */
bool progIsDay( IProgram prog, int iHour ) {
	return prog->hour[iHour];
}
