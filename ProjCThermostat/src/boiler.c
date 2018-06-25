/**
 * @file boiler.c
 *
 * Un Boiler est un contr�leur pour la chaudi�re.
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
 * Cr�er un contr�leur de chaudi�re (Boiler).
 *
 * @return L'adresse du Boiler cr��.
 */
IBoiler createBoiler(IPort port) {
	IBoiler boiler = malloc(sizeof(struct Boiler));
	// par d�faut le boiler est �teint (bit 7 � 0)
	portWrite(port,0x7F);
	boiler->led = port;
	return boiler;
}

/**
 * Allume ou �teint la chaudi�re.
 *
 * @param boiler Ce Boiler.
 * @param bOn true pour allumer, false pour �teindre.
 */
void boilerSetOn(IBoiler boiler, bool bOn) {
	// si bOn est vrai on allumme en passant le bit 7 � 1
	if (bOn == true){
		portWrite(boiler->led,0x80);
	}
	if (bOn == false){
		portWrite(boiler->led,0x7F);
		}
}

/**
 * Indique si ce Boiler est allum� ou �teint.
 *
 * @param boiler Ce Boiler.
 * @return true quand ce Boiler est allum�, false sinon.
 */
bool boilerIsOn(IBoiler boiler) {
	// le boiler est allum� si le bit 7 est � 1
	return portReadLatch(boiler->led) >= 0x80;

}

