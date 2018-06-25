/**
 * @file thermui_editprog.c
 *
 * Created on: 11 juil. 2012
 * @author Christophe TOMBELLE
 */
#include <stdlib.h>
#include "thermui.h"
#include "thermui_editprog.h"
#include "keyboard.h"

/* TODO : d�clarer ici (en static) les variables n�cessaires */

static IThermostat thermAffichage;
static IDisplay dispAffichage;

static int temperatureAfichage;
static int programmeAffichage;
static int jourAffichage;
static int heureActuelle;
static int heureSuivante;
static char nuitJourAffichage1;
static char nuitJourAffichage2;
static bool isSave;


char programmJourNuit (int heure){

	IProgram program = thermGetProgram(thermAffichage, programmeAffichage);

	//Programme Hors-gel
	if(programmeAffichage == 0){
		return 'H';
	}else if (programmeAffichage == 1){
		return 'N';
	} else if ( programmeAffichage == 7){
		return 'J';
	}else {
		if(progIsDay(program,heure))
			return 'J';
		else
			return 'N';
	}
}


void miseajourVariableAffichage (){

	temperatureAfichage = thermGetTemp(thermAffichage);
	nuitJourAffichage1 = programmJourNuit(heureSuivante-1);
}


/**
 * R�agir � l'�v�nement clavier.
 *
 * @param iKey Le code de touche de cet �v�nement.
 */
void uipProcessKey(int iKey) {
	switch(iKey){
		case NPROG :
			programmeAffichage ++;
			thermSetProg(thermAffichage,programmeAffichage,jourAffichage);
			if(programmeAffichage >= 8){
				programmeAffichage = 0;
			}
			thermSetProg(thermAffichage,programmeAffichage,jourAffichage);
			isSave = false;
			miseajourVariableAffichage();
			break;
		case PROG :
			jourAffichage++;
			if(jourAffichage >= 7){
				jourAffichage = 0;
			}
			programmeAffichage = thermGetCurrentProg(thermAffichage);
			isSave = false;
			miseajourVariableAffichage();
			break;
		case SUN:
			isSave = true;

			heureSuivante++;

			if(heureSuivante >=24)
				heureSuivante = 0;

			if(programmeAffichage != 0 && programmeAffichage != 1 && programmeAffichage != 7){
				IProgram program = thermGetProgram(thermAffichage, programmeAffichage);
				progSetChoice(program,true,heureSuivante-1);
				nuitJourAffichage2 = 'N';
			}else{
				nuitJourAffichage2 = programmJourNuit(heureSuivante);
			}
			miseajourVariableAffichage();
			break;
		case MOON:
			//Program doit �tre diff�rent de programme 0,1,7
			isSave = true;
			heureSuivante++;

			if(heureSuivante >=24)
				heureSuivante = 0;

			if(programmeAffichage != 0 && programmeAffichage != 1 && programmeAffichage != 7){
				IProgram program2 = thermGetProgram(thermAffichage, programmeAffichage);
				progSetChoice(program2,false,heureSuivante-1);
				nuitJourAffichage2 = 'N';
			}else{
				nuitJourAffichage2 = programmJourNuit(heureSuivante);
			}

			miseajourVariableAffichage();
			break;
		case OK :
			uiSetMode(NORMAL);
			break;
	}
}


/**
 *Convertie un chiffre (0-9) en ASCII
 *
 *@param car : chiffre � convertir
 *
 *Return -1 en cas d'erreur sinon le chiffre en ASCII
 */
int conversionIntegerASCII(int car){
	if(car >=0 && car < 10)
		return 48 + car;
	else
		return -1;
}

/**
 *Affiche un entier de la forme 0N (nombre >= 10) ou N (chiffre 0-9)
 *
 *@param valeur : valeur � afficher
 *@param dispAffichage : Zone d'affichage
 */
void afficherValeurEntiere (int valeur, IDisplay dispAffichage){
	if (valeur < 10) {
		int valeurAscii = conversionIntegerASCII(valeur);
		dispWriteString(dispAffichage,"0",1);
		dispWriteChar(dispAffichage,valeurAscii);
	}
	else {
	    char BufferValeur[20];
	    itoa(valeur,BufferValeur,10);
		dispWriteString(dispAffichage,BufferValeur,2);
	}
}


