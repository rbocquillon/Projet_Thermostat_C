/**
 * @file thermui_editcons.c
 * L'interface utilisateur en mode "édition de consigne"
 *
 * Created on: 11 juil. 2012
 * @author Christophe TOMBELLE
 */

#include "thermui.h"
#include "thermui_editcons.h"
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
 * Réagir à l'événement clavier.
 *
 * @param iKey Le code de touche de cet événement.
 */
void uicProcessKey(int iKey) {
	/* iKey : 7-> SUN , 8-> MOON, 9-> OK */
	if (iKey==7) {
		if (bDay){
			/* si on est déjà en mode jour, on incrémente le seuil de température de 0,5 degrés */
			int Temp = (thermGetThreshold(_therm, bDay) + 5)%260;
			thermSetThreshold(_therm, Temp, bDay);
		}
		else
			/* sinon on change de mode, passage en mode jour */
			bDay=true;
		}
	else if (iKey==8) {
			if (!bDay){
				/* si on est déjà en mode nuit, on incrémente le seuil de température de 0,5 degrés */
				int Temp = (thermGetThreshold(_therm, false) + 5)%260;
				thermSetThreshold(_therm, Temp, false);
			}
			else
				/* sinon on change de mode, passage en mode nuit */
				bDay=false;
		}
	else if (iKey==9){
		/* retour au mode normal */
		uiSetMode(NORMAL);
	}
}

/**
 * Mettre à jour l'affichage.
 */
void uicUpdate() {
	/* Génération des 32 caractères de l'affichage */
	/* On commence par effacer et mettre le curseur en première position */
	dispHome(_disp);
	/* Affichage du numéro de programme (code ASCII) */
	dispWriteChar(_disp, 'P');
	dispWriteChar(_disp, Programme + '0');
	/* Affichage du jour dans le format demandé */
	int IndiceJour = clkGetDay(iclk);
	dispWriteString(_disp, Jours[IndiceJour], 8);
	/* Récupération du seuil de température en fonction du mode et calcul des valeurs à afficher une par une + affichage */
	int Temp = thermGetThreshold(_therm,bDay);
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
	/* Affichage du mode sélectionné (Jour/Nuit) en fonction de bDay (true=Jour) */
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

/**
 * Réagir à l'événement "entrée dans le mode 'édition de consigne'" ;
 * cet événement se produit à chaque fois que ThermUI entre en mode
 * 'édition de consigne'.
 */
void uicEntering() {
	/* Initialisation des variables lors de l'entrée dans le mode édition de consigne */
	iclk=thermGetClock(_therm);
	Programme=thermGetCurrentProg(_therm);
	IProg=thermGetProgram(_therm, Programme);
	/* L'édition de consigne commence toujours par le mode jour */
	bDay=true;
}

/**
 * Créer le ThermUIEditCons (mode "édition de consigne")
 *
 * @param therm Le Thermostat à utiliser.
 * @param disp Le Display à utiliser.
 */
void createThermUIEditCons(IThermostat therm, IDisplay disp) {
	_therm=therm;
	_disp=disp;
}
