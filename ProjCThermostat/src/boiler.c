/**
 * @file boiler.c
 *
 * Un Boiler est un contrôleur pour la chaudière.
 *
 * Created on: 11 juil. 2012
 * @author Christophe TOMBELLE
 */

#include "appalloc.h"
#include "boiler.h"

struct Boiler {
	IPort led;
};

/**
 * Créer un contrôleur de chaudière (Boiler).
 *
 * @return L'adresse du Boiler créé.
 */
IBoiler createBoiler(IPort port) {
	IBoiler boiler = malloc(sizeof(struct Boiler));
	// par défaut le boiler est éteint (bit 7 à 0)
	portWrite(port,0x7F);
	boiler->led = port;
	return boiler;
}

/**
 * Allume ou éteint la chaudière.
 *
 * @param boiler Ce Boiler.
 * @param bOn true pour allumer, false pour éteindre.
 */
void boilerSetOn(IBoiler boiler, bool bOn) {
	// si bOn est vrai on allumme en passant le bit 7 à 1
	if (bOn == true){
		portWrite(boiler->led,0x80);
	}
	if (bOn == false){
		portWrite(boiler->led,0x7F);
		}
}

/**
 * Indique si ce Boiler est allumé ou éteint.
 *
 * @param boiler Ce Boiler.
 * @return true quand ce Boiler est allumé, false sinon.
 */
bool boilerIsOn(IBoiler boiler) {
	// le boiler est allumé si le bit 7 est à 1
	return portReadLatch(boiler->led) >= 0x80;

}

