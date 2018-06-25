/**
 * @file thermui_normal.c
 *
 * Created on: 11 juil. 2012
 * @author Christophe TOMBELLE
 */

#include "thermui.h"
#include "thermui_normal.h"
#include "keyboard.h"
#include <stdlib.h>

static IThermostat _therm;
static IDisplay _disp;

static int Programme;
static IProgram IProg;
static bool bDay;
static IClock iclk;
static char* Jours[7]={" Diman. "," Lundi  "," Mardi  "," Mercr. "," Jeudi  "," Vendr. "," Samedi "};


/**
 * Réagir à l'événement "entrée dans le mode 'normal'" ; cet événement se
 * produit à chaque fois que ThermUI entre en mode 'normal'.
 */
void uinEntering() {
	/* Initialisation des variables lors de l'entrée dans le mode normal */
	iclk=thermGetClock(_therm);
	Programme=thermGetCurrentProg(_therm);
	IProg=thermGetProgram(_therm, Programme);
	int Hour = clkGetHour(iclk);
	if (IProg!=NULL)
		bDay=progIsDay(IProg, Hour);
}

/**
 * Réagir à l'événement clavier.
 *
 * @param iKey Le code de touche de cet événement.
 */
void uinProcessKey(int iKey) {
	/* iKey : 1-> TEMP , 2-> DAY, 3-> HOUR, 4->MINUTE, 5-> PROG */
	if (iKey==2) {
		/* on incrémente le jour (on retourne à 0 si on atteint 7) */
		int Day=(clkGetDay(iclk)+1)%7;
		clkSetDay(iclk, Day);
		}
	else if (iKey==3) {
		/* on incrémente l'heure (on retourne à 0 si on atteint 24) */
		int Hour=(clkGetHour(iclk)+1)%24;
		clkSetHour(iclk, Hour);
		}
	else if (iKey==4) {
		/* on incrémente la minute (on retourne à 0 si on atteint 60) */
		int Min=(clkGetMinute(iclk)+1)%60;
		clkSetMinute(iclk, Min);
		}
	else if (iKey==1) {
		/* entrer dans le mode édition de consigne */
		uiSetMode(EDIT_CONS);
		}
	else if (iKey==5) {
		/* entrer dans le mode édition de programme */
		uiSetMode(EDIT_PROG);
		}
}


/**
 * Mettre à jour l'affichage.
 */
void uinUpdate() {
	/* Génération des 32 caractères de l'affichage */
		/* On commence par effacer et mettre le curseur en première position */
		dispHome(_disp);
		/* Affichage du numéro de programme (code ASCII) */
		dispWriteChar(_disp, 'P');
		/* Récupération du programme actuel */
		Programme=thermGetCurrentProg(_therm);
		dispWriteChar(_disp, Programme + '0');
		/* Affichage du jour dans le format demandé */
		int IndiceJour = clkGetDay(iclk);
		dispWriteString(_disp, Jours[IndiceJour], 8);
		/* Récupération de la température actuelle et calcul des valeurs à afficher une par une + affichage */
		int Temp = thermGetTemp(_therm);
		int diz=(int) Temp/100;
		int unit=(int) (Temp-diz*100)/10;
		int reste=(int) Temp-diz*100-unit*10;
		dispWriteChar(_disp, diz + '0');
		dispWriteChar(_disp, unit + '0');
		dispWriteChar(_disp, ',');
		dispWriteChar(_disp, reste + '0');
		dispWriteChar(_disp, '°');
		dispWriteChar(_disp, 'C');
		/* Récupération de l'heure et calcul des valeurs à afficher une par une + affichage */
		int Hour=clkGetHour(iclk);
		int h1=(int) Hour/10;
		int h2=(int) Hour-h1*10;
		dispWriteChar(_disp, h1 + '0');
		dispWriteChar(_disp, h2 + '0');
		dispWriteChar(_disp, ':');
		/* Récupération des minutes et calcul des valeurs à afficher une par une + affichage */
		int Min=clkGetMinute(iclk);
		int m1=(int) Min/10;
		int m2=(int) Min-m1*10;
		dispWriteChar(_disp, m1 + '0');
		dispWriteChar(_disp, m2 + '0');
		dispWriteChar(_disp, ':');
		/* Récupération des secondes et calcul des valeurs à afficher une par une + affichage */
		int Sec=clkGetSecond(iclk);
		int s1=(int) Sec/10;
		int s2=(int) Sec-s1*10;
		dispWriteChar(_disp, s1 + '0');
		dispWriteChar(_disp, s2 + '0');
		dispWriteChar(_disp, ' ');
		/* Calcul puis affichage du mode sélectionné (Jour/Nuit/Hors gel) en fonction de bDay (true=Jour) et du pointeur IProg */
		IProg=thermGetProgram(_therm, Programme);
		Hour = clkGetHour(iclk);
		if (IProg!=NULL)
			bDay=progIsDay(IProg, Hour);
		if (IProg!=NULL){
			if (bDay) {
				dispWriteChar(_disp, 'J');
				dispWriteChar(_disp, 'o');
				dispWriteChar(_disp, 'u');
				dispWriteChar(_disp, 'r');
				dispWriteChar(_disp, ' ');
				dispWriteChar(_disp, ' ');
			}
			else {
				dispWriteChar(_disp, 'N');
				dispWriteChar(_disp, 'u');
				dispWriteChar(_disp, 'i');
				dispWriteChar(_disp, 't');
				dispWriteChar(_disp, ' ');
				dispWriteChar(_disp, ' ');
			}
		}
		else {
			dispWriteChar(_disp, 'H');
			dispWriteChar(_disp, '.');
			dispWriteChar(_disp, ' ');
			dispWriteChar(_disp, 'G');
			dispWriteChar(_disp, 'e');
			dispWriteChar(_disp, 'l');
		}
		/* Afficher * si la chaudière est allumée */
		if ( thermIsBoilerOn(_therm))
			dispWriteChar(_disp, '*');
		else
			dispWriteChar(_disp, ' ');
}

/**
 * Créer le ThermUINormal (mode "normal")
 *
 * @param therm Le Thermostat à utiliser.
 * @param disp Le Display à utiliser.
 */
void createThermUINormal(IThermostat therm, IDisplay disp) {
	_therm=therm;
	_disp=disp;
	/* Obligation d'initialiser, comme dans l'entering, car on est de base dans le mode normal sans forcément passer par l'entering */
	iclk=thermGetClock(_therm);
	Programme=thermGetCurrentProg(_therm);
	IProg=thermGetProgram(_therm, Programme);
	int Hour = clkGetHour(iclk);
	if (IProg!=NULL)
		bDay=progIsDay(IProg, Hour);
}
