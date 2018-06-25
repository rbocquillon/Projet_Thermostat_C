/**
 * @file main.c
 *
 * Created on: 11 juil. 2012
 * @author Christophe TOMBELLE
 */
#include <stdio.h>
#include "gboard.h"

#include "display.h"
#include "keyboard.h"
#include "lcd.h"
#include "port.h"
#include "thermui.h"
#include "thermostat.h"
#include "time.h"

/*
 * ***************************
 * NE PAS MODIFIER CE FICHIER
 * ***************************
 */

int main() {
	initSimulation();
	/** cr�er les objets */
	IClock clock = createClock();
	IPort portLeds = createPort(0x380000);
	IBoiler boiler = createBoiler(portLeds);
	IPort portADC = createPort(0x380001);
	ISensor sensor = createSensor(portADC);
	IThermostat therm = createThermostat(boiler, sensor, clock);
    IPort portCmd = createPort(0x380002);
    IPort portData = createPort(0x380003);
    ILCD lcd = createLCD( portCmd, portData );
    IDisplay disp = createDisplay( lcd );
    createThermUI(therm, disp);
    IPort portDip = createPort( 0x380000 );
    IKeyboard kbd = createKeyboard( portLeds, portDip);
    /** boucle principale */
	while (!simulationEnded()) {
		/* lire l'�tat du clavier */
		int iKey = kbdGetKey(kbd);
		if (iKey!=0)
			printf("%d\n", iKey);
		/* r�agir � un �v�nement clavier */
		uiProcessKey(iKey);
		/* piloter la chaudi�re selon la temp�rature et
		 * la consigne de temp�rature actuelles */
		thermRegulate(therm);
		/* mettre � jour l'affichage */
		uiUpdate();
	}
	return 0;
}
