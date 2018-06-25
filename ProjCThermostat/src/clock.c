/**
 * @file clock.c
 *
 * Horloge hebdomadaire fournissant les informations jour,
 * heure, minute et seconde.
 *
 * Created on: 11 juil. 2012
 * @author Christophe TOMBELLE
 */
#include "appalloc.h"

#include "clock.h"
#include "itime.h"

struct Clock {
	/* Représente le temps écoulé entre le lancement du programme et l'origine de l'horloge */
	long long TempsEcoule;
};

/**
 * Fournir l'instant courant comme nombre de millisecondes écoulées depuis
 * le début de la semaine (dimanche 00:00:00)
 *
 * @param clock Cette Clock.
 * @return Le nombre de millisecondes.
 */
long long clkGetTime(IClock clock) {
	/* Calcul du temps écoulé depuis le début de l'horloge modulo une semaine */
	long long Time =(now()-clock->TempsEcoule)%MS_PER_WEEK;
	return Time;
}

/**
 * Définir l'instant courant pour la mise à l'heure de cette Clock.
 *
 * @param clock Cette Clock.
 * @param msNow Nombre de millisecondes écoulées depuis le début de la semaine.
 */
void clkSetTime(IClock clock, long long msNow) {
	/* Redéfinition de l'origine de l'horloge */
	clock->TempsEcoule=now()- msNow;
}

/**
 * Fournir le jour actuel.
 *
 * @param clock Cette Clock.
 * @return Le jour actuel [0-6] avec 0 pour Dimanche.
 */
int clkGetDay(IClock clock) {
	/* Calcul du nombre de jours puis du numéro de jour actuel [0-6] par modulo 7 (0 = dimanche) */
	int NbJours = (int) clkGetTime(clock)/(MS_PER_DAY);
	return NbJours%7;
}

/**
 * Fournir l'heure actuelle.
 *
 * @param clock Cette Clock.
 * @return L'heure actuelle [0-23].
 */
int clkGetHour(IClock clock) {
	/* Calcul du nombre d'heures total puis des heures du jour actuel par modulo 24 */
	int NbHeures = (int) clkGetTime(clock)/(MS_PER_HOUR);
	return NbHeures%24;
}

/**
 * Fournir la minute actuelle.
 *
 * @param clock Cette Clock.
 * @return La minute actuelle [0..59].
 */
int clkGetMinute(IClock clock) {
	/* Calcul du nombre de minutes total puis des minutes de l'heure actuelle par modulo 60 */
	int NbMinutes = (int) clkGetTime(clock)/(MS_PER_MINUTE);
	return NbMinutes%60;
}

/**
 * Fournir la seconde actuelle.
 *
 * @param clock Cette Clock.
 * @return La seconde actuelle [0..59].
 */
int clkGetSecond(IClock clock) {
	/* Calcul du nombre de secondes total puis des secondes de l'heure actuelle par modulo 60 */
	int NbSecondes = (int) clkGetTime(clock)/(MS_PER_SECOND);
	return NbSecondes%60;
}

/**
 * Définir le jour actuel pour la mise à l'heure de cette Clock.
 *
 * @param clock Cette Clock.
 * @param iDay Le jour actuel [0-6] avec 0 pour Dimanche.
 */
void clkSetDay(IClock clock, int iDay) {
	/* Calcul du nombre de jours rajoutés modulo 7 */
	int NbJoursRajoute = (iDay-clkGetDay(clock))%7;
	/* Calcul du nombre de millisecondes écoulées depuis le début de l'horloge mise à jour */
	int NbMS = (NbJoursRajoute)*MS_PER_DAY + clkGetTime(clock);
	/* Mise à jour de l'horloge */
	clkSetTime(clock, NbMS);
}

/**
 * Définir l'heure actuelle pour la mise à l'heure de cette Clock.
 *
 * @param clock Cette Clock.
 * @param iHour L'heure actuelle [0-23].
 */
void clkSetHour(IClock clock, int iHour) {
	/* Calcul du nombre d'heures rajoutées modulo 24 */
	int NbHeuresRajoute = (iHour-clkGetHour(clock))%24;
	/* Calcul du nombre de millisecondes écoulées depuis le début de l'horloge mise à jour */
	int NbMS = (NbHeuresRajoute)*MS_PER_HOUR + clkGetTime(clock);
	/* Mise à jour de l'horloge */
	clkSetTime(clock, NbMS);
}

/**
 * Définir la minute actuelle pour la mise à l'heure de cette Clock et mettre à 0 les secondes et millisecondes.
 *
 * @param clock Cette Clock.
 * @param iMinute La minute actuelle [0..59].
 */
void clkSetMinute(IClock clock, int iMinute) {
	/* Calcul du nombre de minutes rajoutées modulo 60 */
	int NbMinutesRajoute = (iMinute-clkGetMinute(clock))%60;
	/* Calcul du nombre de millisecondes écoulées depuis le début de l'horloge mise à jour sans tenir compte des secondes et ms */
	int NbMinutes = (NbMinutesRajoute + clkGetMinute(clock))*MS_PER_MINUTE;
	int NbHeures = clkGetHour(clock)*MS_PER_HOUR;
	int NbJours = clkGetDay(clock)*MS_PER_DAY;
	int NbMS =  NbMinutes + NbHeures + NbJours;
	/* Mise à jour de l'horloge */
	clkSetTime(clock, NbMS);
}

/**
 * Créer une Clock.
 *
 * @return L'adresse de la Clock créée.
 */
IClock createClock() {
	/* Allocation d'un emplacement pour le pointeur */
	IClock clock = malloc(sizeof(struct Clock));
	/* Initialisation de l'horloge */
	clock->TempsEcoule=now();
	return clock;
}

