/**
 * @file keyboard.c
 * Clavier constitué des touches TEMP, DAY, HOUR, MINUTE, Prog, NPROG, SUN,
 * MOON et OK. Si aucune touche n'est active la fonction kbdGetKey retourne 0.
 * Si une touche est maintenue appuyée, la fonction kbdGetKey retourne le code de
 * la touche au premier appel puis retourne 0 lors des appels suivants s'ils
 * interviennent avant un certain "délai de répétition". S'ils interviennent après
 * le délai, le code de la touche maintenue appuyée est de nouveau retourné. La
 * comptabilisation du délai redémarre à chaque fois que la fonction retourne un
 * code différent de 0.
 *
 * Created on: 11 juil. 2012
 * @author Christophe TOMBELLE
 */
#include <stdbool.h>
#include <stdio.h>

#include "appalloc.h"
#include "hal.h"
#include "keyboard.h"
#define TEMPS_ATTENTE 0

struct Keyboard {
	IPort _portLeds;
	IPort _portDip;
	bool IsLocked;
};

static bool INTR;
static long long toucheApp = 0;

/**
 * Interrupt Service Routine pour la gestion de la touche OK.
 */
static void kbdInterruptServiceRoutine() {
	INTR = true;
}

/**
 * Créer un Keyboard.
 *
 * @param portLeds Le port à utiliser pour le pilotage des colonnes du clavier matriciel
 * @param portDip Le port à utiliser pour la lecture des lignes du clavier matriciel
 *
 * @return L'adresse du Keyboard créé.
 */
IKeyboard createKeyboard(IPort portLeds, IPort portDip) {
	IKeyboard kbd = (IKeyboard)malloc(sizeof(struct Keyboard));
	kbd->IsLocked = false;
	kbd->_portLeds = portLeds;
	kbd->_portDip = portDip;
	installISR(kbdInterruptServiceRoutine);
	return kbd;
}

/**
 * Lire le code touche clavier indépendamment du mécanisme de répétition.
 *
 * @return Le code de touche (TEMP, DAY, ...) ou 0 si pas de touche active.
 */
static int kbdGetKeyRaw(IKeyboard kbd) {
	int MASK =  0xFC;
	int touche = 0;
	int val = portReadLatch(kbd->_portLeds);
	val = val & MASK;

	portWrite(kbd->_portLeds, val | 0x01);
	switch(portRead(kbd->_portDip)){
		case 0x01 :
			touche = TEMP;
			break;
		case 0x02:
			touche = DAY;
			break;
		case 0x04:
			touche = HOUR;
			break;
		case 0x08:
			touche = MINUTE;
			break;
	}

	portWrite(kbd->_portLeds, val | 0x02);
	switch(portRead(kbd->_portDip)){
		case 0x01 :
			touche = PROG;
			break;
		case 0x02:
			touche = NPROG;
			break;
		case 0x04:
			touche = SUN;
			break;
		case 0x08:
			touche = MOON;
			break;
		}

	if(INTR ==true){
		touche = OK;
		INTR = false;
	}
	return touche;
}

/**
 * Lire le code de la touche tenant compte du délai de répétition.
 *
 * @return Le code de touche (TEMP, DAY, ...) ou 0 si pas de touche active
 * ou si le délai de répétition n'est pas encore écoulé depuis le dernier
 * code de touche retourné différent de 0.
 */
int kbdGetKey(IKeyboard kbd) {
	int val = kbdGetKeyRaw(kbd);
	if (val!=0){
		if(toucheApp==TEMPS_ATTENTE){
			toucheApp = now();
			return val;
		}
		else{
			if(now()<toucheApp+REPEAT_DELAY){
				return 0;
			}
			else{
				toucheApp = now();
				return val;
			}
		}
	}
	else{
		toucheApp=TEMPS_ATTENTE;
		return 0;
	}
}