/**
 *Affiche la partie d�cimale d'un nombre �crit de la forme : 255 = 25.5 ==> Affiche 5
 *
 *@param valeur : valeur � afficher
 *@param dispAffichage : Zone d'affichage
 */
void afficherValeurDecimale (int valeur, IDisplay dispAffichage){
	int valeurDecimale = valeur%10;
	int valeurAscii = conversionIntegerASCII(valeurDecimale);
	dispWriteChar(dispAffichage,valeurAscii);
}


/**
 * Mettre � jour l'affichage.
 */
void uipUpdate() {

	dispHome(dispAffichage);

	int ProgrammeAffichageASCII = conversionIntegerASCII(programmeAffichage);

	//Mise � jour de la variable temp�rature :
	temperatureAfichage = thermGetTemp(thermAffichage);

	//Affichage Programme
	dispWriteString(dispAffichage,"P",1);
	dispWriteChar(dispAffichage,ProgrammeAffichageASCII);
	dispWriteString(dispAffichage," ",1);

	//Affichage Jours
	switch(jourAffichage){
	case 0 : //dimanche
		dispWriteString(dispAffichage,"Diman. ",7);
		break;
	case 1 : // Lundi
		dispWriteString(dispAffichage,"Lundi  ",7);
		break;
	case 2 : // Mardi
		dispWriteString(dispAffichage,"Mardi  ",7);
		break;
	case 3 : // Mercredi
		dispWriteString(dispAffichage,"Mercr. ",7);
		break;
	case 4 : // Jeudi
		dispWriteString(dispAffichage,"Jeudi  ",7);
		break;
	case 5 : // Vendredi
		dispWriteString(dispAffichage,"Vendr. ",7);
		break;
	case 6 : // Vendredi
		dispWriteString(dispAffichage,"Samedi ",7);
		break;
	}

	//Affichage Temp�rature
	afficherValeurEntiere(temperatureAfichage/10,dispAffichage);
	dispWriteString(dispAffichage,",",1);
	afficherValeurDecimale(temperatureAfichage,dispAffichage);
	dispWriteString(dispAffichage,"�C",2);

	//Affichage Heures
	if(!isSave){
		dispWriteString(dispAffichage,"        ",8);
		afficherValeurEntiere(heureSuivante,dispAffichage);
		dispWriteString(dispAffichage,":00 ",4);
		dispWriteString(dispAffichage,&nuitJourAffichage1,1);
		dispWriteString(dispAffichage," ",1);
	}else{
		afficherValeurEntiere(heureSuivante-1,dispAffichage);
		dispWriteString(dispAffichage,":00 ",4);
		dispWriteString(dispAffichage,&nuitJourAffichage1,1);

		dispWriteString(dispAffichage," ",1);

		afficherValeurEntiere(heureSuivante,dispAffichage);
		dispWriteString(dispAffichage,":00 ",4);
		dispWriteString(dispAffichage,&nuitJourAffichage2,1);
		dispWriteString(dispAffichage," ",1);
	}
}


/**
 * Retourne le caract�re � afficher selon le programme et l'heure: H = Hors-gel ; J = Jour ; N=Nuit
 *
 *@param heure : heure � du programme
 */

/**
 * R�agir � l'�v�nement "entr�e dans le mode '�dition de programme'" ;
 * cet �v�nement se produit � chaque fois que ThermUI entre en mode
 * '�dition de programme'.
 */
void uipEntering() {

	IClock clock = thermGetClock(thermAffichage);

	isSave = false;

	temperatureAfichage = thermGetTemp(thermAffichage);
	programmeAffichage = thermGetCurrentProg(thermAffichage);

	jourAffichage = clkGetDay(clock);

	heureActuelle = clkGetHour(clock);
	heureSuivante = heureActuelle;

	nuitJourAffichage1 = programmJourNuit(heureActuelle);
	nuitJourAffichage2 = programmJourNuit(heureSuivante);
}

/**
 * Cr�er le ThermUIEditProg (mode "�dition de programme")
 *
 * @param therm Le Thermostat � utiliser.
 * @param disp Le Display � utiliser.
 */
void createThermUIEditProg(IThermostat therm, IDisplay disp) {
	thermAffichage = therm;
	dispAffichage = disp;

}
